// espeak_data_unpacker_tool.cpp
// 独立工具：将 espeak_data.pack 文件解包到目录
#include "EspeakDataPacker.h"
#include <iostream>
#include <string>
#include <filesystem>
#include <chrono>

void PrintUsage(const char* program_name) {
    std::cout << "Usage: " << program_name << " <pack-file> <output-directory>\n";
    std::cout << "\n";
    std::cout << "Examples:\n";
    std::cout << "  " << program_name << " espeak_data.pack ./extracted-espeak-data\n";
    std::cout << "  " << program_name << " C:/data/espeak_data.pack C:/output/espeak-ng-data\n";
    std::cout << "\n";
    std::cout << "This tool unpacks an espeak_data.pack file back to individual files\n";
    std::cout << "in a directory structure, restoring the original espeak-ng-data layout.\n";
}

bool ValidatePackFile(const std::string& pack_path) {
    if (!std::filesystem::exists(pack_path)) {
        std::cerr << "Error: Pack file does not exist: " << pack_path << std::endl;
        return false;
    }
    
    if (!std::filesystem::is_regular_file(pack_path)) {
        std::cerr << "Error: Path is not a regular file: " << pack_path << std::endl;
        return false;
    }
    
    // 检查文件大小
    auto file_size = std::filesystem::file_size(pack_path);
    if (file_size < 12) { // 至少需要 header (8) + count (4)
        std::cerr << "Error: Pack file is too small to be valid: " << file_size << " bytes" << std::endl;
        return false;
    }
    
    std::cout << "Pack file size: " << file_size << " bytes (";
    if (file_size >= 1024 * 1024) {
        std::cout << (file_size / (1024.0 * 1024.0)) << " MB";
    } else if (file_size >= 1024) {
        std::cout << (file_size / 1024.0) << " KB";
    } else {
        std::cout << file_size << " bytes";
    }
    std::cout << ")" << std::endl;
    
    return true;
}

bool ValidateOutputDir(const std::string& output_dir) {
    std::filesystem::path dir_path(output_dir);
    
    // 如果目录已存在，询问是否覆盖
    if (std::filesystem::exists(dir_path)) {
        if (!std::filesystem::is_directory(dir_path)) {
            std::cerr << "Error: Output path exists but is not a directory: " << output_dir << std::endl;
            return false;
        }
        
        // 检查目录是否为空
        bool is_empty = std::filesystem::is_empty(dir_path);
        if (!is_empty) {
            std::cout << "Warning: Output directory is not empty: " << output_dir << std::endl;
            std::cout << "Files in the directory may be overwritten." << std::endl;
            std::cout << "Continue anyway? (y/N): ";
            
            std::string response;
            std::getline(std::cin, response);
            return (response == "y" || response == "Y" || response == "yes");
        }
    } else {
        // 创建目录
        try {
            std::filesystem::create_directories(dir_path);
            std::cout << "Created output directory: " << output_dir << std::endl;
        } catch (const std::exception& e) {
            std::cerr << "Error: Failed to create output directory: " << e.what() << std::endl;
            return false;
        }
    }
    
    return true;
}

void PrintUnpackingSummary(const std::string& pack_file, const std::string& output_dir) {
    std::cout << "\n=== Unpacking Summary ===\n";
    std::cout << "Input pack file: " << pack_file << std::endl;
    std::cout << "Output directory: " << output_dir << std::endl;
    std::cout << "\n";
}

int main(int argc, char* argv[]) {
    std::cout << "Espeak-NG Data Unpacker Tool v1.0\n";
    std::cout << "Copyright (c) 2024 - Unpack espeak_data.pack to individual files\n\n";
    
    // 检查命令行参数
    if (argc != 3) {
        PrintUsage(argv[0]);
        return 1;
    }
    
    std::string pack_file = argv[1];
    std::string output_dir = argv[2];
    
    // 规范化路径
    try {
        pack_file = std::filesystem::absolute(pack_file).string();
        output_dir = std::filesystem::absolute(output_dir).string();
    } catch (const std::exception& e) {
        std::cerr << "Error processing paths: " << e.what() << std::endl;
        return 1;
    }
    
   
    // 显示解包信息摘要
    PrintUnpackingSummary(pack_file, output_dir);
    
 
    
    // 执行解包
    std::cout << "Loading pack file...\n";
    
    auto start_time = std::chrono::high_resolution_clock::now();
    
    // 加载资源包
    sherpa_onnx::EspeakResourcePack pack;
    if (!sherpa_onnx::EspeakDataPacker::LoadPack(pack_file, pack)) {
        std::cerr << "\n❌ Failed to load pack file!\n";
        std::cerr << "Please check if the file is a valid espeak_data.pack file.\n";
        return 1;
    }
    
    std::cout << "Pack file loaded successfully!\n";
    std::cout << "Found " << pack.entries.size() << " files in the pack.\n\n";
    
    std::cout << "Extracting files...\n";
    
    // 解包文件
    size_t extracted_count = 0;
    size_t failed_count = 0;
    
    for (const auto& entry : pack.entries) {
        try {
            // 构建完整的输出路径
            std::filesystem::path full_output_path = std::filesystem::path(output_dir) / entry.path;
            
            // 创建必要的父目录
            std::filesystem::create_directories(full_output_path.parent_path());
            
            // 获取文件数据
            std::vector<char> file_data;
            if (!sherpa_onnx::EspeakDataPacker::GetFileData(pack, entry.path, file_data)) {
                std::cerr << "Failed to get data for file: " << entry.path << std::endl;
                failed_count++;
                continue;
            }
            
            // 写入文件
            std::ofstream out_file(full_output_path, std::ios::binary);
            if (!out_file) {
                std::cerr << "Failed to create file: " << full_output_path << std::endl;
                failed_count++;
                continue;
            }
            
            out_file.write(file_data.data(), file_data.size());
            out_file.close();
            
            if (out_file.good()) {
                extracted_count++;
                if (extracted_count % 10 == 0 || extracted_count <= 5) {
                    std::cout << "  [" << extracted_count << "/" << pack.entries.size() << "] " << entry.path << std::endl;
                }
            } else {
                std::cerr << "Failed to write file: " << full_output_path << std::endl;
                failed_count++;
            }
            
        } catch (const std::exception& e) {
            std::cerr << "Error extracting " << entry.path << ": " << e.what() << std::endl;
            failed_count++;
        }
    }
    
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time);
    
    // 显示结果
    std::cout << "\n";
    if (failed_count == 0) {
        std::cout << "✅ Unpacking completed successfully!\n";
    } else {
        std::cout << "⚠️ Unpacking completed with warnings!\n";
    }
    
    std::cout << "Time taken: " << duration.count() << " ms\n";
    std::cout << "Files extracted: " << extracted_count << "/" << pack.entries.size() << std::endl;
    
    if (failed_count > 0) {
        std::cout << "Failed files: " << failed_count << std::endl;
    }
    
    std::cout << "\nExtracted files are now available in: " << output_dir << std::endl;
    
    // 显示一些提取的文件作为验证
    std::cout << "\nSample extracted files:\n";
    int sample_count = 0;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(output_dir)) {
        if (entry.is_regular_file() && sample_count < 5) {
            auto rel_path = std::filesystem::relative(entry.path(), output_dir);
            std::cout << "  " << rel_path.string() << " (" << entry.file_size() << " bytes)\n";
            sample_count++;
        }
    }
    
    if (failed_count == 0) {
        return 0;
    } else {
        return 1;
    }
}