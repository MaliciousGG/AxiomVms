#pragma once

#include <cstdint>

// Signed Integers
using int8   = std::int8_t;
using int16  = std::int16_t;
using int32  = std::int32_t;
using int64  = std::int64_t;

// Unsigned Integers
using uint8  = std::uint8_t;
using uint16 = std::uint16_t;
using uint32 = std::uint32_t;
using uint64 = std::uint64_t;

// Floating Points
using float32 = float;
using float64 = double;

// Size Type
using Size_T = std::size_t;

#include <string>
using TCHAR = char;
using FString = std::string;
#define AXIOM_TEXT(x) x // TCHAR maintains a UTF-8 encoding for now


#include <variant>
template<typename... T>
using TVariant = std::variant<T...>;

#include <vector>
template<typename T>
using TArray = std::vector<T>;

#include <unordered_map>
template<typename T, typename V>
using TMap = std::unordered_map<T, V>;



