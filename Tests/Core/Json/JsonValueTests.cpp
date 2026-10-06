#include <gtest/gtest.h>

#include "Core/Json/JsonValue.h"

namespace AxiomVms::Tests
{
    TEST(JsonValueTests, EmptyConstructorIsNullPointer)
    {
        const JsonValue jsonValue;
        EXPECT_EQ(jsonValue.IsNull(), true);
    }

    TEST(JsonValueTests, NullPointerConstructorIsNull)
    {
        const JsonValue jsonValue(nullptr);
        EXPECT_EQ(jsonValue.IsNull(), true);
    }

    TEST(JsonValueTests, BooleanConstructorIsBoolean)
    {
        const JsonValue jsonValue(true);
        EXPECT_EQ(jsonValue.IsBoolean(), true);
    }

    TEST(JsonValueTests, Float32ConstructorIsNumber)
    {
        const JsonValue jsonValue(1.0f);
        EXPECT_EQ(jsonValue.IsNumber(), true);
    }

    TEST(JsonValueTests, Float64ConstructorIsNumber)
    {
        const JsonValue jsonValue(1.0);
        EXPECT_EQ(jsonValue.IsNumber(), true);
    }

    TEST(JsonValueTests, StringConstructorIsString)
    {
        const JsonValue jsonValue("test");
        EXPECT_EQ(jsonValue.IsString(), true);
    }
    
    TEST(JsonValueTests, ArrayConstructorIsArray)
    {
        const JsonValue jsonValue(JsonArray{});
        EXPECT_EQ(jsonValue.IsArray(), true);
    }

    TEST(JsonValueTests, ObjectConstructorIsObject)
    {
        const JsonValue jsonValue(JsonObject{});

        EXPECT_EQ(jsonValue.IsObject(), true);
    }

    TEST(JsonValueTests, AsBoolean)
    {
        const JsonValue jsonValue(true);
    
        EXPECT_TRUE(jsonValue.AsBoolean());
    }
    
    TEST(JsonValueTests, AsFloat32)
    {
        const JsonValue jsonValue(1.0f);
    
        EXPECT_FLOAT_EQ(jsonValue.AsFloat32(), 1.0f);
    }
    
    TEST(JsonValueTests, AsFloat64)
    {
        const JsonValue jsonValue(1.0);
    
        EXPECT_DOUBLE_EQ(jsonValue.AsFloat64(), 1.0);
    }
    
    TEST(JsonValueTests, AsFString)
    {
        const JsonValue jsonValue("test");
    
        EXPECT_EQ(jsonValue.AsFString(), "test");
    }
    
    TEST(JsonValueTests, AsArray)
    {
        const JsonValue jsonValue(JsonArray{});
    
        EXPECT_TRUE(jsonValue.AsArray().empty());
    }
    
    TEST(JsonValueTests, AsObject)
    {
        JsonValue jsonValue(JsonObject{});
    
        EXPECT_TRUE(jsonValue.AsObject().empty());
    }

    TEST(JsonValueTests, AsArrayReturnsStoredValues)
    {
        JsonArray array;

        array.emplace_back(true);
        array.emplace_back(1.0);
        array.emplace_back("test");

        const JsonValue jsonValue(array);

        EXPECT_EQ(jsonValue.AsArray().size(), 3);
        EXPECT_EQ(jsonValue.AsArray()[0].AsBoolean(), true);
        EXPECT_DOUBLE_EQ(jsonValue.AsArray()[1].AsFloat64(), 1.0);
        EXPECT_EQ(jsonValue.AsArray()[2].AsFString(), "test");
    }

    TEST(JsonValueTests, AsObjectReturnsStoredValues)
    {
        JsonObject object;
        object.emplace("name", JsonValue("Front Entrance"));

        const JsonValue jsonValue(std::move(object));

        const JsonObject& result = jsonValue.AsObject();

        ASSERT_EQ(result.size(), 1);
        EXPECT_EQ(result.at("name").AsFString(), "Front Entrance");
    }
}