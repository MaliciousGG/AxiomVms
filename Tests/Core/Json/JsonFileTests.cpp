#include <gtest/gtest.h>

#include "CoreMinimal.h"
#include "Core/Json/JsonFile.h"

namespace AxiomVms::Tests
{
    TEST(JsonFileTests, JsonFileLoads)
    {
        JsonFile file;
        FString error;

        ASSERT_TRUE(file.Load("test.json", error)) << "Failed to load test.json: " << error;
    }

    TEST(JsonFileTests, JsonFileContainsExpectedData_1)
    {
        JsonFile file;
        FString error;

        ASSERT_TRUE(file.Load("JsonFileTestData/JsonFileContainsExpectedData_1.json", error)) << "Failed to load test.json: " << error;

        const JsonValue& root = file.GetRoot();

        ASSERT_TRUE(root.IsObject()) << "Expected a JSON object.";

        struct ExpectedData
        {
            FString Key;
            FString Value;
        };

        const ExpectedData expectedData[] =
        {
            {
                .Key = "name",
                .Value = "Test Camera"
            }
        };

        const JsonObject& object = root.AsObject();

        for (const auto&[Key, Value] : expectedData)
        {
            const auto entry = object.find(Key);

            ASSERT_NE(entry, object.end()) << "Missing key: " << Key;

            ASSERT_TRUE(entry->second.IsString()) << "Expected a string for key: " << Key;

            EXPECT_EQ(entry->second.AsFString(), Value) << "Unexpected value for key: " << Key;
        }
    }
}
