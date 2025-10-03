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

project(websocketpp-populate NONE)



include(ExternalProject)
ExternalProject_Add(websocketpp-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "FALSE" "EXTERNALPROJECT_INTERNAL_ARGUMENT_SEPARATOR" "URL" "https://github.com/zaphoyd/websocketpp/archive/b9aeec6eaf3d5610503439b4fae3581d9aff08e8.zip" "https://hf-mirror.com/csukuangfj/sherpa-onnx-cmake-deps/resolve/main/websocketpp-b9aeec6eaf3d5610503439b4fae3581d9aff08e8.zip" "URL_HASH" "SHA256=1385135ede8191a7fbef9ec8099e3c5a673d48df0c143958216cd1690567f583"
                    SOURCE_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/websocketpp-src"
                    BINARY_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/websocketpp-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


