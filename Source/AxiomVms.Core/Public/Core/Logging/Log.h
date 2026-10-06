#pragma once

#include "Core/Logging/ILogger.h"

#include <format>
#include <memory>
#include <string_view>
#include <utility>

namespace AxiomVms
{
    class Log
    {
    public:
        static void Configure(std::unique_ptr<ILogger> logger);

        template<typename... Args>
        static void Debug(std::format_string<Args...> format,Args&&... args)
        {
            Write(ELogLevel::Debug,std::format(format, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void Info(std::format_string<Args...> format,Args&&... args)
        {
            Write(ELogLevel::Info,std::format(format, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void Warning(std::format_string<Args...> format,Args&&... args)
        {
            Write(ELogLevel::Warning,std::format(format, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void Error(std::format_string<Args...> format,Args&&... args)
        {
            Write(ELogLevel::Error,std::format(format, std::forward<Args>(args)...));
        }

        template<typename... Args>
        static void Fatal(std::format_string<Args...> format, Args&&... args)
        {
            Write(ELogLevel::Fatal, std::format(format, std::forward<Args>(args)...));
        }

    private:
        static void Write(ELogLevel level, std::string_view message);

        static std::unique_ptr<ILogger> Logger_;
    };
}

#if !defined(NDEBUG)

    #define AXIOM_LOG_TRACE(...) \
        ::AxiomVms::Log::Trace(__VA_ARGS__)

    #define AXIOM_LOG_DEBUG(...) \
        ::AxiomVms::Log::Debug(__VA_ARGS__)

#else

    #define AXIOM_LOG_TRACE(...) ((void)0)
    #define AXIOM_LOG_DEBUG(...) ((void)0)

#endif

#define AXIOM_LOG_INFO(...) \
    ::AxiomVms::Log::Info(__VA_ARGS__)

#define AXIOM_LOG_WARNING(...) \
    ::AxiomVms::Log::Warning(__VA_ARGS__)

#define AXIOM_LOG_ERROR(...) \
    ::AxiomVms::Log::Error(__VA_ARGS__)

#define AXIOM_LOG_FATAL(...) \
    ::AxiomVms::Log::Fatal(__VA_ARGS__)
