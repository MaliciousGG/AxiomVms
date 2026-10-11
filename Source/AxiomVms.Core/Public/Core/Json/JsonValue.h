#pragma once

#include "Core/Types.h"


namespace Axiom
{
    class JsonValue;

    using JsonArray = TArray<JsonValue>;
    using JsonObject = TMap<FString, JsonValue>;
    using JsonVariant = TVariant<std::nullptr_t, bool, float32,
        float64, FString, JsonArray, JsonObject>;


    enum class EJsonValueType : uint8
    {
        Null,
        Boolean,
        Number,
        String,
        Array,
        Object
    };

    class JsonValue
    {
    public:
        JsonValue();

        explicit JsonValue(std::nullptr_t);
        explicit JsonValue(bool Value);
        explicit JsonValue(float32 Value);
        explicit JsonValue(float64 Value);
        explicit JsonValue(FString Value);
        explicit JsonValue(const TCHAR* Value);
        explicit JsonValue(JsonArray Value);
        explicit JsonValue(JsonObject Value);

        bool AsBoolean() const;

        float32 AsFloat32() const;
        float64 AsFloat64() const;

        const FString& AsFString() const;

        JsonArray& AsArray();
        const JsonArray& AsArray() const;

        JsonObject& AsObject();
        const JsonObject& AsObject() const;

        EJsonValueType GetType() const;
        const JsonVariant& GetValue() const;
        
        bool IsNull() const;
        bool IsBoolean() const;
        bool IsNumber() const;
        bool IsString() const;
        bool IsArray() const;
        bool IsObject() const;

    private:
        EJsonValueType ValueType_;
        JsonVariant Value_;
    };

    constexpr std::string_view ToString(const EJsonValueType Type)
    {
        switch (Type)
        {
            case EJsonValueType::Null:    return "Null";
            case EJsonValueType::Boolean: return "Boolean";
            case EJsonValueType::Number:  return "Number";
            case EJsonValueType::String:  return "String";
            case EJsonValueType::Array:   return "Array";
            case EJsonValueType::Object:  return "Object";
        }

        return "Unknown";
    }
}