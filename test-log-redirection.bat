@echo off
echo Testing log redirection functionality...
echo.

REM Build the project first
echo Building the project...
cmake --build build --config Release
if errorlevel 1 (
    echo Build failed!
    exit /b 1
)

REM Test 1: Normal run (logs to console only)
echo.
echo Test 1: Running without log redirection...
echo ========================================
build\bin\Release\sherpa-onnx-offline-tts.exe ^
  --vits-model="D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\en_US-amy-low.onnx" ^
  --vits-tokens="D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\tokens.txt" ^
  --vits-data-dir="D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\espeak-ng-data" ^
  --debug=1 ^
  --output-filename="test_no_log.wav" ^
  "Hello, this is a test without log redirection."
echo.

REM Test 2: Run with log redirection
echo Test 2: Running with log redirection...
echo ======================================
build\bin\Release\sherpa-onnx-offline-tts.exe ^
  --vits-model="D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\en_US-amy-low.onnx" ^
  --vits-tokens="D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\tokens.txt" ^
  --vits-data-dir="D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\espeak-ng-data" ^
  --debug=1 ^
  --log-file="sherpa-onnx.log" ^
  --output-filename="test_with_log.wav" ^
  "Hello, this is a test with log redirection."
echo.

REM Check if log file was created
if exist "sherpa-onnx.log" (
    echo SUCCESS: Log file was created!
    echo.
    echo Log file contents:
    echo ==================
    type "sherpa-onnx.log"
    echo.
    echo ==================
) else (
    echo ERROR: Log file was NOT created!
)

echo.
echo Testing completed.
pause