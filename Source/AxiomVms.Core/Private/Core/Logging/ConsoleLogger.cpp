#include "Core/Logging/ConsoleLogger.h"

#include <chrono>
#include <iostream>

namespace Axiom
{
    void ConsoleLogger::Write(const ELogLevel InLevel, const std::string_view InMessage)
    {
        if (!ShouldLog(InLevel))
        {
            return;
        }

        const auto Now = std::chrono::system_clock::now();

        const std::string Formatted = std::format(
            "[{:%Y-%m-%d %H:%M:%S}] [{}] {}",
            Now,
            ToString(InLevel),
            InMessage
        );

        switch (InLevel)
        {
            case ELogLevel::Error:
            case ELogLevel::Fatal:
                std::cerr << Formatted << '\n';
                break;

            default:
                std::cout << Formatted << '\n';
                break;
        }
    }

    bool ConsoleLogger::ShouldLog(const ELogLevel InLevel) const
    {
        return InLevel >= LogLevel_;
    }

    void ConsoleLogger::SetLevel(const ELogLevel InLevel)
    {
        LogLevel_ = InLevel;
    }
}