# Sherpa-ONNX 日志重定向功能

## 功能说明

为了解决在 UE (Unreal Engine) 中使用 sherpa-onnx 库时难以查看日志的问题，我们为库添加了日志重定向功能。现在你可以将所有 sherpa-onnx 的内部日志输出到指定的文件中。

## 新增的 C API

### SherpaOnnxSetLogFile
```c
void SherpaOnnxSetLogFile(const char *filename);
```
- 功能：将日志重定向到指定文件（UTF-8 编码路径）
- 参数：`filename` - 日志文件的完整路径（UTF-8 编码）
- 说明：支持跨平台使用，推荐使用此接口

### SherpaOnnxSetLogFileW (仅 Windows)
```c
void SherpaOnnxSetLogFileW(const wchar_t *filename);
```
- 功能：将日志重定向到指定文件（宽字符路径）
- 参数：`filename` - 日志文件的完整路径（宽字符编码）
- 说明：仅在 Windows 平台可用，方便处理包含非 ASCII 字符的路径

### SherpaOnnxFlushLog
```c
void SherpaOnnxFlushLog(void);
```
- 功能：手动刷新日志缓冲区，确保日志立即写入文件
- 说明：在需要立即查看日志内容时调用

## 在 UE 中的使用方法

### 1. 基本使用
在你的 UE 模块的 `StartupModule()` 函数中，或者在首次使用 sherpa-onnx 之前调用：

```cpp
// UTF-8 版本（推荐）
SherpaOnnxSetLogFile("C:/YourProject/Saved/Logs/sherpa_onnx.log");

// 或者 Windows 宽字符版本
SherpaOnnxSetLogFileW(L"C:\\YourProject\\Saved\\Logs\\sherpa_onnx.log");
```

### 2. 推荐的实践

```cpp
void FYourModule::StartupModule()
{
    // 获取 UE 日志目录
    FString LogDir = FPaths::ProjectLogDir();
    FString LogPath = FPaths::Combine(LogDir, TEXT("sherpa_onnx.log"));
    
    // 转换为 UTF-8 字符串
    std::string LogPathUtf8 = TCHAR_TO_UTF8(*LogPath);
    
    // 设置 sherpa-onnx 日志文件
    SherpaOnnxSetLogFile(LogPathUtf8.c_str());
    
    // 现在可以安全地使用 sherpa-onnx
    // ... 初始化 sherpa-onnx 相关代码
}

void FYourModule::ShutdownModule()
{
    // 在模块关闭前刷新日志
    SherpaOnnxFlushLog();
}
```

### 3. 运行时日志管理

```cpp
// 在需要立即查看日志时调用
void FlushLogs()
{
    SherpaOnnxFlushLog();
}

// 在运行时切换日志文件
void SwitchLogFile(const FString& NewLogPath)
{
    std::string NewLogPathUtf8 = TCHAR_TO_UTF8(*NewLogPath);
    SherpaOnnxSetLogFile(NewLogPathUtf8.c_str());
}
```

## 日志行为说明

- **默认行为**：没有设置日志文件时，日志输出到 stderr
- **设置文件后**：日志会同时写入到指定文件和 stderr（方便本地调试）
- **文件模式**：以追加模式打开日志文件，不会覆盖已有内容
- **编码格式**：日志文件使用 UTF-8 编码
- **缓冲模式**：使用行缓冲，确保实时写入

## 注意事项

1. **调用时机**：必须在使用任何 sherpa-onnx 功能之前设置日志文件
2. **路径权限**：确保指定的路径可写，建议使用 UE 的 `ProjectLogDir()`
3. **文件管理**：日志文件会不断增长，建议定期清理或轮转
4. **线程安全**：日志写入是线程安全的
5. **平台兼容**：支持 Windows、Linux、macOS 等平台

## 编译要求

确保在编译 sherpa-onnx 时包含了新增的源文件：
- `sherpa-onnx/csrc/log-sink.h`
- `sherpa-onnx/csrc/log-sink.cc`

这些文件已经被添加到 CMakeLists.txt 中，正常编译即可。

## 故障排除

如果日志没有写入文件，请检查：
1. 文件路径是否正确且可写
2. 是否在使用 sherpa-onnx 之前调用了设置函数
3. 磁盘空间是否充足
4. 在 UE 的打包版本中路径是否仍然有效