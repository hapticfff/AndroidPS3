#include "emulator/logging/Logger.h"

namespace emu::logging {
const char* ToString(Category c) {
    switch (c) {
        case Category::CPU:return "CPU"; case Category::SPU:return "SPU"; case Category::GPU:return "GPU";
        case Category::VULKAN:return "VULKAN"; case Category::MEMORY:return "MEMORY"; case Category::OS:return "OS";
        case Category::LOADER:return "LOADER"; case Category::AUDIO:return "AUDIO"; case Category::INPUT:return "INPUT";
        case Category::ANDROID_LOG:return "ANDROID"; case Category::GENERAL:return "GENERAL";
    } return "UNKNOWN";
}
const char* ToString(Level l) {
    switch (l) { case Level::Trace:return "TRACE"; case Level::Debug:return "DEBUG"; case Level::Info:return "INFO";
        case Level::Warning:return "WARNING"; case Level::Error:return "ERROR"; case Level::Fatal:return "FATAL"; }
    return "UNKNOWN";
}
Logger& Logger::Instance() { static Logger x; return x; }
void Logger::SetMinLevel(Level l) { std::lock_guard<std::mutex> lock(mutex_); min_level_=l; }
Level Logger::GetMinLevel() const { std::lock_guard<std::mutex> lock(mutex_); return min_level_; }
void Logger::Log(Category c, Level l, std::string_view m) {
    std::lock_guard<std::mutex> lock(mutex_);
    if (static_cast<int>(l) < static_cast<int>(min_level_)) return;
    std::fprintf(stdout, "[%s] [%s] %.*s\n", ToString(c), ToString(l), static_cast<int>(m.size()), m.data());
    std::fflush(stdout);
}
} // namespace emu::logging
