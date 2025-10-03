# Install script for directory: D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/sherpa-onnx")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE FILE FILES
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/AdolcForward"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/AlignedVector3"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/ArpackSupport"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/AutoDiff"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/BVH"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/EulerAngles"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/FFT"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/IterativeSolvers"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/KroneckerProduct"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/LevenbergMarquardt"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/MatrixFunctions"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/MoreVectorization"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/MPRealSupport"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/NonLinearOptimization"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/NumericalDiff"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/OpenGLSupport"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/Polynomials"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/Skyline"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/SparseExtra"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/SpecialFunctions"
    "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/Splines"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Devel" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/eigen3/unsupported/Eigen" TYPE DIRECTORY FILES "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-src/unsupported/Eigen/src" FILES_MATCHING REGEX "/[^/]*\\.h$")
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-build/unsupported/Eigen/CXX11/cmake_install.cmake")

endif()

string(REPLACE ";" "\n" CMAKE_INSTALL_MANIFEST_CONTENT
       "${CMAKE_INSTALL_MANIFEST_FILES}")
if(CMAKE_INSTALL_LOCAL_ONLY)
  file(WRITE "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/eigen-build/unsupported/Eigen/install_local_manifest.txt"
     "${CMAKE_INSTALL_MANIFEST_CONTENT}")
endif()
