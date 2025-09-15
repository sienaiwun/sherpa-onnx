// EspeakDataPacker.cpp
// 实现：espeak-ng-data 目录打包和内存访问
#include "EspeakDataPacker.h"
#include <filesystem>
#include <iostream>
#include <cstring>
#include <fstream>

namespace sherpa_onnx {

// 资源包文件格式：
// [Header: 8 bytes] "ESPKDATA"
// [Entry Count: 4 bytes] uint32_t 文件数量
// [Entry Table: variable] 每个文件的信息
//   - [Path Length: 2 bytes] uint16_t 路径长度
//   - [Path: variable] 相对路径字符串
//   - [Offset: 8 bytes] uint64_t 在数据区的偏移
//   - [Size: 8 bytes] uint64_t 文件大小
// [Data Section: variable] 所有文件的连续二进制数据

bool EspeakDataPacker::PackDirectory(const std::string& espeak_data_dir, const std::string& output_pack_file) {
    std::vector<EspeakResourceEntry> entries;
    std::vector<char> data;
    
    // 递归扫描目录
    PackDirectoryRecursive(espeak_data_dir, "", entries, data);
    
    // 写入资源包文件
    std::ofstream out(output_pack_file, std::ios::binary);
    if (!out) {
        std::cerr << "Failed to create pack file: " << output_pack_file << std::endl;
        return false;
    }
    
    // 写入文件头
    out.write("ESPKDATA", 8);
    
    // 写入文件数量
    uint32_t entry_count = static_cast<uint32_t>(entries.size());
    out.write(reinterpret_cast<const char*>(&entry_count), sizeof(entry_count));
    
    // 计算数据区偏移（Header + Entry Count + Entry Table Size）
    uint64_t data_offset = 8 + 4; // Header + Entry Count
    for (const auto& entry : entries) {
        data_offset += 2 + entry.path.size() + 8 + 8; // Path Length + Path + Offset + Size
    }
    
    // 写入文件条目表
    uint64_t current_data_offset = 0;
    for (auto& entry : entries) {
        // 路径长度
        uint16_t path_len = static_cast<uint16_t>(entry.path.size());
        out.write(reinterpret_cast<const char*>(&path_len), sizeof(path_len));
        
        // 路径
        out.write(entry.path.c_str(), entry.path.size());
        
        // 计算实际偏移
        entry.offset = data_offset + current_data_offset;
        current_data_offset += entry.size;
        
        // 偏移和大小
        out.write(reinterpret_cast<const char*>(&entry.offset), sizeof(entry.offset));
        out.write(reinterpret_cast<const char*>(&entry.size), sizeof(entry.size));
    }
    
    // 写入数据区
    out.write(data.data(), data.size());
    
    std::cout << "Packed " << entries.size() << " files (" << data.size() << " bytes) to " << output_pack_file << std::endl;
    return true;
}

void EspeakDataPacker::PackDirectoryRecursive(const std::string& dir_path, const std::string& relative_prefix,
                                             std::vector<EspeakResourceEntry>& entries, std::vector<char>& data) {
    try {
        for (const auto& entry : std::filesystem::directory_iterator(dir_path)) {
            std::string filename = entry.path().filename().string();
            std::string relative_path = relative_prefix.empty() ? filename : relative_prefix + "/" + filename;
            
            if (entry.is_directory()) {
                // 递归处理子目录
                PackDirectoryRecursive(entry.path().string(), relative_path, entries, data);
            } else if (entry.is_regular_file()) {
                // 处理文件
                std::ifstream file(entry.path(), std::ios::binary);
                if (file) {
                    // 获取文件大小
                    file.seekg(0, std::ios::end);
                    uint64_t file_size = file.tellg();
                    file.seekg(0, std::ios::beg);
                    
                    // 读取文件数据
                    size_t current_offset = data.size();
                    data.resize(data.size() + file_size);
                    file.read(data.data() + current_offset, file_size);
                    
                    // 创建条目
                    EspeakResourceEntry resource_entry;
                    resource_entry.path = relative_path;
                    resource_entry.offset = current_offset; // 临时偏移，后面会调整
                    resource_entry.size = file_size;
                    entries.push_back(resource_entry);
                    
                    std::cout << "Packed: " << relative_path << " (" << file_size << " bytes)" << std::endl;
                }
            }
        }
    } catch (const std::exception& e) {
        std::cerr << "Error scanning directory " << dir_path << ": " << e.what() << std::endl;
    }
}

bool EspeakDataPacker::LoadPack(const std::string& pack_file, EspeakResourcePack& pack) {
    std::ifstream file(pack_file, std::ios::binary);
    if (!file.is_open()) {
        std::cerr << "Error: Could not open pack file: " << pack_file << std::endl;
        return false;
    }

    // 获取文件大小
    file.seekg(0, std::ios::end);
    size_t file_size = file.tellg();
    file.seekg(0, std::ios::beg);

    // 读取整个文件到内存
    std::vector<char> file_data(file_size);
    file.read(file_data.data(), file_size);
    if (file.fail()) {
        std::cerr << "Error: Failed to read pack file: " << pack_file << std::endl;
        return false;
    }
    return InitEspeakFromMemory(file_data.data(), file_size);
    // 使用内存数据解析
    //return LoadPackFromMemory(file_data.data(), file_size, pack);
}

bool EspeakDataPacker::LoadPackFromMemory(const void* pack_data, size_t pack_size, EspeakResourcePack& pack) {
    if (!pack_data || pack_size == 0) {
        std::cerr << "Error: Invalid pack data or size" << std::endl;
        return false;
    }

    const char* data_ptr = static_cast<const char*>(pack_data);
    size_t pos = 0;

    // 检查并读取文件头 "ESPKDATA"
    if (pos + 8 > pack_size) {
        std::cerr << "Error: Pack data too small for header" << std::endl;
        return false;
    }

    if (std::memcmp(data_ptr + pos, "ESPKDATA", 8) != 0) {
        std::cerr << "Error: Invalid pack file header" << std::endl;
        return false;
    }
    pos += 8;

    // 检查并读取条目数量
    if (pos + sizeof(uint32_t) > pack_size) {
        std::cerr << "Error: Pack data too small to contain entry count" << std::endl;
        return false;
    }

    uint32_t entry_count;
    std::memcpy(&entry_count, data_ptr + pos, sizeof(entry_count));
    pos += sizeof(entry_count);

    pack.entries.clear();
    pack.entries.reserve(entry_count);

    // 读取所有条目的元数据
    for (uint32_t i = 0; i < entry_count; ++i) {
        EspeakResourceEntry entry;
        
        // 检查路径长度 (uint16_t)
        if (pos + sizeof(uint16_t) > pack_size) {
            std::cerr << "Error: Pack data truncated at entry " << i << " path length" << std::endl;
            return false;
        }

        // 读取路径长度
        uint16_t path_length;
        std::memcpy(&path_length, data_ptr + pos, sizeof(path_length));
        pos += sizeof(path_length);

        // 检查路径数据
        if (pos + path_length > pack_size) {
            std::cerr << "Error: Pack data truncated at entry " << i << " path data" << std::endl;
            return false;
        }

        // 读取路径
        entry.path.assign(data_ptr + pos, path_length);
        pos += path_length;

        // 检查偏移和大小数据
        if (pos + sizeof(entry.offset) + sizeof(entry.size) > pack_size) {
            std::cerr << "Error: Pack data truncated at entry " << i << " offset/size" << std::endl;
            return false;
        }

        // 读取偏移和大小
        std::memcpy(&entry.offset, data_ptr + pos, sizeof(entry.offset));
        pos += sizeof(entry.offset);
        std::memcpy(&entry.size, data_ptr + pos, sizeof(entry.size));
        pos += sizeof(entry.size);

        pack.entries.push_back(entry);
    }

    // 数据区从当前位置开始到文件末尾
    size_t data_start = pos;
    size_t data_size = pack_size - data_start;

    // 读取所有文件数据（实际上就是剩余的所有数据）
    pack.data.assign(data_ptr + data_start, data_ptr + pack_size);

    std::cout << "Successfully loaded pack from memory with " << entry_count << " files, data size: " << data_size << " bytes" << std::endl;
    return true;
}

bool EspeakDataPacker::GetFileData(const EspeakResourcePack& pack, const std::string& file_path, std::vector<char>& out_data) {
    for (const auto& entry : pack.entries) {
        if (entry.path == file_path) {
            // 直接使用 pack.data，因为在 LoadPackFromMemory 中，数据偏移是相对于数据区开始的
            // 我们需要重新计算在 pack.data 中的偏移
            size_t relative_offset = 0;
            for (const auto& e : pack.entries) {
                if (e.path == entry.path) break;
                relative_offset += e.size;
            }
            
            if (relative_offset + entry.size <= pack.data.size()) {
                out_data.assign(pack.data.begin() + relative_offset, pack.data.begin() + relative_offset + entry.size);
                return true;
            } else {
                std::cerr << "Error: File data out of bounds for " << file_path 
                         << " (offset: " << relative_offset << ", size: " << entry.size 
                         << ", pack data size: " << pack.data.size() << ")" << std::endl;
                return false;
            }
        }
    }
    std::cerr << "Error: File not found in pack: " << file_path << std::endl;
    return false;
}

// EspeakMemoryFS 实现
void EspeakMemoryFS::LoadFromUEMap(const std::map<std::string, std::vector<uint8_t>>& ue_data) {
    file_map_.clear();
    for (const auto& [path, data] : ue_data) {
        file_map_[path].assign(data.begin(), data.end());
    }
}

void EspeakMemoryFS::LoadFromPack(const EspeakResourcePack& pack) {
    file_map_.clear();
    for (const auto& entry : pack.entries) {
        std::vector<char> file_data;
        if (EspeakDataPacker::GetFileData(pack, entry.path, file_data)) {
            file_map_[entry.path] = std::move(file_data);
        }
    }
}

bool EspeakMemoryFS::GetFile(const std::string& path, std::vector<char>& out_data) const {
    auto it = file_map_.find(path);
    if (it != file_map_.end()) {
        out_data = it->second;
        return true;
    }
    return false;
}

bool EspeakMemoryFS::FileExists(const std::string& path) const {
    return file_map_.find(path) != file_map_.end();
}

// 从内存中的pack数据初始化 espeak
bool InitEspeakFromMemory(const void* pack_data, size_t pack_size) {
    if (!pack_data || pack_size == 0) {
        std::cerr << "Error: Invalid pack data or size for InitEspeakFromMemory" << std::endl;
        return false;
    }

    // 使用固定的临时目录
    std::string temp_dir;
    try {
        // 使用 std::filesystem 获取临时目录，创建固定名称的子目录
        std::filesystem::path temp_path = std::filesystem::temp_directory_path();
        temp_dir = (temp_path / "sherpa_onnx_espeak_data").string();
        
        // 检查是否已经解压过文件
        bool already_extracted = false;
        if (std::filesystem::exists(temp_dir)) {
            // 检查关键文件是否存在（phontab, phondata, phonindex 是 espeak 的核心文件）
            std::vector<std::string> key_files = {"phontab", "phondata", "phonindex"};
            bool has_key_files = true;
            
            for (const auto& key_file : key_files) {
                std::string key_file_path = temp_dir + "/" + key_file;
                if (!std::filesystem::exists(key_file_path)) {
                    has_key_files = false;
                    break;
                }
            }
            
            if (has_key_files) {
                already_extracted = true;
                std::cout << "Espeak data already extracted to: " << temp_dir << std::endl;
            }
        }
        
        // 如果还没有解压，则进行解压
        if (!already_extracted) {
            // 解析内存中的 pack 数据
            EspeakResourcePack pack;
            if (!EspeakDataPacker::LoadPackFromMemory(pack_data, pack_size, pack)) {
                std::cerr << "Error: Failed to load pack from memory" << std::endl;
                return false;
            }
            
            // 创建目录
            std::filesystem::create_directories(temp_dir);
            
            // 解包所有文件到固定临时目录
            std::cout << "Extracting espeak data to: " << temp_dir << std::endl;
            for (const auto& entry : pack.entries) {
                std::string full_path = temp_dir + "/" + entry.path;
                
                // 创建父目录
                std::filesystem::path file_path(full_path);
                std::filesystem::create_directories(file_path.parent_path());
                
                // 获取文件数据并写入
                std::vector<char> file_data;
                if (EspeakDataPacker::GetFileData(pack, entry.path, file_data)) {
                    std::ofstream out(full_path, std::ios::binary);
                    if (out) {
                        out.write(file_data.data(), file_data.size());
                        std::cout << "Extracted: " << full_path << " (" << file_data.size() << " bytes)" << std::endl;
                    } else {
                        std::cerr << "Error: Failed to write file: " << full_path << std::endl;
                        return false;
                    }
                } else {
                    std::cerr << "Error: Failed to get data for file: " << entry.path << std::endl;
                    return false;
                }
            }
            std::cout << "Successfully extracted " << pack.entries.size() << " files to: " << temp_dir << std::endl;
        }
        
        std::cout << "Using espeak data directory: " << temp_dir << std::endl;
        return true;
        
    } catch (const std::exception& e) {
        std::cerr << "Error during pack extraction: " << e.what() << std::endl;
        return false;
    }
}


} // namespace sherpa_onnx
