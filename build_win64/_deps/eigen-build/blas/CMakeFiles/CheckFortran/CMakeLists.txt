cmake_minimum_required(VERSION 3.31.0)
set(CMAKE_MODULE_PATH "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/cmake")
project(CheckFortran Fortran)
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/result.cmake"
  "set(CMAKE_Fortran_COMPILER \"${CMAKE_Fortran_COMPILER}\")\n"
  
  )