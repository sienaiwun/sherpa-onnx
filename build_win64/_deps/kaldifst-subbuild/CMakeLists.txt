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

project(kaldifst-populate NONE)



include(ExternalProject)
ExternalProject_Add(kaldifst-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "FALSE" "EXTERNALPROJECT_INTERNAL_ARGUMENT_SEPARATOR" "URL" "https://github.com/k2-fsa/kaldifst/archive/refs/tags/v1.7.17.tar.gz" "URL_HASH" "SHA256=c4b701a23a400bda8032586b02c7e0d5e813a765832df60c23e6df9e62b010f4"
                    SOURCE_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/kaldifst-src"
                    BINARY_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/kaldifst-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


