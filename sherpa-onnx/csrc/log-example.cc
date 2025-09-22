// sherpa-onnx/csrc/log-example.cc
//
// Example showing how to use the new log callback system with UE

#include "sherpa-onnx/csrc/macros.h"
#include <iostream>

// Example UE-style log callback (simulated)
void UELogCallback(enum sherpa_onnx_log_level level, const char* text, void* user_data) {
    // In real UE code, you would use:
    // UE_LOG(LogTemp, Error, TEXT("sherpa-onnx %s"), *FString(text));
    
    const char* level_name;
    switch (level) {
        case SHERPA_ONNX_LOG_LEVEL_ERROR:
            level_name = "ERROR";
            break;
        case SHERPA_ONNX_LOG_LEVEL_WARN:
            level_name = "WARN";
            break;
        case SHERPA_ONNX_LOG_LEVEL_INFO:
            level_name = "INFO";
            break;
        case SHERPA_ONNX_LOG_LEVEL_DEBUG:
            level_name = "DEBUG";
            break;
        default:
            level_name = "UNKNOWN";
    }
    
    // Simulate UE_LOG
    std::cout << "[UE_LOG][" << level_name << "] sherpa-onnx " << text << std::endl;
}

int main() {
    std::cout << "=== sherpa-onnx Log Callback Example ===" << std::endl;
    
    // Test default logging
    std::cout << "\n1. Default logging:" << std::endl;
    SHERPA_ONNX_LOGE("This is an error message");
    SHERPA_ONNX_LOGW("This is a warning message");
    SHERPA_ONNX_LOGI("This is an info message");
    SHERPA_ONNX_LOGD("This is a debug message");
    
    // Set custom UE-style callback
    std::cout << "\n2. After setting UE callback:" << std::endl;
    sherpa_onnx_log_set(UELogCallback, nullptr);
    
    SHERPA_ONNX_LOGE("Error from UE callback");
    SHERPA_ONNX_LOGW("Warning from UE callback");
    SHERPA_ONNX_LOGI("Info from UE callback");
    SHERPA_ONNX_LOGD("Debug from UE callback");
    
    // Reset to default
    std::cout << "\n3. Reset to default:" << std::endl;
    sherpa_onnx_log_set(nullptr, nullptr);
    
    SHERPA_ONNX_LOGE("Back to default logging");
    
    std::cout << "\n=== Example completed ===" << std::endl;
    return 0;
}