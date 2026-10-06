#pragma once

#include "Core/Json/JsonParseError.h"
#include "Core/Types.h"
#include "Core/Json/JsonValue.h"
#include "Core/Json/JsonSourceLocation.h"

namespace AxiomVms
{
    class JsonParser
    {
        using JsonArray = TArray<JsonValue>;
        using JsonObject = TMap<FString, JsonValue>;
    
    public:
        JsonValue Parse(const FString& input);
    
        JsonValue ParseValue();
    
        JsonObject ParseObject();
        JsonArray ParseArray();
    
        FString ParseString();
        float64 ParseNumber();
        
        bool ParseBoolean();
        void ParseNull();
    
        void SkipWhitespace();
    
        [[nodiscard]]
        TCHAR Current() const;

        TCHAR Next();

        [[nodiscard]]
        bool IsAtEnd() const;
    
        [[nodiscard]]
        std::string_view GetInput() const;

        [[nodiscard]]
        JsonSourceLocation GetLocation() const;

        [[nodiscard]]
        TArray<JsonParseError> GetErrors() const;

        void Reset();
        void LogErrors();
        void AddError(const JsonParseError& error);
    
    private:
        std::string_view Input_{};
        JsonSourceLocation Location_{};
        TArray<JsonParseError> Errors_{};
    };
}