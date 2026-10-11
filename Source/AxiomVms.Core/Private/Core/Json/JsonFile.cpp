#include "Core/Json/JsonFile.h"

#include <fstream>

#include "Core/Json/JsonParser.h"

namespace Axiom
{
    bool JsonFile::Load(const std::filesystem::path& Path, FString& Error)
    {
        FString Text;

        if (!ReadJson(Path, Text, Error))
        {
            return false;
        }

        JsonParser Parser;
        JsonValue ParsedRoot = Parser.Parse(Text);

        if (const TArray<JsonParseError> Errors = Parser.GetErrors(); !Errors.empty())
        {
            Error = Errors[0].Message;

            return false;
        }

        Root_ = std::move(ParsedRoot);

        return true;
    }

    const JsonValue& JsonFile::GetRoot() const
    {
        return Root_;
    }

    bool JsonFile::ReadJson(const std::filesystem::path& Path, FString& Text, FString& Error)
    {
        Text.clear();
        Error.clear();

        std::ifstream File(Path, std::ios::binary);

        if (!File.is_open())
        {
            Error = "Failed to open file: " + Path.string();

            return false;
        }

        std::ostringstream Buffer;

        Buffer << File.rdbuf();

        if (File.bad())
        {
            Error = "Failed to read JSON file.";

            return false;
        }

        Text = Buffer.str();

        return true;
    }
}
