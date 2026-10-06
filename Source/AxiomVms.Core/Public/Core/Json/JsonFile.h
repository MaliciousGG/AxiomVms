#pragma once

#include <filesystem>

#include "JsonValue.h"

namespace AxiomVms
{
    class JsonFile
    {
    public:
        JsonFile() = default;

        [[nodiscard]]
        bool Load(const std::filesystem::path& path, FString& error);

        [[nodiscard]]
        const JsonValue& GetRoot() const;

    private:
        static bool ReadJson(const std::filesystem::path& path, FString& text, FString& error);

        JsonValue Root_;
    };
}
