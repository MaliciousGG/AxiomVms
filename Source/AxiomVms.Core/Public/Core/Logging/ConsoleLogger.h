#pragma once

#include "Core/Logging/ILogger.h"

namespace AxiomVms
{
    class ConsoleLogger final : public ILogger
    {
    public:
        ConsoleLogger() = default;

        explicit ConsoleLogger(ELogLevel level)
            : LogLevel_(level)
        {}

        void Write(ELogLevel level, std::string_view message) override;

        bool ShouldLog(ELogLevel level) const override;
        void SetLevel(ELogLevel level) override;

    private:
        ELogLevel LogLevel_{ELogLevel::Info};
    };
}