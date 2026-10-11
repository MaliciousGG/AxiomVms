#pragma once

#include "Core/Logging/ILogger.h"

namespace Axiom
{
    class ConsoleLogger final : public ILogger
    {
    public:
        ConsoleLogger() = default;

        explicit ConsoleLogger(ELogLevel InLevel)
            : LogLevel_(InLevel)
        {}

        void Write(ELogLevel InLevel, std::string_view InMessage) override;

        bool ShouldLog(ELogLevel InLevel) const override;
        void SetLevel(ELogLevel InLevel) override;

    private:
        ELogLevel LogLevel_{ELogLevel::Info};
    };
}