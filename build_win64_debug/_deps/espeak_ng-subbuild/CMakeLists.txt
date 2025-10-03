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

project(espeak_ng-populate NONE)



include(ExternalProject)
ExternalProject_Add(espeak_ng-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "FALSE" "EXTERNALPROJECT_INTERNAL_ARGUMENT_SEPARATOR" "URL" "https://github.com/csukuangfj/espeak-ng/archive/f6fed6c58b5e0998b8e68c6610125e2d07d595a7.zip" "https://hf-mirror.com/csukuangfj/sherpa-onnx-cmake-deps/resolve/main/espeak-ng-f6fed6c58b5e0998b8e68c6610125e2d07d595a7.zip" "URL_HASH" "SHA256=70cbf4050e7a014aae19140b05e57249da4720f56128459fbe3a93beaf971ae6"
                    SOURCE_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/espeak_ng-src"
                    BINARY_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/espeak_ng-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


