// sherpa-onnx/csrc/offline-tts-model-config.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include "sherpa-onnx/csrc/offline-tts-model-config.h"

#include "sherpa-onnx/csrc/macros.h"

namespace sherpa_onnx {

void OfflineTtsModelConfig::Register(ParseOptions *po) {
  vits.Register(po);
  matcha.Register(po);
  kokoro.Register(po);
  zipvoice.Register(po);
  kitten.Register(po);

  po->Register("num-threads", &num_threads,
               "Number of threads to run the neural network");

  po->Register("debug", &debug,
               "true to print model information while loading it.");

  po->Register("provider", &provider,
               "Specify a provider to use: cpu, cuda, coreml");
}

bool OfflineTtsModelConfig::Validate() const { return true; }

std::string OfflineTtsModelConfig::ToString() const {
  std::ostringstream os;

  os << "OfflineTtsModelConfig(";
  os << "vits=" << vits.ToString() << ", ";
  os << "matcha=" << matcha.ToString() << ", ";
  os << "kokoro=" << kokoro.ToString() << ", ";
  os << "zipvoice=" << zipvoice.ToString() << ", ";
  os << "kitten=" << kitten.ToString() << ", ";
  os << "num_threads=" << num_threads << ", ";
  os << "debug=" << (debug ? "True" : "False") << ", ";
  os << "provider=\"" << provider << "\")";

  return os.str();
}

}  // namespace sherpa_onnx
