#include <gtest/gtest.h>

#include "Core/Json/JsonParser.h"

namespace Axiom::Tests
{
    TEST(JsonParserTests, EmptyInputReturnsEmptyValue)
    {
        const FString Input;

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_EQ(Parser.GetErrors().Size(), 1);
        EXPECT_EQ(Parser.GetErrors()[0].Message, "Input is empty");
    }

    TEST(JsonParserTests, EmptyArrayReturnsEmptyArray)
    {
        const FString Input = "[]";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsArray());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, EmptyObjectReturnsEmptyObject)
    {
        const FString Input = "{}";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsObject());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, StringValueReturnsString)
    {
        const FString Input = "\"Hello World\"";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsString());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, NumberReturnsNumber)
    {
        const FString Input = "67";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNumber());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, NegativeNumberReturnsNumber)
    {
        const FString Input = "-67";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNumber());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, DecimalNumberReturnsNumber)
    {
        const FString Input = "67.5";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNumber());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, TrueReturnsBoolean)
    {
        const FString Input = "true";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsBoolean());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, FalseReturnsBoolean)
    {
        const FString Input = "false";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsBoolean());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, NullReturnsNull)
    {
        const FString Input = "null";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, WhitespaceBeforeValueIsIgnored)
    {
        const FString Input = " 67";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNumber());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, WhitespaceAfterValueIsIgnored)
    {
        const FString Input = "67 ";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNumber());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, NewLinesAndTabsAreIgnored)
    {
        const FString Input = "\n\t\r  true";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsBoolean());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, SimpleArrayReturnsArray)
    {
        const FString Input = "[1, 2, 3]";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsArray());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, MixedArrayReturnsArray)
    {
        const FString Input = "[1, \"Test\", true, null]";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsArray());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, NestedArrayReturnsArray)
    {
        const FString Input = "[[1, 2], [3, 4]]";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsArray());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, SimpleObjectReturnsObject)
    {
        const FString Input = "{\"Item\": 0}";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsObject());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, ObjectWithMultipleValuesReturnsObject)
    {
        const FString Input = R"({"Name": "Camera", "Id": 1, "Enabled": true})";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsObject());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, NestedObjectReturnsObject)
    {
        const FString Input = R"({"Camera": {"Id": 1, "Name": "Front Door"}})";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsObject());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, ObjectContainingArrayReturnsObject)
    {
        const FString Input = "{\"Items\": [1, 2, 3]}";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsObject());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, ArrayOfObjectsReturnsArray)
    {
        const FString Input = R"([{"Item": 0}, {"Item": 1}])";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsArray());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, EscapedQuoteInStringReturnsString)
    {
        const FString Input = R"("Hello \"World\"")";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsString());
        EXPECT_EQ(Parser.GetErrors().Size(), 0);
    }

    TEST(JsonParserTests, MalformedJsonReturnsError_1)
    {
        const FString Input = "{";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_EQ(Parser.GetErrors().Size(), 1);
        EXPECT_EQ(Parser.GetErrors()[0].Message, FString("Unexpected end of input while parsing Json object"));
    }

    TEST(JsonParserTests, MalformedJsonReturnsError_2)
    {
        const FString Input = "{}'";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_EQ(Parser.GetErrors().Size(), 1);
        EXPECT_EQ(Parser.GetErrors()[0].Message, FString("Unexpected trailing characters after Json value"));
    }

    TEST(JsonParserTests, ObjectMissingColonReturnsError)
    {
        const FString Input = "{\"Item\" 0}";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_EQ(Parser.GetErrors().Size(), 2);

    }

    TEST(JsonParserTests, ObjectMissingValueReturnsError)
    {
        const FString Input = "{\"Item\":}";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_EQ(Parser.GetErrors().Size(), 2);
    }

    TEST(JsonParserTests, ObjectMissingClosingBraceReturnsError)
    {
        const FString Input = "{\"Item\": 0";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_EQ(Parser.GetErrors().Size(), 1);
    }

    TEST(JsonParserTests, ArrayMissingClosingBracketReturnsError)
    {
        const FString Input = "[1, 2, 3";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_EQ(Parser.GetErrors().Size(), 1);
    }

    TEST(JsonParserTests, TrailingCommaInArrayReturnsError)
    {
        const FString Input = "[1, 2, 3,]";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_FALSE(Parser.GetErrors().IsEmpty());
    }

    TEST(JsonParserTests, TrailingCommaInObjectReturnsError)
    {
        const FString Input = "{\"Item\": 0,}";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_FALSE(Parser.GetErrors().IsEmpty());
    }

    TEST(JsonParserTests, InvalidBooleanReturnsError)
    {
        const FString Input = "tru";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_FALSE(Parser.GetErrors().IsEmpty());
    }

    TEST(JsonParserTests, InvalidNullReturnsError)
    {
        const FString Input = "nul";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_FALSE(Parser.GetErrors().IsEmpty());
    }

    TEST(JsonParserTests, InvalidCharacterReturnsError)
    {
        const FString Input = "@";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_FALSE(Parser.GetErrors().IsEmpty());
    }

    TEST(JsonParserTests, UnterminatedStringReturnsError)
    {
        const FString Input = "\"Hello World";

        JsonParser Parser;
        const JsonValue Value = Parser.Parse(Input);

        EXPECT_TRUE(Value.IsNull());
        EXPECT_FALSE(Parser.GetErrors().IsEmpty());
    }
}