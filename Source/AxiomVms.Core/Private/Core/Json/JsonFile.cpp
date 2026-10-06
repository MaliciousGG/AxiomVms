#include "Core/Json/JsonFile.h"

#include <fstream>

#include "Core/Json/JsonParser.h"

namespace AxiomVms
{
    bool JsonFile::Load(const std::filesystem::path& path, FString& error)
    {
        FString text;

        if (!ReadJson(path, text, error))
        {
            return false;
        }

        JsonParser parser;
        JsonValue parsedRoot = parser.Parse(text);

        if (const TArray<JsonParseError> errors = parser.GetErrors(); !errors.empty())
        {
            error = errors[0].Message;

            return false;
        }

        Root_ = std::move(parsedRoot);

        return true;
    }

    const JsonValue& JsonFile::GetRoot() const
    {
        return Root_;
    }

    bool JsonFile::ReadJson(const std::filesystem::path& path, FString& text, FString& error)
    {
        text.clear();
        error.clear();

        std::ifstream file(path, std::ios::binary);

        if (!file.is_open())
        {
            error = "Failed to open file: " + path.string();

            return false;
        }

        std::ostringstream buffer;

        buffer << file.rdbuf();

        if (file.bad())
        {
            error = "Failed to read JSON file.";
            return false;
        }

        text = buffer.str();

        return true;
    }
}
