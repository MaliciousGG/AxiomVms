#pragma once

#if defined(_WIN32)
    #define AXIOM_PLATFORM_WINDOWS 1
    #define AXIOM_PLATFORM_LINUX   0
    #define AXIOM_PLATFORM_MACOS   0

#elif defined(__APPLE__)
    #define AXIOM_PLATFORM_WINDOWS 0
    #define AXIOM_PLATFORM_LINUX   0
    #define AXIOM_PLATFORM_MACOS   1

#elif defined(__linux__)
    #define AXIOM_PLATFORM_WINDOWS 0
    #define AXIOM_PLATFORM_LINUX   1
    #define AXIOM_PLATFORM_MACOS   0
    
#else
    #error Unsupported platform
#endif