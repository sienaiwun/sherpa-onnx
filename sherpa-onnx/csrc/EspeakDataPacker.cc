#include "../../EspeakDataPacker.h"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <cstring>
#include <iostream>

#ifdef _WIN32
#include <windows.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#include "espeak-ng/speak_lib.h"
#include "sherpa-onnx/csrc/macros.h"

namespace sherpa_onnx {

static std::string temp_espeak_dir;

// Forward declaration
void CleanupEspeakTempData();

// Implementation of LoadPackFromMemory
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

// Implementation of GetFileData
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

std::string ExtractEspeakDataToTemp(const void* pack_data, size_t pack_data_size) {
    if (!pack_data || pack_data_size == 0) {
        return "";
    }

    try {
        // 使用固定的临时目录
        std::filesystem::path temp_path = std::filesystem::temp_directory_path();
        std::filesystem::path tmp = temp_path / "sherpa_onnx_espeak_data";

        // 如果关键文件已存在则直接复用
        if (std::filesystem::exists(tmp)) {
            std::vector<std::string> keys = {"phontab", "phondata", "phonindex"};
            bool ok = true;
            for (const auto &k : keys) {
                if (!std::filesystem::exists(tmp / k)) { ok = false; break; }
            }
            if (ok) {
                temp_espeak_dir = tmp.string();
                return temp_espeak_dir;
            }
        }

        // 解析 pack 内容
        EspeakResourcePack pack;
        if (!EspeakDataPacker::LoadPackFromMemory(pack_data, pack_data_size, pack)) {
            return "";
        }

        // 创建目录并解包所有文件
        std::filesystem::create_directories(tmp);
        for (const auto &entry : pack.entries) {
            std::filesystem::path out_path = tmp / entry.path;
            std::filesystem::create_directories(out_path.parent_path());

            std::vector<char> file_data;
            if (!EspeakDataPacker::GetFileData(pack, entry.path, file_data)) {
                return "";
            }

            std::ofstream out(out_path, std::ios::binary);
            if (!out) { return ""; }
            out.write(file_data.data(), static_cast<std::streamsize>(file_data.size()));
            if (!out) { return ""; }
        }

        temp_espeak_dir = tmp.string();
        return temp_espeak_dir;
    } catch (...) {
        return "";
    }
}

bool InitEspeakNgFromMemory(const void* pack_data, size_t pack_data_size) {
    // Extract data to temporary directory
    std::string temp_dir = ExtractEspeakDataToTemp(pack_data, pack_data_size);
    if (temp_dir.empty()) {
        return false;
    }
    
    try {
        // Initialize espeak with the temporary directory
        int32_t result = espeak_Initialize(AUDIO_OUTPUT_SYNCHRONOUS, 0, temp_dir.c_str(), 0);
        if (result != 22050) {
            SHERPA_ONNX_LOGE("Failed to initialize espeak-ng from memory. Return code is: %d", result);
            CleanupEspeakTempData();
            return false;
        }
        
        return true;
        
    } catch (const std::exception& e) {
        SHERPA_ONNX_LOGE("Exception during espeak initialization from memory: %s", e.what());
        CleanupEspeakTempData();
        return false;
    }
}

void CleanupEspeakTempData() {
    if (!temp_espeak_dir.empty()) {
        try {
            std::filesystem::remove_all(temp_espeak_dir);
        } catch (const std::exception& e) {
            SHERPA_ONNX_LOGE("Failed to cleanup temporary espeak data: %s", e.what());
        }
        temp_espeak_dir.clear();
    }
}

}  // namespace sherpa_onnx