#pragma once

#include "Core/Compiler.h"
#include "Core/Logging/Log.h"

#if !defined(NDEBUG)

    #define AXIOM_ASSERT(condition)                         \
        do                                                  \
        {                                                   \
            if (!(condition))                               \
            {                                               \
                AXIOM_LOG_FATAL(                         \
                    "Assertion failed: {}",                 \
                    #condition);                            \
                                                            \
                AXIOM_DEBUG_BREAK();                        \
            }                                               \
        } while (false)

    #define AXIOM_ASSERT_MSG(condition, ...)                \
        do                                                  \
        {                                                   \
            if (!(condition))                               \
            {                                               \
                AXIOM_LOG_FATAL(__VA_ARGS__);            \
                AXIOM_DEBUG_BREAK();                        \
            }                                               \
        } while (false)

#else

    #define AXIOM_ASSERT(condition) \
        ((void)0)

    #define AXIOM_ASSERT_MSG(condition, ...) \
        ((void)0)

#endif