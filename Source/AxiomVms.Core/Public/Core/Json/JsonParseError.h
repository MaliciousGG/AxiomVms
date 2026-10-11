#pragma once

#include "Core/Json/JsonSourceLocation.h"
#include "Core/Types.h"

namespace Axiom
{
    struct JsonParseError
    {
        FString Message;
        JsonSourceLocation Location;
    };
}