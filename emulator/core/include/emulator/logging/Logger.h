#pragma once

#include <cstdio>
#include <mutex>
#include <string>
#include <string_view>

namespace emu::logging {

enum class Category { CPU, SPU, GPU, VULKAN, MEMORY, OS, LOADER, AUDIO, INPUT, ANDROID, GENERAL };
enum class Level { Trace, Debug, Info, Warning, Error, Fatal };

const char* ToString(Category category);
const char* ToString(Level level);

class Logger {
public:
    static Logger& Instance();
    void SetMinLevel(Level level);
    Level GetMinLevel() const;
    void Log(Category category, Level level, std::string_view message);
private:
    Logger() = default;
    mutable std::mutex mutex_;
    Level min_level_ = Level::Debug;
};

} // namespace emu::logging

#define EMU_LOG(category, level, message) ::emu::logging::Logger::Instance().Log((category), (level), (message))
#define EMU_LOG_TRACE(category, message) EMU_LOG(category, ::emu::logging::Level::Trace, message)
#define EMU_LOG_DEBUG(category, message) EMU_LOG(category, ::emu::logging::Level::Debug, message)
#define EMU_LOG_INFO(category, message) EMU_LOG(category, ::emu::logging::Level::Info, message)
#define EMU_LOG_WARNING(category, message) EMU_LOG(category, ::emu::logging::Level::Warning, message)
#define EMU_LOG_ERROR(category, message) EMU_LOG(category, ::emu::logging::Level::Error, message)
#define EMU_LOG_FATAL(category, message) EMU_LOG(category, ::emu::logging::Level::Fatal, message)
