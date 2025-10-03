# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/espeak_ng-src")
  file(MAKE_DIRECTORY "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/espeak_ng-src")
endif()
file(MAKE_DIRECTORY
  "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/espeak_ng-build"
  "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/espeak_ng-subbuild/espeak_ng-populate-prefix"
  "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/espeak_ng-subbuild/espeak_ng-populate-prefix/tmp"
  "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/espeak_ng-subbuild/espeak_ng-populate-prefix/src/espeak_ng-populate-stamp"
  "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/espeak_ng-subbuild/espeak_ng-populate-prefix/src"
  "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/espeak_ng-subbuild/espeak_ng-populate-prefix/src/espeak_ng-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/espeak_ng-subbuild/espeak_ng-populate-prefix/src/espeak_ng-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/espeak_ng-subbuild/espeak_ng-populate-prefix/src/espeak_ng-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
