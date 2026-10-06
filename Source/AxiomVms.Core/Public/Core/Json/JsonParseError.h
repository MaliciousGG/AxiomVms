#pragma once

#include "Core/Json/JsonSourceLocation.h"
#include "Core/Types.h"

namespace AxiomVms
{
    struct JsonParseError
    {
        FString Message;
        JsonSourceLocation Location;
    };
}