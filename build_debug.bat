@echo off
REM build_debug.bat - 生成 Debug 模式的 Visual Studio 解决方案

echo ================================================
echo Building Debug Visual Studio Solution
echo ================================================

REM 创建 Debug 构建目录
if not exist build-debug mkdir build-debug
cd build-debug

echo.
echo [1/2] Configuring CMake for Debug...
cmake .. -DCMAKE_BUILD_TYPE=Debug

if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo [2/2] Debug Solution generated successfully!
echo.

REM 检查解决方案文件是否存在
if exist "sherpa-onnx.sln" (
    echo ✅ Debug solution file created: build-debug\sherpa-onnx.sln
    echo.
    echo The solution now includes:
    echo   - All original sherpa-onnx projects
    echo   - espeak_data_packer project 
    echo.
    echo You can now:
    echo   1. Open build-debug\sherpa-onnx.sln in Visual Studio
    echo   2. Build the espeak_data_packer project
    echo   3. Find the executable in Debug folder
    echo.
    echo Do you want to open the solution now? (y/N):
    set /p open_solution=
    if /i "%open_solution%"=="y" (
        start sherpa-onnx.sln
    )
) else (
    echo ❌ Could not find sherpa-onnx.sln
    echo Please check the CMake output for errors
)

echo.
echo ================================================
echo Debug solution generation completed
echo ================================================
pause