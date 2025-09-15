#include "sherpa-onnx/csrc/EspeakDataPacker.h"

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>

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

std::string ExtractEspeakDataToTemp(const void* pack_data, size_t pack_data_size) {
    if (!pack_data || pack_data_size == 0) {
        SHERPA_ONNX_LOGE("Invalid pack data parameters");
        return "";
    }

    try {
        // Create a temporary directory for espeak data
        std::filesystem::path temp_dir = std::filesystem::temp_directory_path();
        temp_dir /= "sherpa_espeak_" + std::to_string(GetTickCount());
        
        if (!std::filesystem::create_directories(temp_dir)) {
            SHERPA_ONNX_LOGE("Failed to create temporary directory: %s", temp_dir.string().c_str());
            return "";
        }
        
        // Store the temp directory for cleanup later
        temp_espeak_dir = temp_dir.string();
        
        // For now, we'll create a simple implementation that extracts the data
        // This is a placeholder - in a real implementation, you would need to:
        // 1. Parse the packed data format (e.g., ZIP, TAR, custom format)
        // 2. Extract files to the temporary directory
        // 3. Maintain the correct directory structure for espeak-ng-data
        
        // Simple implementation: assume pack_data contains raw espeak-ng-data
        // In reality, you would parse the pack format and extract properly
        std::filesystem::path espeak_data_dir = temp_dir / "espeak-ng-data";
        if (!std::filesystem::create_directories(espeak_data_dir)) {
            SHERPA_ONNX_LOGE("Failed to create espeak data directory: %s", espeak_data_dir.string().c_str());
            return "";
        }
        
        // Write the packed data to a file (this is a simplified approach)
        // In a real implementation, you would extract the actual espeak data files
        std::ofstream out(espeak_data_dir / "packed_data.bin", std::ios::binary);
        if (!out) {
            SHERPA_ONNX_LOGE("Failed to create packed data file");
            return "";
        }
        
        out.write(static_cast<const char*>(pack_data), pack_data_size);
        out.close();
        
        SHERPA_ONNX_LOGE("Extracted espeak data to temporary directory: %s", temp_dir.string().c_str());
        return temp_dir.string();
        
    } catch (const std::exception& e) {
        SHERPA_ONNX_LOGE("Exception during espeak data extraction: %s", e.what());
        CleanupEspeakTempData();
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