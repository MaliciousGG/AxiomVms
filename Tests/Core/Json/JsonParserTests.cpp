#include <gtest/gtest.h>

#include "Core/Json/JsonParser.h"

namespace AxiomVms::Tests
{
    TEST(JsonParserTests, EmptyInputReturnsEmptyValue)
    {
        const FString input;

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_EQ(parser.GetErrors().size(), 1);
        EXPECT_EQ(parser.GetErrors()[0].Message, "Input is empty");
    }

    TEST(JsonParserTests, EmptyArrayReturnsEmptyArray)
    {
        const FString input = "[]";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsArray());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, EmptyObjectReturnsEmptyObject)
    {
        const FString input = "{}";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsObject());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, StringValueReturnsString)
    {
        const FString input = "\"Hello World\"";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsString());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, NumberReturnsNumber)
    {
        const FString input = "67";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNumber());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, NegativeNumberReturnsNumber)
    {
        const FString input = "-67";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNumber());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, DecimalNumberReturnsNumber)
    {
        const FString input = "67.5";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNumber());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, TrueReturnsBoolean)
    {
        const FString input = "true";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsBoolean());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, FalseReturnsBoolean)
    {
        const FString input = "false";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsBoolean());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, NullReturnsNull)
    {
        const FString input = "null";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, WhitespaceBeforeValueIsIgnored)
    {
        const FString input = " 67";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNumber());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, WhitespaceAfterValueIsIgnored)
    {
        const FString input = "67 ";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNumber());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, NewLinesAndTabsAreIgnored)
    {
        const FString input = "\n\t\r  true";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsBoolean());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, SimpleArrayReturnsArray)
    {
        const FString input = "[1, 2, 3]";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsArray());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, MixedArrayReturnsArray)
    {
        const FString input = "[1, \"Test\", true, null]";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsArray());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, NestedArrayReturnsArray)
    {
        const FString input = "[[1, 2], [3, 4]]";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsArray());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, SimpleObjectReturnsObject)
    {
        const FString input = "{\"Item\": 0}";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsObject());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, ObjectWithMultipleValuesReturnsObject)
    {
        const FString input =
            R"({"Name": "Camera", "Id": 1, "Enabled": true})";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsObject());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, NestedObjectReturnsObject)
    {
        const FString input =
            R"({"Camera": {"Id": 1, "Name": "Front Door"}})";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsObject());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, ObjectContainingArrayReturnsObject)
    {
        const FString input = "{\"Items\": [1, 2, 3]}";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsObject());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, ArrayOfObjectsReturnsArray)
    {
        const FString input = R"([{"Item": 0}, {"Item": 1}])";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsArray());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, EscapedQuoteInStringReturnsString)
    {
        const FString input = R"("Hello \"World\"")";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsString());
        EXPECT_EQ(parser.GetErrors().size(), 0);
    }

    TEST(JsonParserTests, MalformedJsonReturnsError_1)
    {
        const FString input = "{";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_EQ(parser.GetErrors().size(), 1);
        EXPECT_EQ(parser.GetErrors()[0].Message, FString("Unexpected end of input while parsing Json object"));
    }

    TEST(JsonParserTests, MalformedJsonReturnsError_2)
    {
        const FString input = "{}'";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_EQ(parser.GetErrors().size(), 1);
        EXPECT_EQ(parser.GetErrors()[0].Message, FString("Unexpected trailing characters after Json value"));
    }

    TEST(JsonParserTests, ObjectMissingColonReturnsError)
    {
        const FString input = "{\"Item\" 0}";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_EQ(parser.GetErrors().size(), 2);

    }

    TEST(JsonParserTests, ObjectMissingValueReturnsError)
    {
        const FString input = "{\"Item\":}";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_EQ(parser.GetErrors().size(), 2);
    }

    TEST(JsonParserTests, ObjectMissingClosingBraceReturnsError)
    {
        const FString input = "{\"Item\": 0";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_EQ(parser.GetErrors().size(), 1);
    }

    TEST(JsonParserTests, ArrayMissingClosingBracketReturnsError)
    {
        const FString input = "[1, 2, 3";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_EQ(parser.GetErrors().size(), 1);
    }

    TEST(JsonParserTests, TrailingCommaInArrayReturnsError)
    {
        const FString input = "[1, 2, 3,]";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_FALSE(parser.GetErrors().empty());
    }

    TEST(JsonParserTests, TrailingCommaInObjectReturnsError)
    {
        const FString input = "{\"Item\": 0,}";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_FALSE(parser.GetErrors().empty());
    }

    TEST(JsonParserTests, InvalidBooleanReturnsError)
    {
        const FString input = "tru";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_FALSE(parser.GetErrors().empty());
    }

    TEST(JsonParserTests, InvalidNullReturnsError)
    {
        const FString input = "nul";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_FALSE(parser.GetErrors().empty());
    }

    TEST(JsonParserTests, InvalidCharacterReturnsError)
    {
        const FString input = "@";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_FALSE(parser.GetErrors().empty());
    }

    TEST(JsonParserTests, UnterminatedStringReturnsError)
    {
        const FString input = "\"Hello World";

        JsonParser parser;
        const JsonValue value = parser.Parse(input);

        EXPECT_TRUE(value.IsNull());
        EXPECT_FALSE(parser.GetErrors().empty());
    }
}