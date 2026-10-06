#pragma once

#include "Core/Logging/LogLevel.h"

#include <string_view>

namespace AxiomVms
{
    class ILogger
    {
    public:
        virtual ~ILogger() = default;

        virtual void Write(ELogLevel level, std::string_view message) = 0;

    protected:
        virtual bool ShouldLog(ELogLevel level) const = 0;
        virtual void SetLevel(ELogLevel level) = 0;
    };
}