#pragma once

#include "Core/Types.h"

namespace AxiomVms
{
    struct JsonSourceLocation
    {
        Size_T Offset = 0;
        Size_T Line = 1;
        Size_T Column = 1;
    };
}