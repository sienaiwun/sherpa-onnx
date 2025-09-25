// sherpa-onnx/csrc/sherpa-onnx-offline-tts.cc
//
// Copyright (c)  2023  Xiaomi Corporation

#include <chrono>  // NOLINT
#include <fstream>
#include <iomanip>
#include <memory>
#include <mutex>

#include "sherpa-onnx/csrc/file-utils.h"
#include "sherpa-onnx/csrc/offline-tts.h"
#include "sherpa-onnx/csrc/parse-options.h"
#include "sherpa-onnx/csrc/wave-writer.h"
#include "sherpa-onnx/csrc/macros.h"

// Global log file stream
static std::unique_ptr<std::ofstream> g_log_file;
static std::mutex g_log_mutex;

// Log callback function that writes to file
void FileLogCallback(SherpaOnnxLogLevel level, const char* text, void* user_data) {
  std::lock_guard<std::mutex> lock(g_log_mutex);
  
  if (!g_log_file || !g_log_file->is_open()) {
    return;
  }
  
  const char* level_name;
  switch (level) {
    case SHERPA_ONNX_LOG_LEVEL_ERROR:
      level_name = "ERROR";
      break;
    case SHERPA_ONNX_LOG_LEVEL_WARN:
      level_name = "WARN";
      break;
    case SHERPA_ONNX_LOG_LEVEL_INFO:
      level_name = "INFO";
      break;
    case SHERPA_ONNX_LOG_LEVEL_DEBUG:
      level_name = "DEBUG";
      break;
    default:
      level_name = "UNKNOWN";
  }
  
  // Get current time
  auto now = std::chrono::system_clock::now();
  auto time_t = std::chrono::system_clock::to_time_t(now);
  auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()) % 1000;
  
  char time_buffer[100];
  std::strftime(time_buffer, sizeof(time_buffer), "%Y-%m-%d %H:%M:%S", std::localtime(&time_t));
  
  *g_log_file << time_buffer << "." << std::setfill('0') << std::setw(3) << ms.count() 
              << " [" << level_name << "] " << text << std::endl;
  g_log_file->flush();
}

static int32_t AudioCallback(const float * /*samples*/, int32_t n,
                             float progress) {
  printf("sample=%d, progress=%f\n", n, progress);
  return 1;
}

int main(int32_t argc, char *argv[]) {
  const char *kUsageMessage = R"usage(
Offline/Non-streaming text-to-speech with sherpa-onnx

Usage example:

wget https://github.com/k2-fsa/sherpa-onnx/releases/download/tts-models/vits-piper-en_US-amy-low.tar.bz2
tar xf vits-piper-en_US-amy-low.tar.bz2

./bin/sherpa-onnx-offline-tts \
 --vits-model=./vits-piper-en_US-amy-low/en_US-amy-low.onnx \
 --vits-tokens=./vits-piper-en_US-amy-low/tokens.txt \
 --vits-data-dir=./vits-piper-en_US-amy-low/espeak-ng-data \
 --output-filename=./generated.wav \
 --log-file=./tts.log \
 "Today as always, men fall into two groups: slaves and free men. Whoever does not have two-thirds of his day for himself, is a slave, whatever he may be: a statesman, a businessman, an official, or a scholar."
“--vits-model="D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\en_US-amy-low.onnx" --vits-tokens="D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\tokens.txt" --vits-pack-data="D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\espeak-ng-data.pack" --debug=1 --output-filename="D:\Sound\sherpa-onnx\generated_with_pack.wav" "How are you?"”
“--vits-model="D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\en_US-amy-low.onnx" --vits-tokens="D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\tokens.txt" --vits-data-dir="D:\Sound\sherpa-onnx\vits-piper-en_US-amy-low\espeak-ng-data" --debug=1 --output-filename="D:\Sound\sherpa-onnx\generated_with_pack.wav" "How are you?"”

It will generate a file ./generated.wav as specified by --output-filename.
Log messages will be saved to ./tts.log if --log-file is specified.

You can find more models at
https://github.com/k2-fsa/sherpa-onnx/releases/tag/tts-models

Please see
https://k2-fsa.github.io/sherpa/onnx/tts/index.html
or details.
)usage";

  sherpa_onnx::ParseOptions po(kUsageMessage);
  std::string output_filename = "./generated.wav";
  std::string log_filename = "sherpa_output.txt";
  int32_t sid = 0;

  po.Register("output-filename", &output_filename,
              "Path to save the generated audio");

  po.Register("log-file", &log_filename,
              "Path to save the log messages. If empty, logs are printed to stderr");

  po.Register("sid", &sid,
              "Speaker ID. Used only for multi-speaker models, e.g., models "
              "trained using the VCTK dataset. Not used for single-speaker "
              "models, e.g., models trained using the LJSpeech dataset");

  sherpa_onnx::OfflineTtsConfig config;

  config.Register(&po);
  po.Read(argc, argv);

  // Set up log redirection if log file is specified
  if (!log_filename.empty()) {
    g_log_file = std::make_unique<std::ofstream>(log_filename, std::ios::out | std::ios::app);
    if (g_log_file->is_open()) {
      sherpa_onnx_log_set(FileLogCallback, nullptr);
      fprintf(stderr, "Log messages will be saved to: %s\n", log_filename.c_str());
      
      // Log the startup information
      SHERPA_ONNX_LOGI("=== sherpa-onnx-offline-tts started ===");
      SHERPA_ONNX_LOGI("Log file: %s", log_filename.c_str());
      SHERPA_ONNX_LOGI("Output file: %s", output_filename.c_str());
      SHERPA_ONNX_LOGI("Speaker ID: %d", sid);
    } else {
      fprintf(stderr, "Warning: Failed to open log file %s. Using stderr for logs.\n", log_filename.c_str());
    }
  }

  if (po.NumArgs() == 0) {
    fprintf(stderr, "Error: Please provide the text to generate audio.\n\n");
    po.PrintUsage();
    exit(EXIT_FAILURE);
  }

  if (po.NumArgs() > 1) {
    fprintf(stderr,
            "Error: Accept only one positional argument. Please use single "
            "quotes to wrap your text\n");
    po.PrintUsage();
    exit(EXIT_FAILURE);
  }

  if (config.model.debug) {
    fprintf(stderr, "%s\n", config.model.ToString().c_str());
  }

  if (!config.Validate()) {
    SHERPA_ONNX_LOGE("Errors in config!");
    fprintf(stderr, "Errors in config!\n");
    exit(EXIT_FAILURE);
  }

  // Read model files into memory if specified
  std::vector<char> vits_model_data;
  std::vector<char> vits_pack_data;
  std::vector<char> vits_token_data;
  std::vector<char> matcha_model_data;
  std::vector<char> kokoro_model_data;
  std::vector<char> kitten_model_data;
  
  // Load VITS model from file to memory
  if (!config.model.vits.model.empty()) {
    if (sherpa_onnx::FileExists(config.model.vits.model)) {
      fprintf(stderr, "Loading VITS model from: %s\n", config.model.vits.model.c_str());
      vits_model_data = sherpa_onnx::ReadFile(config.model.vits.model);
      config.model.vits.model_data = vits_model_data.data();
      config.model.vits.model_data_size = static_cast<int32_t>(vits_model_data.size());
      config.model.vits.model = std::string(); 
      fprintf(stderr, "VITS model loaded into memory: %d bytes\n", config.model.vits.model_data_size);
    }
  }
  
  // Load VITS tokens from file to memory
  if (!config.model.vits.tokens.empty()) {
    if (sherpa_onnx::FileExists(config.model.vits.tokens)) {
      fprintf(stderr, "Loading VITS tokens from: %s\n", config.model.vits.tokens.c_str());
      vits_token_data = sherpa_onnx::ReadFile(config.model.vits.tokens);
      config.model.vits.token_data = vits_token_data.data();
      config.model.vits.token_data_size = static_cast<int32_t>(vits_token_data.size());
      config.model.vits.tokens = std::string(); 
      fprintf(stderr, "VITS tokens loaded into memory: %d bytes\n", config.model.vits.token_data_size);
    }
  }
  
  // Load VITS pack data from file to memory if specified
  if (!config.model.vits.pack_data_path.empty()) {
    if (sherpa_onnx::FileExists(config.model.vits.pack_data_path)) {
      fprintf(stderr, "Loading VITS pack data from: %s\n", config.model.vits.pack_data_path.c_str());
      vits_pack_data = sherpa_onnx::ReadFile(config.model.vits.pack_data_path);
      config.model.vits.pack_data = vits_pack_data.data();
      config.model.vits.pack_data_size = static_cast<int32_t>(vits_pack_data.size());
      config.model.vits.pack_data_path = std::string(); 
      fprintf(stderr, "VITS pack data loaded into memory: %d bytes\n", config.model.vits.pack_data_size);
    }
  }

  SHERPA_ONNX_LOGI("Text to synthesize: %s", po.GetArg(1).c_str());
  SHERPA_ONNX_LOGI("Creating TTS engine...");
  sherpa_onnx::OfflineTts tts(config);

  SHERPA_ONNX_LOGI("Starting audio generation...");
  const auto begin = std::chrono::steady_clock::now();
  auto audio = tts.Generate(po.GetArg(1), sid, 1.0, AudioCallback);
  const auto end = std::chrono::steady_clock::now();

  if (audio.samples.empty()) {
    SHERPA_ONNX_LOGE("Error in generating audio. Please read previous error messages.");
    fprintf(
        stderr,
        "Error in generating audio. Please read previous error messages.\n");
    exit(EXIT_FAILURE);
  }

  float elapsed_seconds =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - begin)
          .count() /
      1000.;
  float duration = audio.samples.size() / static_cast<float>(audio.sample_rate);

  float rtf = elapsed_seconds / duration;
  
  // Log performance metrics
  SHERPA_ONNX_LOGI("Performance metrics:");
  SHERPA_ONNX_LOGI("  Number of threads: %d", config.model.num_threads);
  SHERPA_ONNX_LOGI("  Elapsed seconds: %.3f s", elapsed_seconds);
  SHERPA_ONNX_LOGI("  Audio duration: %.3f s", duration);
  SHERPA_ONNX_LOGI("  Real-time factor (RTF): %.3f", rtf);
  SHERPA_ONNX_LOGI("  Sample rate: %d Hz", audio.sample_rate);
  SHERPA_ONNX_LOGI("  Number of samples: %d", static_cast<int>(audio.samples.size()));
  
  fprintf(stderr, "Number of threads: %d\n", config.model.num_threads);
  fprintf(stderr, "Elapsed seconds: %.3f s\n", elapsed_seconds);
  fprintf(stderr, "Audio duration: %.3f s\n", duration);
  fprintf(stderr, "Real-time factor (RTF): %.3f/%.3f = %.3f\n", elapsed_seconds,
          duration, rtf);

  SHERPA_ONNX_LOGI("Writing audio to file: %s", output_filename.c_str());
  bool ok = sherpa_onnx::WriteWave(output_filename, audio.sample_rate,
                                   audio.samples.data(), audio.samples.size());
  if (!ok) {
    SHERPA_ONNX_LOGE("Failed to write wave to %s", output_filename.c_str());
    fprintf(stderr, "Failed to write wave to %s\n", output_filename.c_str());
    exit(EXIT_FAILURE);
  }

  fprintf(stderr, "The text is: %s. Speaker ID: %d\n", po.GetArg(1).c_str(),
          sid);
  fprintf(stderr, "Saved to %s successfully!\n", output_filename.c_str());
  
  SHERPA_ONNX_LOGI("Audio generation completed successfully");
  SHERPA_ONNX_LOGI("=== sherpa-onnx-offline-tts finished ===");

  // Clean up log file
  if (g_log_file && g_log_file->is_open()) {
    g_log_file->close();
  }

  return 0;
}
