#pragma once


#if defined(_MSC_VER)

    #define AXIOM_COMPILER_MSVC 1
    #define AXIOM_COMPILER_CLANG 0
    #define AXIOM_COMPILER_GCC 0

    #define AXIOM_FORCE_INLINE __forceinline
    #define AXIOM_NO_INLINE __declspec(noinline)
    #define AXIOM_DEBUG_BREAK() __debugbreak()

#elif defined(__clang__)

    #define AXIOM_COMPILER_MSVC 0
    #define AXIOM_COMPILER_CLANG 1
    #define AXIOM_COMPILER_GCC 0

    #define AXIOM_FORCE_INLINE inline __attribute__((always_inline))
    #define AXIOM_NO_INLINE __attribute__((noinline))
    #define AXIOM_DEBUG_BREAK() __builtin_trap()

#elif defined(__GNUC__)

    #define AXIOM_COMPILER_MSVC 0
    #define AXIOM_COMPILER_CLANG 0
    #define AXIOM_COMPILER_GCC 1

    #define AXIOM_FORCE_INLINE inline __attribute__((always_inline))
    #define AXIOM_NO_INLINE __attribute__((noinline))
    #define AXIOM_DEBUG_BREAK() __builtin_trap()

#else

    #error "Unsupported compiler."

#endif