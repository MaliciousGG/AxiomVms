#include "Core/Json/JsonParser.h"
#include "Core/Json/JsonParseError.h"
#include "Core/Json/JsonSourceLocation.h"
#include "Core/Types.h"
#include "Core/Logging/Log.h"

#include <cctype>
#include <cstdlib>

namespace Axiom
{
    JsonValue JsonParser::Parse(const FString& Input)
    {
        Reset();

        Input_ = Input;

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

        JsonValue Value = ParseValue();

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

        return Value;
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

        const TCHAR CurrentChar = Current();

        switch (CurrentChar)
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

        if (CurrentChar == '-' || std::isdigit(static_cast<unsigned char>(CurrentChar)))
        {
            return JsonValue(ParseNumber());
        }

        AddError({
            .Message = FString("Unexpected character while parsing Json value: '") + CurrentChar + "'",
            .Location = Location_
        });

        return {};
    }


    JsonObject JsonParser::ParseObject()
    {
        JsonObject Object;

        Next();

        SkipWhitespace();

        if (Current() == '}')
        {
            Next();

            return Object;
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

            const FString Key = ParseString();

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

            const JsonValue Value = ParseValue();

            if (!Errors_.empty())
            {
                return {};
            }

            Object[Key] = Value;

            SkipWhitespace();

            if (Current() == '}')
            {
                Next();

                return Object;
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
        JsonArray Array;

        Next();

        SkipWhitespace();

        if (Current() == ']')
        {
            Next();

            return Array;
        }

        while (!IsAtEnd())
        {
            JsonValue Value = ParseValue();

            if (!Errors_.empty())
            {
                return {};
            }

            Array.emplace_back(Value);

            SkipWhitespace();

            if (Current() == ']')
            {
                Next();

                return Array;
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
        FString Result;

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
            const TCHAR CurrentChar = Next();

            if (CurrentChar == '"')
            {
                return Result;
            }

            if (CurrentChar == '\\')
            {
                if (IsAtEnd())
                {
                    AddError({
                        .Message = "Unexpected end of input after escape character",
                        .Location = Location_
                    });

                    return {};
                }

                switch (const TCHAR EscapedChar = Next())
                {
                    case '"':
                        Result += '"';
                        break;

                    case '\\':
                        Result += '\\';
                        break;

                    case '/':
                        Result += '/';
                        break;

                    case 'b':
                        Result += '\b';
                        break;

                    case 'f':
                        Result += '\f';
                        break;

                    case 'n':
                        Result += '\n';
                        break;

                    case 'r':
                        Result += '\r';
                        break;

                    case 't':
                        Result += '\t';
                        break;

                    default:
                        AddError({
                            .Message = FString("Invalid escape sequence: \\") + EscapedChar,
                            .Location = Location_
                        });

                        return {};
                }

                continue;
            }

            if (CurrentChar == '\n' || CurrentChar == '\r')
            {
                AddError({
                    .Message = "Unescaped newline in Json string",
                    .Location = Location_
                });

                return {};
            }

            Result += CurrentChar;
        }

        AddError({
            .Message = "Unterminated Json string",
            .Location = Location_
        });

        return {};
    }


    float64 JsonParser::ParseNumber()
    {
        const Size_T Start = Location_.Offset;

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

            while (!IsAtEnd() && std::isdigit(static_cast<unsigned char>(Current())))
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

        const Size_T Length = Location_.Offset - Start;

        const FString NumberString{Input_.substr(Start, Length)};

        return std::strtod(NumberString.c_str(), nullptr);
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

        const TCHAR CurrentChar = Input_[Location_.Offset];

        ++Location_.Offset;

        if (CurrentChar == '\n')
        {
            ++Location_.Line;
            Location_.Column = 1;
        }
        else
        {
            ++Location_.Column;
        }

        return CurrentChar;
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

        for (const auto& [Message, Location] : Errors_)
        {
            AXIOM_LOG_ERROR("Error at line {} column {}: {}", Location.Line, Location.Column, Message);
        }
    }


    void JsonParser::AddError(const JsonParseError& Error)
    {
        if (Error.Message.empty())
        {
            return;
        }

        Errors_.emplace_back(Error);
    }


    void JsonParser::Reset()
    {
        Input_ = {};
        Location_ = {};
        Errors_.clear();
    }
}