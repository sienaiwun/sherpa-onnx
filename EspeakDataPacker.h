// EspeakDataPacker.h
// 工具：把 espeak-ng-data 目录打包成单一文件
#pragma once

#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <cstdint>

namespace sherpa_onnx {

// 简单的资源包格式
struct EspeakResourceEntry {
    std::string path;           // 相对路径，如 "voices/en/en-us"
    uint64_t offset;           // 在资源包中的偏移
    uint64_t size;             // 文件大小
};

struct EspeakResourcePack {
    std::vector<EspeakResourceEntry> entries;
    std::vector<char> data;     // 所有文件的二进制数据
};

class EspeakDataPacker {
public:
    // 打包整个 espeak-ng-data 目录到单一文件
    static bool PackDirectory(const std::string& espeak_data_dir, const std::string& output_pack_file);
    
    // 从资源包文件加载
    static bool LoadPack(const std::string& pack_file, EspeakResourcePack& pack);
    
    // 从内存加载资源包
    static bool LoadPackFromMemory(const void* pack_data, size_t pack_size, EspeakResourcePack& pack);
    
    // 从资源包中获取指定文件的数据
    static bool GetFileData(const EspeakResourcePack& pack, const std::string& file_path, std::vector<char>& out_data);

private:
    static void PackDirectoryRecursive(const std::string& dir_path, const std::string& relative_prefix, 
                                      std::vector<EspeakResourceEntry>& entries, std::vector<char>& data);
};

// UE风格的资源管理器
class EspeakMemoryFS {
public:
    // 从UE的TMap加载数据
    void LoadFromUEMap(const std::map<std::string, std::vector<uint8_t>>& ue_data);
    
    // 从资源包加载
    void LoadFromPack(const EspeakResourcePack& pack);
    
    // 获取文件数据
    bool GetFile(const std::string& path, std::vector<char>& out_data) const;
    
    // 检查文件是否存在
    bool FileExists(const std::string& path) const;

private:
    std::map<std::string, std::vector<char>> file_map_;
};

// 从内存中的pack数据初始化 espeak
bool InitEspeakFromMemory(const void* pack_data, size_t pack_size);

} // namespace sherpa_onnx