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

project(cppinyin-populate NONE)



include(ExternalProject)
ExternalProject_Add(cppinyin-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "FALSE" "EXTERNALPROJECT_INTERNAL_ARGUMENT_SEPARATOR" "URL" "https://github.com/pkufool/cppinyin/archive/refs/tags/v0.10.tar.gz" "https://gh-proxy.com/https://github.com/pkufool/cppinyin/archive/refs/tags/v0.10.tar.gz" "URL_HASH" "SHA256=abe6584d7ee56829e8f4b5fbda3b50ecdf49a13be8e413a78d1b0d5d5c019982"
                    SOURCE_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/cppinyin-src"
                    BINARY_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/cppinyin-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


