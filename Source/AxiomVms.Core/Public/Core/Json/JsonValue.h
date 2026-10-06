#pragma once

#include "Core/Types.h"


namespace AxiomVms
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
        explicit JsonValue(bool value);
        explicit JsonValue(float32 value);
        explicit JsonValue(float64 value);
        explicit JsonValue(FString value);
        explicit JsonValue(const TCHAR* value);
        explicit JsonValue(JsonArray value);
        explicit JsonValue(JsonObject value);

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

    constexpr std::string_view ToString(EJsonValueType type)
    {
        switch (type)
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