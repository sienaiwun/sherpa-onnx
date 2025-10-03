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

project(piper_phonemize-populate NONE)



include(ExternalProject)
ExternalProject_Add(piper_phonemize-populate
                     "UPDATE_DISCONNECTED" "False" "DOWNLOAD_EXTRACT_TIMESTAMP" "FALSE" "EXTERNALPROJECT_INTERNAL_ARGUMENT_SEPARATOR" "URL" "https://github.com/csukuangfj/piper-phonemize/archive/78a788e0b719013401572d70fef372e77bff8e43.zip" "https://hf-mirror.com/csukuangfj/sherpa-onnx-cmake-deps/resolve/main/piper-phonemize-78a788e0b719013401572d70fef372e77bff8e43.zip" "URL_HASH" "SHA256=89641a46489a4898754643ce57bda9c9b54b4ca46485fdc02bf0dc84b866645d"
                    SOURCE_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/piper_phonemize-src"
                    BINARY_DIR          "D:/Sound/sienaiwun/sherpa-onnx/build_win64/_deps/piper_phonemize-build"
                    CONFIGURE_COMMAND   ""
                    BUILD_COMMAND       ""
                    INSTALL_COMMAND     ""
                    TEST_COMMAND        ""
                    USES_TERMINAL_DOWNLOAD  YES
                    USES_TERMINAL_UPDATE    YES
                    USES_TERMINAL_PATCH     YES
)


