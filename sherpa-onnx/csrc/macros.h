// sherpa-onnx/csrc/macros.h
//
// Copyright      2023  Xiaomi Corporation

#ifndef SHERPA_ONNX_CSRC_MACROS_H_
#define SHERPA_ONNX_CSRC_MACROS_H_
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include <utility>

// Log levels (similar to ggml_log_level in llama.cpp)
enum sherpa_onnx_log_level {
    SHERPA_ONNX_LOG_LEVEL_ERROR = 2,
    SHERPA_ONNX_LOG_LEVEL_WARN  = 3,
    SHERPA_ONNX_LOG_LEVEL_INFO  = 4,
    SHERPA_ONNX_LOG_LEVEL_DEBUG = 5,
};

// Log callback function type
typedef void (*sherpa_onnx_log_callback)(enum sherpa_onnx_log_level level, const char* text, void* user_data);

#ifdef __cplusplus
extern "C" {
#endif

// Function declarations for log callback management
void sherpa_onnx_log_set(sherpa_onnx_log_callback log_callback, void* user_data);
sherpa_onnx_log_callback sherpa_onnx_log_get(void);
void* sherpa_onnx_log_get_user_data(void);

// Internal logging function
void sherpa_onnx_log_internal(enum sherpa_onnx_log_level level, const char* file, const char* func, int line, const char* format, ...);

#ifdef __cplusplus
}
#endif
#if __OHOS__
#include "hilog/log.h"

#undef LOG_DOMAIN
#undef LOG_TAG

// https://gitee.com/openharmony/docs/blob/145a084f0b742e4325915e32f8184817927d1251/en/contribute/OpenHarmony-Log-guide.md#hilog-api-usage-specifications
#define LOG_DOMAIN 0x6666
#define LOG_TAG "sherpa_onnx"
#endif

// Updated logging macros to use the new callback system
#define SHERPA_ONNX_LOGE(...) \
    sherpa_onnx_log_internal(SHERPA_ONNX_LOG_LEVEL_ERROR, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define SHERPA_ONNX_LOGW(...) \
    sherpa_onnx_log_internal(SHERPA_ONNX_LOG_LEVEL_WARN, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define SHERPA_ONNX_LOGI(...) \
    sherpa_onnx_log_internal(SHERPA_ONNX_LOG_LEVEL_INFO, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define SHERPA_ONNX_LOGD(...) \
    sherpa_onnx_log_internal(SHERPA_ONNX_LOG_LEVEL_DEBUG, __FILE__, __func__, __LINE__, ##__VA_ARGS__)

#define SHERPA_ONNX_EXIT(code) exit(code)

// Read an integer
#define SHERPA_ONNX_READ_META_DATA(dst, src_key)                           \
  do {                                                                     \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator); \
    if (value.empty()) {                                                   \
      SHERPA_ONNX_LOGE("'%s' does not exist in the metadata", src_key);    \
      SHERPA_ONNX_EXIT(-1);                                                \
    }                                                                      \
                                                                           \
    dst = atoi(value.c_str());                                             \
    if (dst < 0) {                                                         \
      SHERPA_ONNX_LOGE("Invalid value %d for '%s'", dst, src_key);         \
      SHERPA_ONNX_EXIT(-1);                                                \
    }                                                                      \
  } while (0)

#define SHERPA_ONNX_READ_META_DATA_WITH_DEFAULT(dst, src_key, default_value) \
  do {                                                                       \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator);   \
    if (value.empty()) {                                                     \
      dst = default_value;                                                   \
    } else {                                                                 \
      dst = atoi(value.c_str());                                             \
      if (dst < 0) {                                                         \
        SHERPA_ONNX_LOGE("Invalid value %d for '%s'", dst, src_key);         \
        SHERPA_ONNX_EXIT(-1);                                                \
      }                                                                      \
    }                                                                        \
  } while (0)

// read a vector of integers
#define SHERPA_ONNX_READ_META_DATA_VEC(dst, src_key)                           \
  do {                                                                         \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator);     \
    if (value.empty()) {                                                       \
      SHERPA_ONNX_LOGE("'%s' does not exist in the metadata", src_key);        \
      SHERPA_ONNX_EXIT(-1);                                                    \
    }                                                                          \
                                                                               \
    bool ret = SplitStringToIntegers(value.c_str(), ",", true, &dst);          \
    if (!ret) {                                                                \
      SHERPA_ONNX_LOGE("Invalid value '%s' for '%s'", value.c_str(), src_key); \
      SHERPA_ONNX_EXIT(-1);                                                    \
    }                                                                          \
  } while (0)

// read a vector of floats
#define SHERPA_ONNX_READ_META_DATA_VEC_FLOAT(dst, src_key)                     \
  do {                                                                         \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator);     \
    if (value.empty()) {                                                       \
      SHERPA_ONNX_LOGE("%s does not exist in the metadata", src_key);          \
      SHERPA_ONNX_EXIT(-1);                                                    \
    }                                                                          \
                                                                               \
    bool ret = SplitStringToFloats(value.c_str(), ",", true, &dst);            \
    if (!ret) {                                                                \
      SHERPA_ONNX_LOGE("Invalid value '%s' for '%s'", value.c_str(), src_key); \
      SHERPA_ONNX_EXIT(-1);                                                    \
    }                                                                          \
  } while (0)

// read a vector of strings
#define SHERPA_ONNX_READ_META_DATA_VEC_STRING(dst, src_key)                \
  do {                                                                     \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator); \
    if (value.empty()) {                                                   \
      SHERPA_ONNX_LOGE("'%s' does not exist in the metadata", src_key);    \
      SHERPA_ONNX_EXIT(-1);                                                \
    }                                                                      \
    SplitStringToVector(value.c_str(), ",", false, &dst);                  \
                                                                           \
    if (dst.empty()) {                                                     \
      SHERPA_ONNX_LOGE("Invalid value '%s' for '%s'. Empty vector!",       \
                       value.c_str(), src_key);                            \
      SHERPA_ONNX_EXIT(-1);                                                \
    }                                                                      \
  } while (0)

// read a vector of strings separated by sep
#define SHERPA_ONNX_READ_META_DATA_VEC_STRING_SEP(dst, src_key, sep)       \
  do {                                                                     \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator); \
    if (value.empty()) {                                                   \
      SHERPA_ONNX_LOGE("'%s' does not exist in the metadata", src_key);    \
      SHERPA_ONNX_EXIT(-1);                                                \
    }                                                                      \
    SplitStringToVector(value.c_str(), sep, false, &dst);                  \
                                                                           \
    if (dst.empty()) {                                                     \
      SHERPA_ONNX_LOGE("Invalid value '%s' for '%s'. Empty vector!",       \
                       value.c_str(), src_key);                            \
      SHERPA_ONNX_EXIT(-1);                                                \
    }                                                                      \
  } while (0)

// Read a string
#define SHERPA_ONNX_READ_META_DATA_STR(dst, src_key)                       \
  do {                                                                     \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator); \
    if (value.empty()) {                                                   \
      SHERPA_ONNX_LOGE("'%s' does not exist in the metadata", src_key);    \
      SHERPA_ONNX_EXIT(-1);                                                \
    }                                                                      \
                                                                           \
    dst = std::move(value);                                                \
    if (dst.empty()) {                                                     \
      SHERPA_ONNX_LOGE("Invalid value for '%s'\n", src_key);               \
      SHERPA_ONNX_EXIT(-1);                                                \
    }                                                                      \
  } while (0)

#define SHERPA_ONNX_READ_META_DATA_STR_ALLOW_EMPTY(dst, src_key)           \
  do {                                                                     \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator); \
                                                                           \
    dst = std::move(value);                                                \
  } while (0)

#define SHERPA_ONNX_READ_META_DATA_STR_WITH_DEFAULT(dst, src_key,          \
                                                    default_value)         \
  do {                                                                     \
    auto value = LookupCustomModelMetaData(meta_data, src_key, allocator); \
    if (value.empty()) {                                                   \
      dst = default_value;                                                 \
    } else {                                                               \
      dst = std::move(value);                                              \
      if (dst.empty()) {                                                   \
        SHERPA_ONNX_LOGE("Invalid value for '%s'\n", src_key);             \
        SHERPA_ONNX_EXIT(-1);                                              \
      }                                                                    \
    }                                                                      \
  } while (0)

#endif  // SHERPA_ONNX_CSRC_MACROS_H_
