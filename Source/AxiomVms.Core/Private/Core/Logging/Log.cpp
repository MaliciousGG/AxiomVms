#include "Core/Logging/Log.h"

#include <utility>

namespace AxiomVms
{
    std::unique_ptr<ILogger> Log::Logger_;

    void Log::Configure(std::unique_ptr<ILogger> logger)
    {
        if (!logger)
        {
            throw std::invalid_argument("Logger cannot be null.");
        }
    
        if (Logger_)
        {
            throw std::logic_error("Logger has already been configured.");
        }
    
        Logger_ = std::move(logger);
    }

    void Log::Write(const ELogLevel level, const std::string_view message)
    {
        if (!Logger_)
        {
            return;
        }

        Logger_->Write(level, message);
    }
}