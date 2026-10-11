#pragma once

#include <filesystem>

#include "JsonValue.h"

namespace Axiom
{
    class JsonFile
    {
    public:
        JsonFile() = default;

        [[nodiscard]]
        bool Load(const std::filesystem::path& Path, FString& Error);

        [[nodiscard]]
        const JsonValue& GetRoot() const;

    private:
        static bool ReadJson(const std::filesystem::path& Path, FString& Text, FString& Error);

        JsonValue Root_;
    };
}
