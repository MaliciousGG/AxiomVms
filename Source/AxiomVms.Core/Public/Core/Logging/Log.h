#pragma once

#include "Core/Logging/ILogger.h"

#include <format>
#include <memory>
#include <string_view>
#include <utility>

namespace Axiom
{
    class Log
    {
    public:
        static void Configure(std::unique_ptr<ILogger> InLogger);

        template<typename... Args>
        static void Debug(std::format_string<Args...> Format, Args&&... InArgs)
        {
            Write(ELogLevel::Debug, std::format(Format, std::forward<Args>(InArgs)...));
        }

        template<typename... Args>
         static void Info(std::format_string<Args...> Format, Args&&... InArgs)
        {
            Write(ELogLevel::Info, std::format(Format, std::forward<Args>(InArgs)...));
        }

        template<typename... Args>
        static void Warning(std::format_string<Args...> Format, Args&&... InArgs)
        {
            Write(ELogLevel::Warning, std::format(Format, std::forward<Args>(InArgs)...));
        }

        template<typename... Args>
        static void Error(std::format_string<Args...> Format, Args&&... InArgs)
        {
            Write(ELogLevel::Error, std::format(Format, std::forward<Args>(InArgs)...));
        }

        template<typename... Args>
        static void Fatal(std::format_string<Args...> Format, Args&&... InArgs)
        {
            Write(ELogLevel::Fatal, std::format(Format, std::forward<Args>(InArgs)...));
        }

    private:
        static void Write(ELogLevel InLevel, std::string_view InMessage);

        static std::unique_ptr<ILogger> Logger_;
    };
}

#if !defined(NDEBUG)

    #define AXIOM_LOG_TRACE(...) \
        ::Axiom::Log::Trace(__VA_ARGS__)

    #define AXIOM_LOG_DEBUG(...) \
        ::Axiom::Log::Debug(__VA_ARGS__)

#else

    #define AXIOM_LOG_TRACE(...) ((void)0)
    #define AXIOM_LOG_DEBUG(...) ((void)0)

#endif

#define AXIOM_LOG_INFO(...) \
    ::Axiom::Log::Info(__VA_ARGS__)

#define AXIOM_LOG_WARNING(...) \
    ::Axiom::Log::Warning(__VA_ARGS__)

#define AXIOM_LOG_ERROR(...) \
    ::Axiom::Log::Error(__VA_ARGS__)

#define AXIOM_LOG_FATAL(...) \
    ::Axiom::Log::Fatal(__VA_ARGS__)
