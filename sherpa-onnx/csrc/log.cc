// sherpa-onnx/csrc/log.cc
//
// Copyright      2023  Xiaomi Corporation

#include "sherpa-onnx/csrc/macros.h"
#include <string>
#include <mutex>
#include <cstdio>
#include <cstring>

// Global state for log callback (similar to llama.cpp implementation)
static sherpa_onnx_log_callback g_log_callback = nullptr;
static void* g_log_user_data = nullptr;
static std::mutex g_log_mutex;

// Default logging function with platform-specific behavior
static void sherpa_onnx_log_default(SherpaOnnxLogLevel level, const char* text, void* user_data) {
    (void)user_data;  // Suppress unused parameter warning
    
    const char* level_str;
    FILE* output = stderr;
    
    switch (level) {
        case SHERPA_ONNX_LOG_LEVEL_ERROR:
            level_str = "ERROR";
            break;
        case SHERPA_ONNX_LOG_LEVEL_WARN:
            level_str = "WARN";
            break;
        case SHERPA_ONNX_LOG_LEVEL_INFO:
            level_str = "INFO";
            output = stdout;
            break;
        case SHERPA_ONNX_LOG_LEVEL_DEBUG:
            level_str = "DEBUG";
            output = stdout;
            break;
        default:
            level_str = "UNKNOWN";
            break;
    }
    
#if __ANDROID_API__ >= 8
    #include "android/log.h"
    android_LogPriority priority;
    switch (level) {
        case SHERPA_ONNX_LOG_LEVEL_ERROR:
            priority = ANDROID_LOG_ERROR;
            break;
        case SHERPA_ONNX_LOG_LEVEL_WARN:
            priority = ANDROID_LOG_WARN;
            break;
        case SHERPA_ONNX_LOG_LEVEL_INFO:
            priority = ANDROID_LOG_INFO;
            break;
        case SHERPA_ONNX_LOG_LEVEL_DEBUG:
            priority = ANDROID_LOG_DEBUG;
            break;
        default:
            priority = ANDROID_LOG_UNKNOWN;
            break;
    }
    __android_log_print(priority, "sherpa-onnx", "%s", text);
#elif defined(__OHOS__)
    #include "hilog/log.h"
    OH_LOG_INFO(LOG_APP, "%s: %s", level_str, text);
#elif SHERPA_ONNX_ENABLE_WASM
    fprintf(stdout, "[%s] %s\n", level_str, text);
#else
    fprintf(output, "[%s] %s\n", level_str, text);
    fflush(output);
#endif
}


void sherpa_onnx_log_set(sherpa_onnx_log_callback log_callback, void* user_data) {
    std::lock_guard<std::mutex> lock(g_log_mutex);
    g_log_callback = log_callback;
    g_log_user_data = user_data;
}

sherpa_onnx_log_callback sherpa_onnx_log_get(void) {
    std::lock_guard<std::mutex> lock(g_log_mutex);
    return g_log_callback;
}

void* sherpa_onnx_log_get_user_data(void) {
    std::lock_guard<std::mutex> lock(g_log_mutex);
    return g_log_user_data;
}

void sherpa_onnx_log_internal(SherpaOnnxLogLevel level, const char* file, const char* func, int line, const char* format, ...) {
    // Get current callback and user data
    std::lock_guard<std::mutex> lock(g_log_mutex);
    sherpa_onnx_log_callback callback = g_log_callback ? g_log_callback : sherpa_onnx_log_default;
    void* user_data = g_log_user_data;
    
    // Format the message
    char buffer[4096];
    va_list args;
    va_start(args, format);
    
    // First format the user message
    char user_msg[2048];
    vsnprintf(user_msg, sizeof(user_msg), format, args);
    va_end(args);
    
    // Then format the complete message with file info
    const char* filename = strrchr(file, '/');
    filename = filename ? filename + 1 : file;
    const char* filename_win = strrchr(filename, '\\');
    filename = filename_win ? filename_win + 1 : filename;
    
    snprintf(buffer, sizeof(buffer), "%s:%s:%d %s", filename, func, line, user_msg);
    
    // Call the callback (releases mutex temporarily)
    g_log_mutex.unlock();
    callback(level, buffer, user_data);
    g_log_mutex.lock();
}

