# Espeak Data 打包/解包工具完整指南

## 工具概述

我们提供了两个工具来处理 espeak-ng-data：

1. **espeak_data_packer** - 将 espeak-ng-data 目录打包成单一 .pack 文件
2. **espeak_data_unpacker** - 将 .pack 文件解包回目录结构

## 快速开始

### 1. 构建工具

首先需要构建这两个工具。它们已经集成到 sherpa-onnx 主项目中：

```bash
# 构建 Release 版本（推荐）
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build . --config Release

# 或者构建 Debug 版本
mkdir build-debug && cd build-debug  
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build . --config Debug
```

构建完成后，工具位于：
- `build/bin/Release/espeak_data_packer.exe`
- `build/bin/Release/espeak_data_unpacker.exe`

### 2. 打包操作

将你的 espeak-ng-data 目录打包成单一文件：

```bash
# 基本语法
espeak_data_packer.exe <input-directory> <output-pack-file>

# 针对你的具体情况
build\bin\Release\espeak_data_packer.exe "D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\espeak-ng-data" "espeak_data.pack"
```

### 3. 解包操作

将 .pack 文件解包回目录结构：

```bash
# 基本语法  
espeak_data_unpacker.exe <input-pack-file> <output-directory>

# 示例
build\bin\Release\espeak_data_unpacker.exe "espeak_data.pack" "extracted-espeak-data"
```

## 详细使用说明

### 打包工具 (espeak_data_packer)

**功能：** 将整个 espeak-ng-data 目录递归打包成单一的二进制文件。

**参数：**
- `<input-directory>`: 输入的 espeak-ng-data 目录路径
- `<output-pack-file>`: 输出的 .pack 文件路径

**示例：**
```bash
# 示例1：使用你的路径
espeak_data_packer.exe "D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\espeak-ng-data" "my_espeak_data.pack"

# 示例2：相对路径
espeak_data_packer.exe "./espeak-ng-data" "./resources/espeak_data.pack"

# 示例3：打包到指定位置
espeak_data_packer.exe "C:\espeak-ng-data" "D:\MyProject\Assets\espeak_data.pack"
```

**输出示例：**
```
Espeak-NG Data Packer Tool v1.0
Copyright (c) 2024 - Pack espeak-ng-data into single file

=== Packing Summary ===
Input directory: D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\espeak-ng-data
Output pack file: espeak_data.pack
Files to pack: 156
Total size: 2048576 bytes (2.0 MB)

Start packing? (Y/n): y
Packing espeak-ng-data...
This may take a few moments...

✅ Packing completed successfully!
Time taken: 234 ms
Output file size: 2056789 bytes (2.0 MB)
Pack file created: espeak_data.pack
```

### 解包工具 (espeak_data_unpacker)

**功能：** 将 .pack 文件解包回原始的目录结构。

**参数：**
- `<input-pack-file>`: 输入的 .pack 文件路径
- `<output-directory>`: 输出目录路径（会自动创建）

**示例：**
```bash
# 示例1：解包到新目录
espeak_data_unpacker.exe "espeak_data.pack" "extracted-data"

# 示例2：解包到指定路径  
espeak_data_unpacker.exe "C:\data\espeak_data.pack" "C:\temp\espeak-ng-data"

# 示例3：验证打包结果
espeak_data_unpacker.exe "espeak_data.pack" "verify-extraction"
```

**输出示例：**
```
Espeak-NG Data Unpacker Tool v1.0
Copyright (c) 2024 - Unpack espeak_data.pack to individual files

Pack file size: 2056789 bytes (2.0 MB)

=== Unpacking Summary ===
Input pack file: espeak_data.pack
Output directory: extracted-data

Start unpacking? (Y/n): y
Loading pack file...
Pack file loaded successfully!
Found 156 files in the pack.

Extracting files...
  [1/156] voices/en/en-us
  [2/156] voices/en/en-gb
  ...
  [156/156] phondata

✅ Unpacking completed successfully!
Time taken: 187 ms
Files extracted: 156/156

Sample extracted files:
  voices/en/en-us (234 bytes)
  voices/zh/zh (567 bytes)
  phondata (45678 bytes)
  phonindex (1234 bytes)
  phontab (8901 bytes)
```

## 快捷脚本

为了方便使用，我们提供了几个批处理脚本：

### pack_your_espeak_data.bat
```bash
@echo off
echo Packing your espeak-ng-data...
build\bin\Release\espeak_data_packer.exe "D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\espeak-ng-data" "espeak_data.pack"
pause
```

### unpack_espeak_data.bat  
```bash
@echo off
echo Unpacking espeak_data.pack...
build\bin\Release\espeak_data_unpacker.exe "espeak_data.pack" "extracted-espeak-data"
pause
```

## 在代码中使用

打包完成后，你可以在代码中使用内存文件系统：

```cpp
#include "EspeakDataPacker.h"

// 方法1：从 .pack 文件加载
sherpa_onnx::EspeakResourcePack pack;
if (sherpa_onnx::EspeakDataPacker::LoadPack("espeak_data.pack", pack)) {
    // 创建内存文件系统
    sherpa_onnx::EspeakMemoryFS memory_fs;
    memory_fs.LoadFromPack(pack);
    
    // 使用文件
    std::vector<char> voice_data;
    if (memory_fs.GetFile("voices/en/en-us", voice_data)) {
        // 使用语音数据...
    }
}

// 方法2：从 UE TMap 加载（如果你已经有数据在 UE 中）
std::map<std::string, std::vector<uint8_t>> ue_data;
// ... 填充数据 ...

sherpa_onnx::EspeakMemoryFS memory_fs;
memory_fs.LoadFromUEMap(ue_data);
```

## 文件格式

`.pack` 文件使用自定义的二进制格式：

```
文件结构：
┌─────────────────┬─────────────────┐
│ Header (8 bytes)│ "ESPKDATA"      │
├─────────────────┼─────────────────┤
│ Count (4 bytes) │ 文件数量        │
├─────────────────┼─────────────────┤
│ File Table      │ 每个文件的：     │
│                 │ - 路径长度(4字节)│
│                 │ - 路径字符串     │
│                 │ - 数据偏移(8字节)│
│                 │ - 数据大小(8字节)│
├─────────────────┼─────────────────┤
│ Data Section    │ 所有文件数据     │
└─────────────────┴─────────────────┘
```

## 故障排除

### 常见错误

1. **"espeak_data_packer.exe not found"**
   - 确保已经构建了项目
   - 检查路径是否正确

2. **"Directory does not exist"**
   - 检查输入目录路径是否正确
   - 确保目录存在且可访问

3. **"Pack file is too small to be valid"**
   - 检查 .pack 文件是否完整
   - 重新生成 .pack 文件

4. **"Failed to create output directory"**
   - 检查输出目录权限
   - 确保父目录存在

### 性能提示

- 打包/解包速度取决于文件数量和大小
- 建议在 SSD 上操作以获得更好的性能
- 大文件可能需要更多内存

### 兼容性

- ✅ Windows (Visual Studio 2019+)
- ✅ Linux (GCC 7+)  
- ✅ macOS (Clang 10+)
- ✅ 支持 Unicode 文件名
- ✅ 支持任意大小的文件

这样你就可以完全将 espeak-ng-data 内嵌到你的应用程序中了！