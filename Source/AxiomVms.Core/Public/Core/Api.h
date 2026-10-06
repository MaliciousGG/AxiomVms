#pragma once

#if AXIOM_PLATFORM_LINUX

#include "Core/Platform.h"

#endif

#if defined(AXIOM_CORE_SHARED)

    #if AXIOM_PLATFORM_WINDOWS
        #if defined(AXIOM_CORE_EXPORTS)
            #define AXIOM_API __declspec(dllexport)
        #else
            #define AXIOM_API __declspec(dllimport)
        #endif
    #else
        #define AXIOM_API __attribute__((visibility("default")))
    #endif

#else
    #define AXIOM_API
#endif