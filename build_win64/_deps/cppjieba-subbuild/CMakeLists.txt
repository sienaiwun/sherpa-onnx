# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.31.0)

# Reject any attempt to use a toolchain file. We must not use one because
# we could be downloading it here. If the CMAKE_TOOLCHAIN_FILE environment
# variable is set, the cache variable will have been initialized from it.
unset(CMAKE_TOOLCHAIN_FILE CACHE)
unset(ENV{CMAKE_TOOLCHAIN_FILE})

# We name the project and the target for the ExternalProject_Add() call
# to something that will highlight to the user what we are working on if
# something goes wrong and an error message is produced.

project(cppjieba-populate NONE)



include(ExternalProject)
ExternalProject_Add(cppjieba-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "FALSE" "EXTERNALPROJECT_INTERNAL_ARGUMENT_SEPARATOR" "URL" "https://github.com/csukuangfj/cppjieba/archive/refs/tags/sherpa-onnx-2024-04-19.tar.gz" "https://hf-mirror.com/csukuangfj/sherpa-onnx-cmake-deps/resolve/main/cppjieba-sherpa-onnx-2024-04-19.tar.gz" "URL_HASH" "SHA256=03e5264687f0efaef05487a07d49c3f4c0f743347bfbf825df4b30cc75ac5288"
                    SOURCE_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/cppjieba-src"
                    BINARY_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/cppjieba-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


