#include "Core/Logging/Log.h"

#include <utility>

namespace Axiom
{
    std::unique_ptr<ILogger> Log::Logger_;

    void Log::Configure(std::unique_ptr<ILogger> InLogger)
    {
        if (!InLogger)
        {
            throw std::invalid_argument("Logger cannot be null.");
        }
    
        if (Logger_)
        {
            throw std::logic_error("Logger has already been configured.");
        }
    
        Logger_ = std::move(InLogger);
    }

    void Log::Write(const ELogLevel InLevel, const std::string_view InMessage)
    {
        if (!Logger_)
        {
            return;
        }

        Logger_->Write(InLevel, InMessage);
    }
}