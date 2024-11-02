#include "Logger.h"

 std::shared_ptr<spdlog::logger> Chess_Game::Logger::s_loggerConsole;
void Chess_Game::Logger::Init()
{
    spdlog::set_pattern("[%Hh:%Mm:%Ss] %^ %v %$");

    s_loggerConsole = spdlog::stdout_color_mt("console");
    auto err_logger = spdlog::stderr_color_mt("stderr");
}
