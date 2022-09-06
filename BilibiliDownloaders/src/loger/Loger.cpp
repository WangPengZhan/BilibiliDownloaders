#include "Loger.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/async.h>

namespace
{
    constexpr int logFileMaxSize = 20 * 1024 * 1024;
}

Logger& Logger::GetInstance()
{
    static Logger logger;
    return logger;
}

void Logger::InitLog()
{
    RegisterLogger("Network");
    RegisterLogger("Aria2");
}

Logger::Logger()
{
    SetLog();
    InitLog();
}

Logger::~Logger()
{
    spdlog::shutdown();
}

void Logger::RegisterLogger(const std::string& logName)
{
    spdlog::rotating_logger_mt<spdlog::async_factory>(logName, "log/" + logName + ".log", logFileMaxSize, 100);
    // spdlog::create_async<spdlog::sinks::basic_file_sink_mt>(logName, "log/" + logName + ".log", logFileMaxSize, 100);
}

void Logger::SetLog()
{
    spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e][thread %t][%@,%!][%l] : %v");
    spdlog::set_level(spdlog::level::trace);
    spdlog::flush_on(spdlog::level::err);
    spdlog::flush_every(std::chrono::seconds(5));
    spdlog::init_thread_pool(32768, 6);
}