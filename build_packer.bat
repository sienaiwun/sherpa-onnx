@echo off
REM build_packer.bat - 构建 espeak data packer 工具

echo ================================================
echo Building Espeak Data Packer Tool
echo ================================================

REM 创建构建目录
if not exist build mkdir build
cd build

echo.
echo [1/3] Configuring CMake...
cmake .. -DCMAKE_BUILD_TYPE=Release

if %errorlevel% neq 0 (
    echo ERROR: CMake configuration failed!
    pause
    exit /b 1
)

echo.
echo [2/3] Building project...
cmake --build . --config Release

if %errorlevel% neq 0 (
    echo ERROR: Build failed!
    pause
    exit /b 1
)

echo.
echo [3/3] Build completed successfully!
echo.

REM 检查可执行文件是否存在
if exist "bin\espeak_data_packer.exe" (
    echo ✅ Executable created: bin\espeak_data_packer.exe
    echo.
    echo Usage:
    echo   bin\espeak_data_packer.exe ^<espeak-ng-data-dir^> ^<output-pack-file^>
    echo.
    echo Example:
    echo   bin\espeak_data_packer.exe C:\espeak-ng-data espeak_data.pack
) else if exist "Release\espeak_data_packer.exe" (
    echo ✅ Executable created: Release\espeak_data_packer.exe
    echo.
    echo Usage:
    echo   Release\espeak_data_packer.exe ^<espeak-ng-data-dir^> ^<output-pack-file^>
    echo.
    echo Example:
    echo   Release\espeak_data_packer.exe C:\espeak-ng-data espeak_data.pack
) else (
    echo ❌ Could not find the executable file
    echo Please check the build output for errors
)

echo.
echo ================================================
echo Build process completed
echo ================================================
pause