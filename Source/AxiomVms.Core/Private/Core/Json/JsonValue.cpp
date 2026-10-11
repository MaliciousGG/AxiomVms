#include "Core/Json/JsonValue.h"
#include "Core/Assert.h"

namespace Axiom
{
    JsonValue::JsonValue()
        : ValueType_(EJsonValueType::Null)
        , Value_(nullptr)
    {
    }

    JsonValue::JsonValue(std::nullptr_t)
        : ValueType_(EJsonValueType::Null)
        , Value_(nullptr)
    {
    }

    JsonValue::JsonValue(bool Value)
        : ValueType_(EJsonValueType::Boolean)
        , Value_(Value)
    {
    }

    JsonValue::JsonValue(float32 Value)
        : ValueType_(EJsonValueType::Number)
        , Value_(Value)
    {
    }

    JsonValue::JsonValue(float64 Value)
        : ValueType_(EJsonValueType::Number)
        , Value_(Value)
    {
    }

    JsonValue::JsonValue(FString Value)
        : ValueType_(EJsonValueType::String)
        , Value_(std::move(Value))
    {
    }

    JsonValue::JsonValue(const TCHAR* Value)
        : ValueType_(EJsonValueType::String)
        , Value_(std::move(FString(Value)))
    {
    }

    JsonValue::JsonValue(JsonArray Value)
        : ValueType_(EJsonValueType::Array)
        , Value_(std::move(Value))
    {
    }

    JsonValue::JsonValue(JsonObject Value)
        : ValueType_(EJsonValueType::Object)
        , Value_(std::move(Value))
    {
    }

    bool JsonValue::AsBoolean() const
    {
        AXIOM_ASSERT_MSG(
            ValueType_ == EJsonValueType::Boolean,
            "Attempted to pass non-boolean value to JsonValue::AsBoolean(): ValueType_ = {}",
            ToString(ValueType_));

        return std::get<bool>(Value_);
    }

    float32 JsonValue::AsFloat32() const
    {
        AXIOM_ASSERT_MSG(
            ValueType_ == EJsonValueType::Number,
            "Attempted to pass non-number value to JsonValue::AsFloat32(): ValueType_ = {}",
            ToString(ValueType_));

        return std::get<float>(Value_);
    }

    float64 JsonValue::AsFloat64() const
    {
        AXIOM_ASSERT_MSG(
            ValueType_ == EJsonValueType::Number,
            "Attempted to pass non-number value to JsonValue::AsFloat64(): ValueType_ = {}",
            ToString(ValueType_));

        return std::get<double>(Value_);
    }

    const FString& JsonValue::AsFString() const
    {
        AXIOM_ASSERT_MSG(
            ValueType_ == EJsonValueType::String,
            "Attempted to pass non-string value to JsonValue::AsFString(): ValueType_ = {}",
            ToString(ValueType_));

        return std::get<FString>(Value_);
    }

    JsonArray& JsonValue::AsArray()
    {
        AXIOM_ASSERT_MSG(
            ValueType_ == EJsonValueType::Array,
            "Attempted to access non-array JsonValue. ValueType_ = {}",
            ToString(ValueType_));

        return std::get<JsonArray>(Value_);
    }

    const JsonArray& JsonValue::AsArray() const
    {
        AXIOM_ASSERT_MSG(
            ValueType_ == EJsonValueType::Array,
            "Attempted to pass non-array value to JsonValue::AsArray(): ValueType_ = {}",
            ToString(ValueType_));

        return std::get<JsonArray>(Value_);
    }

    JsonObject& JsonValue::AsObject()
    {
        AXIOM_ASSERT_MSG(
            ValueType_ == EJsonValueType::Object,
            "Attempted to access non-object JsonValue. ValueType_ = {}",
            ToString(ValueType_));

        return std::get<JsonObject>(Value_);
    }

    const JsonObject& JsonValue::AsObject() const
    {
        AXIOM_ASSERT_MSG(
            ValueType_ == EJsonValueType::Object,
            "Attempted to pass non-object value to JsonValue::AsObject(): ValueType_ = {}",
            ToString(ValueType_));

        return std::get<JsonObject>(Value_);
    }

    

    EJsonValueType JsonValue::GetType() const
    {
        return ValueType_;
    }

    const JsonVariant& JsonValue::GetValue() const
    {
        return Value_;
    }
    

    bool JsonValue::IsNull() const
    {
        return ValueType_ == EJsonValueType::Null;
    }

    bool JsonValue::IsBoolean() const
    {
        return ValueType_ == EJsonValueType::Boolean;
    }

    bool JsonValue::IsNumber() const
    {
        return ValueType_ == EJsonValueType::Number;
    }

    bool JsonValue::IsString() const
    {
        return ValueType_ == EJsonValueType::String;
    }

    bool JsonValue::IsArray() const
    {
        return ValueType_ == EJsonValueType::Array;
    }

    bool JsonValue::IsObject() const
    {
        return ValueType_ == EJsonValueType::Object;
    }
}