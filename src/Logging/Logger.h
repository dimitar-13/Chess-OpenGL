#pragma once
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Chess_Game
{
    class Logger
    {
    public:
       static void Init();
    public:
        static std::shared_ptr<spdlog::logger> GetLoggerConsole() { return s_loggerConsole;}
    private:
        static std::shared_ptr<spdlog::logger> s_loggerConsole;
    };
}
#ifdef CHESS_DEBUG
#define CHESS_LOG_INFO(...)   Chess_Game::Logger::GetLoggerConsole()->info(__VA_ARGS__)
#define CHESS_LOG_WARN(...)   Chess_Game::Logger::GetLoggerConsole()->warn(__VA_ARGS__)
#define CHESS_LOG_ERROR(...)  Chess_Game::Logger::GetLoggerConsole()->error(__VA_ARGS__)
#define CHESS_LOG_FATAL(...)  Chess_Game::Logger::GetLoggerConsole()->critical(__VA_ARGS__)
#else
#define CHESS_LOG_INFO
#define CHESS_LOG_WARN
#define CHESS_LOG_ERROR
#define CHESS_LOG_FATAL
#endif // CHESS_DEBUG