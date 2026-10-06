#include "Core/Json/JsonParser.h"
#include "Core/Json/JsonParseError.h"
#include "Core/Json/JsonSourceLocation.h"
#include "Core/Types.h"
#include "Core/Logging/Log.h"

#include <cctype>
#include <cstdlib>

namespace AxiomVms
{
    JsonValue JsonParser::Parse(const FString& input)
    {
        Reset();

        Input_ = input;

        if (Input_.empty())
        {
            AddError({ 
                .Message = "Input is empty", 
                .Location = Location_ 
            });

            LogErrors();

            return {};
        }

        SkipWhitespace();

        JsonValue value = ParseValue();

        SkipWhitespace();

        if (!IsAtEnd())
        {
            AddError({ .Message = "Unexpected trailing characters after Json value", .Location = Location_ });
        }

        if (!Errors_.empty())
        {
            LogErrors();

            return {};
        }

        return value;
    }


    JsonValue JsonParser::ParseValue()
    {
        SkipWhitespace();

        if (IsAtEnd())
        {
            AddError({ 
                .Message = "Unexpected end of input while parsing value", 
                .Location = Location_
            });

            return {};
        }

        const TCHAR currentChar = Current();

        switch (currentChar)
        {
            case '{':
                return JsonValue(ParseObject());

            case '[':
                return JsonValue(ParseArray());

            case '"':
                return JsonValue(ParseString());

            case 't':
            case 'f':
                return JsonValue(ParseBoolean());

            case 'n':
                ParseNull();
                return JsonValue{};

            default:
                break;
        }

        if (currentChar == '-' || std::isdigit(static_cast<unsigned char>(currentChar)))
        {
            return JsonValue(ParseNumber());
        }

        AddError({
            .Message = FString("Unexpected character while parsing Json value: '") + currentChar + "'",
            .Location = Location_
        });

        return {};
    }


    JsonObject JsonParser::ParseObject()
    {
        JsonObject object;

        Next();

        SkipWhitespace();

        if (Current() == '}')
        {
            Next();

            return object;
        }

        while (!IsAtEnd())
        {
            SkipWhitespace();

            if (Current() != '"')
            {
                AddError({ 
                    .Message = "Expected string key in Json object", 
                    .Location = Location_ });

                return {};
            }

            const FString key = ParseString();

            SkipWhitespace();

            if (Current() != ':')
            {
                AddError({ 
                    .Message = "Expected ':' after Json object key", 
                    .Location = Location_ });

                return {};
            }

            Next();

            SkipWhitespace();

            JsonValue value = ParseValue();

            if (!Errors_.empty())
            {
                return {};
            }

            object[key] = value;

            SkipWhitespace();

            if (Current() == '}')
            {
                Next();

                return object;
            }

            if (Current() != ',')
            {
                AddError({ 
                    .Message = "Expected ',' or '}' after Json object value", 
                    .Location = Location_ 
                });

                return {};
            }

            Next();

            SkipWhitespace();
        }

        AddError({ 
            .Message = "Unexpected end of input while parsing Json object",
            .Location = Location_
        });

        return {};
    }


    JsonArray JsonParser::ParseArray()
    {
        JsonArray array;

        Next();

        SkipWhitespace();

        if (Current() == ']')
        {
            Next();

            return array;
        }

        while (!IsAtEnd())
        {
            JsonValue value = ParseValue();

            if (!Errors_.empty())
            {
                return {};
            }

            array.emplace_back(value);

            SkipWhitespace();

            if (Current() == ']')
            {
                Next();

                return array;
            }

            if (Current() != ',')
            {
                AddError({ 
                    .Message = "Expected ',' or ']' after Json array value", 
                    .Location = Location_ 
                });

                return {};
            }

            Next();

            SkipWhitespace();
        }

        AddError({ 
            .Message = "Unexpected end of input while parsing Json array", 
            .Location = Location_ 
        });

        return {};
    }


    FString JsonParser::ParseString()
    {
        FString result;

        if (Current() != '"')
        {
            AddError({ 
                .Message = "Expected '\"' at start of Json string", 
                .Location = Location_
            });

            return {};
        }

        Next();

        while (!IsAtEnd())
        {
            const TCHAR currentChar = Next();

            if (currentChar == '"')
            {
                return result;
            }

            if (currentChar == '\\')
            {
                if (IsAtEnd())
                {
                    AddError({
                        .Message = "Unexpected end of input after escape character",
                        .Location = Location_
                    });

                    return {};
                }

                switch (const TCHAR escapedChar = Next())
                {
                    case '"':
                        result += '"';
                        break;

                    case '\\':
                        result += '\\';
                        break;

                    case '/':
                        result += '/';
                        break;

                    case 'b':
                        result += '\b';
                        break;

                    case 'f':
                        result += '\f';
                        break;

                    case 'n':
                        result += '\n';
                        break;

                    case 'r':
                        result += '\r';
                        break;

                    case 't':
                        result += '\t';
                        break;

                    default:
                        AddError({
                            .Message = FString("Invalid escape sequence: \\") + escapedChar,
                            .Location = Location_
                        });

                        return {};
                }

                continue;
            }

            if (currentChar == '\n' || currentChar == '\r')
            {
                AddError({
                    .Message = "Unescaped newline in Json string",
                    .Location = Location_
                });

                return {};
            }

            result += currentChar;
        }

        AddError({
            .Message = "Unterminated Json string",
            .Location = Location_
        });

        return {};
    }


    float64 JsonParser::ParseNumber()
    {
        const Size_T start = Location_.Offset;

        if (Current() == '-')
        {
            Next();
        }

        if (IsAtEnd())
        {
            AddError({
                .Message = "Unexpected end of input while parsing number",
                .Location = Location_
            });

            return {};
        }

        if (Current() == '0')
        {
            Next();
        }
        else
        {
            if (!std::isdigit(static_cast<unsigned char>(Current())))
            {
                AddError({
                    .Message = "Expected digit while parsing Json number",
                    .Location = Location_
                });

                return {};
            }

            while (!IsAtEnd() &&
                   std::isdigit(static_cast<unsigned char>(Current())))
            {
                Next();
            }
        }

        if (!IsAtEnd() && Current() == '.')
        {
            Next();

            if (IsAtEnd() || !std::isdigit(static_cast<unsigned char>(Current())))
            {
                AddError({
                    .Message = "Expected digit after decimal point",
                    .Location = Location_
                });

                return {};
            }

            while (!IsAtEnd() && std::isdigit(static_cast<unsigned char>(Current())))
            {
                Next();
            }
        }

        if (!IsAtEnd() && (Current() == 'e' || Current() == 'E'))
        {
            Next();

            if (!IsAtEnd() && (Current() == '+' || Current() == '-'))
            {
                Next();
            }

            if (IsAtEnd() || !std::isdigit(static_cast<unsigned char>(Current())))
            {
                AddError({
                    .Message = "Expected digit in Json number exponent",
                    .Location = Location_
                });

                return {};
            }

            while (!IsAtEnd() && std::isdigit(static_cast<unsigned char>(Current())))
            {
                Next();
            }
        }

        const Size_T length = Location_.Offset - start;

        const FString numberString{Input_.substr(start, length)};

        return std::strtod(numberString.c_str(), nullptr);
    }


    bool JsonParser::ParseBoolean()
    {
        if (Input_.substr(Location_.Offset, 4) == "true")
        {
            Next();
            Next();
            Next();
            Next();

            return true;
        }

        if (Input_.substr(Location_.Offset, 5) == "false")
        {
            Next();
            Next();
            Next();
            Next();
            Next();

            return false;
        }

        AddError({
            .Message = "Invalid Json boolean",
            .Location = Location_
        });

        return false;
    }


    void JsonParser::ParseNull()
    {
        if (Input_.substr(Location_.Offset, 4) == "null")
        {
            Next();
            Next();
            Next();
            Next();

            return;
        }

        AddError({
            .Message = "Invalid Json null value",
            .Location = Location_
        });
    }


    void JsonParser::SkipWhitespace()
    {
        while (!IsAtEnd())
        {
            switch (Current())
            {
                case ' ':
                case '\t':
                case '\n':
                case '\r':
                    Next();
                    break;

                default:
                    return;
            }
        }
    }


    TCHAR JsonParser::Current() const
    {
        if (IsAtEnd())
        {
            return {};
        }

        return Input_[Location_.Offset];
    }


    TCHAR JsonParser::Next()
    {
        if (IsAtEnd())
        {
            return {};
        }

        const TCHAR currentChar = Input_[Location_.Offset];

        ++Location_.Offset;

        if (currentChar == '\n')
        {
            ++Location_.Line;
            Location_.Column = 1;
        }
        else
        {
            ++Location_.Column;
        }

        return currentChar;
    }


    bool JsonParser::IsAtEnd() const
    {
        return Location_.Offset >= Input_.length();
    }


    std::string_view JsonParser::GetInput() const
    {
        return Input_;
    }


    JsonSourceLocation JsonParser::GetLocation() const
    {
        return Location_;
    }


    TArray<JsonParseError> JsonParser::GetErrors() const
    {
        return Errors_;
    }


    void JsonParser::LogErrors()
    {
        if (Errors_.empty())
        {
            return;
        }

        AXIOM_LOG_ERROR("AxiomVms encountered {} errors while parsing Json: {}", Errors_.size(), Input_);

        for (const JsonParseError& error : Errors_)
        {
            AXIOM_LOG_ERROR("Error at line {} column {}: {}", error.Location.Line, error.Location.Column, error.Message);
        }
    }


    void JsonParser::AddError(const JsonParseError& error)
    {
        if (error.Message.empty())
        {
            return;
        }

        Errors_.emplace_back(error);
    }


    void JsonParser::Reset()
    {
        Input_ = {};
        Location_ = {};
        Errors_.clear();
    }
}