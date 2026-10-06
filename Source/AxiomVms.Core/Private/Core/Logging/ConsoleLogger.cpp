#include "Core/Logging/ConsoleLogger.h"

#include <chrono>
#include <iostream>

namespace AxiomVms
{
    void ConsoleLogger::Write(const ELogLevel level, const std::string_view message)
    {
        if (!ShouldLog(level))
        {
            return;
        }

        const auto now = std::chrono::system_clock::now();

        const std::string formatted = std::format("[{:%Y-%m-%d %H:%M:%S}] [{}] {}",
                now,
                ToString(level),
                message);

        switch (level)
        {
            case ELogLevel::Error:
            case ELogLevel::Fatal:
                std::cerr << formatted << '\n';
                break;

            default:
                std::cout << formatted << '\n';
                break;
        }
    }

    bool ConsoleLogger::ShouldLog(const ELogLevel level) const
    {
        return level >= LogLevel_;
    }

    void ConsoleLogger::SetLevel(const ELogLevel level)
    {
        LogLevel_ = level;
    }
}