#include <cstdint>
#include <string>

namespace AxiomVms
{
    enum class ELogLevel : std::uint8_t
    {
        Fatal,
        Error,
        Warning,
        Info,
        Debug
    };

    inline std::string ToString(ELogLevel level)
    {
        switch (level)
        {
            case ELogLevel::Fatal:
                return "Fatal";

            case ELogLevel::Error:
                return "Error";

            case ELogLevel::Warning:
                return "Warning";

            case ELogLevel::Info:
                return "Info";

            case ELogLevel::Debug:
                return "Debug";
                
            default:
                return "Unknown";
        }
    }
}
