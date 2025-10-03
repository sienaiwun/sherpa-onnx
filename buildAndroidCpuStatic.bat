@echo off
setlocal enabledelayedexpansion

echo Building sherpa-onnx for Android arm64-v8a

:: 清除可能冲突的环境变量
set ANDROID_HOME=
set ANDROID_SDK_ROOT=
set NDK_ROOT=
set JAVA_HOME=

:: 设置正确的Android环境
set ANDROID_NDK=C:\Users\NaiwenXie\AppData\Local\Android\Sdk\ndk\25.1.8937393
set ANDROID_SDK_ROOT=C:\Users\NaiwenXie\AppData\Local\Android\Sdk
set ANDROID_HOME=%ANDROID_SDK_ROOT%

:: 设置默认参数
if not defined BUILD_SHARED_LIBS set BUILD_SHARED_LIBS=ON
if not defined SHERPA_ONNX_ENABLE_TTS set SHERPA_ONNX_ENABLE_TTS=ON
if not defined SHERPA_ONNX_ENABLE_SPEAKER_DIARIZATION set SHERPA_ONNX_ENABLE_SPEAKER_DIARIZATION=ON
if not defined SHERPA_ONNX_ENABLE_BINARY set SHERPA_ONNX_ENABLE_BINARY=OFF
if not defined SHERPA_ONNX_ENABLE_C_API set SHERPA_ONNX_ENABLE_C_API=OFF
if not defined SHERPA_ONNX_ENABLE_JNI set SHERPA_ONNX_ENABLE_JNI=ON
if not defined SHERPA_ONNX_ENABLE_RKNN set SHERPA_ONNX_ENABLE_RKNN=OFF

echo BUILD_SHARED_LIBS: %BUILD_SHARED_LIBS%

:: 设置构建目录
if "%BUILD_SHARED_LIBS%"=="ON" (
    set BUILD_DIR=build-android-arm64-v8a
) else (
    set BUILD_DIR=build-android-arm64-v8a-static
)

echo Build directory: %BUILD_DIR%

:: 强制清理构建目录
echo 清理构建目录...
taskkill /f /im cmake.exe >nul 2>&1
taskkill /f /im MSBuild.exe >nul 2>&1
timeout /t 2 >nul

if exist %BUILD_DIR% (
    echo 删除现有构建目录...
    rmdir /s /q %BUILD_DIR% 2>nul
    if exist %BUILD_DIR% (
        echo 警告: 无法完全清理构建目录，某些文件可能被占用
        echo 尝试重命名旧目录...
        ren %BUILD_DIR% %BUILD_DIR%_backup_%RANDOM% >nul 2>&1
    )
)

mkdir %BUILD_DIR%
cd %BUILD_DIR%

if not exist "%ANDROID_NDK%" (
    echo 错误: ANDROID_NDK 目录不存在: %ANDROID_NDK%
    echo 请检查Android NDK是否正确安装
    pause
    exit /b 1
)

echo ANDROID_NDK: %ANDROID_NDK%
echo ANDROID_SDK_ROOT: %ANDROID_SDK_ROOT%

:: 设置ONNX Runtime版本
set ONNXRUNTIME_VERSION=1.17.1

:: 下载ONNX Runtime
if "%BUILD_SHARED_LIBS%"=="ON" (
    if not exist "%ONNXRUNTIME_VERSION%\jni\arm64-v8a\libonnxruntime.so" (
        echo Downloading ONNX Runtime for Android...
        mkdir %ONNXRUNTIME_VERSION% 2>nul
        cd %ONNXRUNTIME_VERSION%
        
        :: 使用PowerShell下载文件
        powershell -Command "Invoke-WebRequest -Uri 'https://github.com/csukuangfj/onnxruntime-libs/releases/download/v%ONNXRUNTIME_VERSION%/onnxruntime-android-%ONNXRUNTIME_VERSION%.zip' -OutFile 'onnxruntime-android-%ONNXRUNTIME_VERSION%.zip'"
        
        :: 解压文件
        powershell -Command "Expand-Archive -Path 'onnxruntime-android-%ONNXRUNTIME_VERSION%.zip' -DestinationPath '.'"
        del onnxruntime-android-%ONNXRUNTIME_VERSION%.zip
        cd ..
    )
    set SHERPA_ONNXRUNTIME_LIB_DIR=%CD%\%ONNXRUNTIME_VERSION%\jni\arm64-v8a
    set SHERPA_ONNXRUNTIME_INCLUDE_DIR=%CD%\%ONNXRUNTIME_VERSION%\headers
) else (
    if not exist "%ONNXRUNTIME_VERSION%-static\lib\libonnxruntime.a" (
        echo Downloading ONNX Runtime static lib for Android...
        
        :: 使用PowerShell下载静态库
        powershell -Command "Invoke-WebRequest -Uri 'https://github.com/csukuangfj/onnxruntime-libs/releases/download/v%ONNXRUNTIME_VERSION%/onnxruntime-android-arm64-v8a-static_lib-%ONNXRUNTIME_VERSION%.zip' -OutFile 'onnxruntime-android-arm64-v8a-static_lib-%ONNXRUNTIME_VERSION%.zip'"
        
        :: 解压文件
        powershell -Command "Expand-Archive -Path 'onnxruntime-android-arm64-v8a-static_lib-%ONNXRUNTIME_VERSION%.zip' -DestinationPath '.'"
        del onnxruntime-android-arm64-v8a-static_lib-%ONNXRUNTIME_VERSION%.zip
        ren onnxruntime-android-arm64-v8a-static_lib-%ONNXRUNTIME_VERSION% %ONNXRUNTIME_VERSION%-static
    )
    set SHERPA_ONNXRUNTIME_LIB_DIR=%CD%\%ONNXRUNTIME_VERSION%-static\lib
    set SHERPA_ONNXRUNTIME_INCLUDE_DIR=%CD%\%ONNXRUNTIME_VERSION%-static\include
)

echo SHERPA_ONNXRUNTIME_LIB_DIR: %SHERPA_ONNXRUNTIME_LIB_DIR%
echo SHERPA_ONNXRUNTIME_INCLUDE_DIR: %SHERPA_ONNXRUNTIME_INCLUDE_DIR%

:: CMake配置
echo Configuring CMake...
cmake -G "Unix Makefiles" ^
    -DCMAKE_TOOLCHAIN_FILE="%ANDROID_NDK%\build\cmake\android.toolchain.cmake" ^
    -DCMAKE_MAKE_PROGRAM="%ANDROID_NDK%\prebuilt\windows-x86_64\bin\make.exe" ^
    -DSHERPA_ONNX_ENABLE_TTS=%SHERPA_ONNX_ENABLE_TTS% ^
    -DSHERPA_ONNX_ENABLE_SPEAKER_DIARIZATION=%SHERPA_ONNX_ENABLE_SPEAKER_DIARIZATION% ^
    -DSHERPA_ONNX_ENABLE_BINARY=%SHERPA_ONNX_ENABLE_BINARY% ^
    -DBUILD_PIPER_PHONMIZE_EXE=OFF ^
    -DBUILD_PIPER_PHONMIZE_TESTS=OFF ^
    -DBUILD_ESPEAK_NG_EXE=OFF ^
    -DBUILD_ESPEAK_NG_TESTS=OFF ^
    -DCMAKE_BUILD_TYPE=Release ^
    -DBUILD_SHARED_LIBS=%BUILD_SHARED_LIBS% ^
    -DSHERPA_ONNX_ENABLE_PYTHON=OFF ^
    -DSHERPA_ONNX_ENABLE_TESTS=OFF ^
    -DSHERPA_ONNX_ENABLE_CHECK=OFF ^
    -DSHERPA_ONNX_ENABLE_PORTAUDIO=OFF ^
    -DSHERPA_ONNX_ENABLE_JNI=%SHERPA_ONNX_ENABLE_JNI% ^
    -DSHERPA_ONNX_LINK_LIBSTDCPP_STATICALLY=OFF ^
    -DSHERPA_ONNX_ENABLE_C_API=%SHERPA_ONNX_ENABLE_C_API% ^
    -DCMAKE_INSTALL_PREFIX=./install ^
    -DSHERPA_ONNX_ENABLE_RKNN=%SHERPA_ONNX_ENABLE_RKNN% ^
    -DANDROID_ABI=arm64-v8a ^
    -DANDROID_PLATFORM=android-21 ^
    -DANDROID_NDK="%ANDROID_NDK%" ^
    ..

if %ERRORLEVEL% neq 0 (
    echo CMake configuration failed!
    pause
    exit /b %ERRORLEVEL%
)

:: 构建
echo Building...
cmake --build . --config Release --parallel

if %ERRORLEVEL% neq 0 (
    echo Build failed!
    pause
    exit /b %ERRORLEVEL%
)

:: 安装
echo Installing...
cmake --build . --target install --config Release

if %ERRORLEVEL% neq 0 (
    echo Install failed!
    pause
    exit /b %ERRORLEVEL%
)

:: 复制ONNX Runtime库文件
if "%BUILD_SHARED_LIBS%"=="ON" (
    if exist "%ONNXRUNTIME_VERSION%\jni\arm64-v8a\libonnxruntime.so" (
        copy "%ONNXRUNTIME_VERSION%\jni\arm64-v8a\libonnxruntime.so" install\lib\ >nul 2>&1
    )
)

:: 清理不需要的文件
if exist install\share rmdir /s /q install\share
if exist install\lib\pkgconfig rmdir /s /q install\lib\pkgconfig
del install\lib\lib*.a >nul 2>&1

:: 创建README文件
if exist install\lib\libsherpa-onnx-c-api.so (
    echo # Introduction > install\lib\README.md
    echo. >> install\lib\README.md
    echo Note that if you use Android Studio, then you only need to >> install\lib\README.md
    echo copy libonnxruntime.so and libsherpa-onnx-jni.so >> install\lib\README.md
    echo to your jniLibs, and you don't need libsherpa-onnx-c-api.so or >> install\lib\README.md
    echo libsherpa-onnx-cxx-api.so. >> install\lib\README.md
    echo. >> install\lib\README.md
    echo In any case, libonnxruntime.so is always needed. >> install\lib\README.md
)

echo.
echo ========================================
echo Android arm64-v8a build completed!
echo ========================================
echo Output directory: %BUILD_DIR%\install
echo.
echo Library files in install\lib:
dir /b install\lib\*.so 2>nul

cd ..
echo Build completed successfully!