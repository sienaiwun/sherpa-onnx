
mkdir build_win64
cmake -B build_win64 -DBUILD_SHARED_LIBS=Off -DGGML_CUDA=Off  -DLLAMA_CURL=OFF
cmake --build build_win64 --config RelWithDebInfo
cmake --install build_win64 --prefix install_build_win64 --config RelWithDebInfo
