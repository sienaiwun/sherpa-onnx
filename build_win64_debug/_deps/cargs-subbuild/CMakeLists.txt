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

project(cargs-populate NONE)



include(ExternalProject)
ExternalProject_Add(cargs-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "FALSE" "EXTERNALPROJECT_INTERNAL_ARGUMENT_SEPARATOR" "URL" "https://github.com/likle/cargs/archive/refs/tags/v1.0.3.tar.gz" "https://hf-mirror.com/csukuangfj/sherpa-onnx-cmake-deps/resolve/main/cargs-1.0.3.tar.gz" "URL_HASH" "SHA256=ddba25bd35e9c6c75bc706c126001b8ce8e084d40ef37050e6aa6963e836eb8b"
                    SOURCE_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/cargs-src"
                    BINARY_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/cargs-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


