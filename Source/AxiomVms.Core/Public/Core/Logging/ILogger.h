#pragma once

#include "Core/Logging/LogLevel.h"

#include <string_view>

namespace Axiom
{
    class ILogger
    {
    public:
        virtual ~ILogger() = default;

        virtual void Write(ELogLevel InLevel, std::string_view InMessage) = 0;

    protected:
        virtual bool ShouldLog(ELogLevel InLevel) const = 0;
        virtual void SetLevel(ELogLevel InLevel) = 0;
    };
}