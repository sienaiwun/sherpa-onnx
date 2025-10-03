# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION ${CMAKE_VERSION}) # this file comes with cmake

# If CMAKE_DISABLE_SOURCE_CHANGES is set to true and the source directory is an
# existing directory in our source tree, calling file(MAKE_DIRECTORY) on it
# would cause a fatal error, even though it would be a no-op.
if(NOT EXISTS "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/kaldi_native_fbank-src")
  file(MAKE_DIRECTORY "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/kaldi_native_fbank-src")
endif()
file(MAKE_DIRECTORY
  "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/kaldi_native_fbank-build"
  "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix"
  "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix/tmp"
  "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix/src/kaldi_native_fbank-populate-stamp"
  "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix/src"
  "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix/src/kaldi_native_fbank-populate-stamp"
)

set(configSubDirs Debug)
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix/src/kaldi_native_fbank-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "D:/Sound/sienaiwun/sherpa-onnx/build_win64_debug/_deps/kaldi_native_fbank-subbuild/kaldi_native_fbank-populate-prefix/src/kaldi_native_fbank-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
