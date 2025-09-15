// espeak_data_packer_tool.cpp
// 工具：把 espeak-ng-data 目录打包成单一 .pack 文件，并支持测试功能
#include "EspeakDataPacker.h"
#include <iostream>
#include <string>
#include <filesystem>

void print_usage(const char* program_name) {
    std::cout << "espeak-ng-data Packer Tool v1.0" << std::endl;
    std::cout << std::endl;
    std::cout << "USAGE:" << std::endl;
    std::cout << "  Pack directory:  " << program_name << " pack <espeak-ng-data-directory> <output.pack>" << std::endl;
    std::cout << "  Test pack file:  " << program_name << " test <input.pack>" << std::endl;
    std::cout << "  List pack files: " << program_name << " list <input.pack>" << std::endl;
    std::cout << std::endl;
    std::cout << "EXAMPLES:" << std::endl;
    std::cout << "  # 打包目录" << std::endl;
    std::cout << "  " << program_name << " pack ./espeak-ng-data ./espeak-ng-data.pack" << std::endl;
    std::cout << std::endl;
    std::cout << "  # 测试包文件（会尝试初始化espeak）" << std::endl;
    std::cout << "  " << program_name << " test ./espeak-ng-data.pack" << std::endl;
    std::cout << std::endl;
    std::cout << "  # 列出包中的所有文件" << std::endl;
    std::cout << "  " << program_name << " list ./espeak-ng-data.pack" << std::endl;
}

int pack_directory(const std::string& espeak_data_dir, const std::string& output_pack_file) {
    // 检查输入目录是否存在
    if (!std::filesystem::exists(espeak_data_dir)) {
        std::cerr << "Error: Input directory does not exist: " << espeak_data_dir << std::endl;
        return 1;
    }

    if (!std::filesystem::is_directory(espeak_data_dir)) {
        std::cerr << "Error: Input path is not a directory: " << espeak_data_dir << std::endl;
        return 1;
    }

    std::cout << "=== PACKING ESPEAK-NG-DATA ===" << std::endl;
    std::cout << "Input directory: " << espeak_data_dir << std::endl;
    std::cout << "Output pack file: " << output_pack_file << std::endl;
    std::cout << std::endl;

    if (sherpa_onnx::EspeakDataPacker::PackDirectory(espeak_data_dir, output_pack_file)) {
        std::cout << std::endl << "✅ SUCCESS! Created pack file: " << output_pack_file << std::endl;
        
        // 显示文件大小信息
        try {
            auto file_size = std::filesystem::file_size(output_pack_file);
            std::cout << "Pack file size: " << file_size << " bytes (" 
                     << (file_size / 1024) << " KB)" << std::endl;
        } catch (const std::exception& e) {
            // 忽略文件大小获取错误
        }
        
        return 0;
    } else {
        std::cerr << "❌ FAILED to create pack file." << std::endl;
        return 1;
    }
}

int test_pack_file(const std::string& pack_file) {
    // 检查包文件是否存在
    if (!std::filesystem::exists(pack_file)) {
        std::cerr << "Error: Pack file does not exist: " << pack_file << std::endl;
        return 1;
    }

    std::cout << "=== TESTING PACK FILE ===" << std::endl;
    std::cout << "Pack file: " << pack_file << std::endl;
    std::cout << std::endl;

    try {
        // 加载包文件
        sherpa_onnx::EspeakResourcePack pack;
        if (!sherpa_onnx::EspeakDataPacker::LoadPack(pack_file, pack)) {
            std::cerr << "❌ FAILED to load pack file" << std::endl;
            return 1;
        }

        std::cout << "✅ Pack file loaded successfully" << std::endl;
        std::cout << "Number of files: " << pack.entries.size() << std::endl;
        std::cout << "Data size: " << pack.data.size() << " bytes" << std::endl;
        std::cout << std::endl;

        // 测试初始化espeak
        std::cout << "Testing espeak initialization..." << std::endl;
        
        // 读取包文件到内存
        std::ifstream file(pack_file, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "❌ FAILED to open pack file for testing" << std::endl;
            return 1;
        }
        
        file.seekg(0, std::ios::end);
        size_t file_size = file.tellg();
        file.seekg(0, std::ios::beg);
        
        std::vector<char> pack_data(file_size);
        file.read(pack_data.data(), file_size);
        
        if (sherpa_onnx::InitEspeakFromMemory(pack_data.data(), file_size)) {
            std::cout << "✅ SUCCESS! Espeak initialized from pack file" << std::endl;
            return 0;
        } else {
            std::cerr << "❌ FAILED to initialize espeak from pack file" << std::endl;
            return 1;
        }
        
    } catch (const std::exception& e) {
        std::cerr << "❌ ERROR during pack file testing: " << e.what() << std::endl;
        return 1;
    }
}

int list_pack_files(const std::string& pack_file) {
    // 检查包文件是否存在
    if (!std::filesystem::exists(pack_file)) {
        std::cerr << "Error: Pack file does not exist: " << pack_file << std::endl;
        return 1;
    }

    std::cout << "=== LISTING PACK CONTENTS ===" << std::endl;
    std::cout << "Pack file: " << pack_file << std::endl;
    std::cout << std::endl;

    try {
        // 加载包文件
        sherpa_onnx::EspeakResourcePack pack;
        if (!sherpa_onnx::EspeakDataPacker::LoadPack(pack_file, pack)) {
            std::cerr << "❌ FAILED to load pack file" << std::endl;
            return 1;
        }

        std::cout << "Total files: " << pack.entries.size() << std::endl;
        std::cout << "Total data size: " << pack.data.size() << " bytes" << std::endl;
        std::cout << std::endl;

        std::cout << "Files in pack:" << std::endl;
        std::cout << "-------------------------------------------" << std::endl;
        
        size_t total_size = 0;
        for (const auto& entry : pack.entries) {
            std::cout << entry.path << " (" << entry.size << " bytes)" << std::endl;
            total_size += entry.size;
        }
        
        std::cout << "-------------------------------------------" << std::endl;
        std::cout << "Total file size: " << total_size << " bytes" << std::endl;
        
        return 0;
        
    } catch (const std::exception& e) {
        std::cerr << "❌ ERROR during pack file listing: " << e.what() << std::endl;
        return 1;
    }
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        print_usage(argv[0]);
        return 1;
    }

    std::string command = argv[1];

    if (command == "pack") {
        if (argc != 4) {
            std::cerr << "Error: pack command requires 2 arguments" << std::endl;
            print_usage(argv[0]);
            return 1;
        }
        return pack_directory(argv[2], argv[3]);
        
    } else if (command == "test") {
        if (argc != 3) {
            std::cerr << "Error: test command requires 1 argument" << std::endl;
            print_usage(argv[0]);
            return 1;
        }
        return test_pack_file(argv[2]);
        
    } else if (command == "list") {
        if (argc != 3) {
            std::cerr << "Error: list command requires 1 argument" << std::endl;
            print_usage(argv[0]);
            return 1;
        }
        return list_pack_files(argv[2]);
        
    } else {
        std::cerr << "Error: Unknown command: " << command << std::endl;
        print_usage(argv[0]);
        return 1;
    }
}