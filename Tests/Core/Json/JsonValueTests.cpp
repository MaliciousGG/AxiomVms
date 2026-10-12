#include <gtest/gtest.h>

#include "Core/Json/JsonValue.h"

namespace Axiom::Tests
{
    TEST(JsonValueTests, EmptyConstructorIsNullPointer)
    {
        const JsonValue JsonValue;
        EXPECT_EQ(JsonValue.IsNull(), true);
    }

    TEST(JsonValueTests, NullPointerConstructorIsNull)
    {
        const JsonValue JsonValue(nullptr);
        EXPECT_EQ(JsonValue.IsNull(), true);
    }

    TEST(JsonValueTests, BooleanConstructorIsBoolean)
    {
        const JsonValue JsonValue(true);
        EXPECT_EQ(JsonValue.IsBoolean(), true);
    }

    TEST(JsonValueTests, Float32ConstructorIsNumber)
    {
        const JsonValue JsonValue(1.0f);
        EXPECT_EQ(JsonValue.IsNumber(), true);
    }

    TEST(JsonValueTests, Float64ConstructorIsNumber)
    {
        const JsonValue JsonValue(1.0);
        EXPECT_EQ(JsonValue.IsNumber(), true);
    }

    TEST(JsonValueTests, StringConstructorIsString)
    {
        const JsonValue JsonValue("test");
        EXPECT_EQ(JsonValue.IsString(), true);
    }
    
    TEST(JsonValueTests, ArrayConstructorIsArray)
    {
        const JsonValue JsonValue(JsonArray{});
        EXPECT_EQ(JsonValue.IsArray(), true);
    }

    TEST(JsonValueTests, ObjectConstructorIsObject)
    {
        const JsonValue JsonValue(JsonObject{});

        EXPECT_EQ(JsonValue.IsObject(), true);
    }

    TEST(JsonValueTests, AsBoolean)
    {
        const JsonValue JsonValue(true);
    
        EXPECT_TRUE(JsonValue.AsBoolean());
    }
    
    TEST(JsonValueTests, AsFloat32)
    {
        const JsonValue JsonValue(1.0f);
    
        EXPECT_FLOAT_EQ(JsonValue.AsFloat32(), 1.0f);
    }
    
    TEST(JsonValueTests, AsFloat64)
    {
        const JsonValue JsonValue(1.0);
    
        EXPECT_DOUBLE_EQ(JsonValue.AsFloat64(), 1.0);
    }
    
    TEST(JsonValueTests, AsFString)
    {
        const JsonValue JsonValue("test");
    
        EXPECT_EQ(JsonValue.AsFString(), "test");
    }
    
    TEST(JsonValueTests, AsArray)
    {
        const JsonValue JsonValue(JsonArray{});
    
        EXPECT_TRUE(JsonValue.AsArray().IsEmpty());
    }
    
    TEST(JsonValueTests, AsObject)
    {
        JsonValue JsonValue(JsonObject{});
    
        EXPECT_TRUE(JsonValue.AsObject().empty());
    }

    TEST(JsonValueTests, AsArrayReturnsStoredValues)
    {
        JsonArray Array;

        Array.Emplace(true);
        Array.Emplace(1.0);
        Array.Emplace("test");

        const JsonValue JsonValue(Array);

        EXPECT_EQ(JsonValue.AsArray().Size(), 3);
        EXPECT_EQ(JsonValue.AsArray()[0].AsBoolean(), true);
        EXPECT_DOUBLE_EQ(JsonValue.AsArray()[1].AsFloat64(), 1.0);
        EXPECT_EQ(JsonValue.AsArray()[2].AsFString(), "test");
    }

    TEST(JsonValueTests, AsObjectReturnsStoredValues)
    {
        JsonObject Object;
        Object.emplace("name", JsonValue("Front Entrance"));

        const JsonValue JsonValue(std::move(Object));

        const JsonObject& Result = JsonValue.AsObject();

        ASSERT_EQ(Result.size(), 1);
        EXPECT_EQ(Result.at("name").AsFString(), "Front Entrance");
    }
}