#ifndef SHERPA_ONNX_CSRC_ESPEAK_DATA_PACKER_H_
#define SHERPA_ONNX_CSRC_ESPEAK_DATA_PACKER_H_

#include <cstddef>
#include <string>

namespace sherpa_onnx {

/**
 * Extract packed espeak data to a temporary directory.
 * 
 * @param pack_data Pointer to the packed espeak data
 * @param pack_data_size Size of the packed data
 * @return Path to the temporary directory containing extracted data, empty string if failed
 */
std::string ExtractEspeakDataToTemp(const void* pack_data, size_t pack_data_size);

/**
 * Initialize espeak-ng from packed data in memory.
 * 
 * @param pack_data Pointer to the packed espeak data
 * @param pack_data_size Size of the packed data
 * @return true if initialization successful, false otherwise
 */
bool InitEspeakNgFromMemory(const void* pack_data, size_t pack_data_size);

/**
 * Cleanup any temporary files created during espeak initialization.
 */
void CleanupEspeakTempData();

}  // namespace sherpa_onnx

#endif  // SHERPA_ONNX_CSRC_ESPEAK_DATA_PACKER_H_