#include "Core/Containers/FString.h"

#include <algorithm>
#include <cctype>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <string>
#include <utility>

namespace Axiom
{
    namespace
    {
        constexpr Size_T MaxLength = P_STRING_MAX_LENGTH;

        void CheckLength(const Size_T Length)
        {
            if (Length > MaxLength)
            {
                throw std::length_error("P_String exceeds P_STRING_MAX_LENGTH");
            }
        }

        // The input must point to a valid null-terminated string.
        std::string ReadText(const TCHAR* Text)
        {
            if (!Text)
            {
                return {};
            }

            Size_T Length = 0;
            while (Text[Length] != '\0')
            {
                if (Length == MaxLength)
                {
                    throw std::length_error("P_String exceeds P_STRING_MAX_LENGTH");
                }

                ++Length;
            }

            return std::string(Text, Length);
        }

        void CheckCharacter(const TCHAR Character, const Size_T Count)
        {
            if (Count && Character == '\0')
            {
                throw std::invalid_argument("Embedded null characters are not supported");
            }
        }

        TCHAR Lower(const TCHAR Character)
        {
            return static_cast<TCHAR>(std::tolower(static_cast<unsigned char>(Character)));
        }

        TCHAR Upper(const TCHAR Character)
        {
            return static_cast<TCHAR>(std::toupper(static_cast<unsigned char>(Character)));
        }

        bool IsSpace(const TCHAR Character)
        {
            return std::isspace(static_cast<unsigned char>(Character)) != 0;
        }
    }

    P_String::P_String()
    {
        EnsureNullTerminator();
    }

    P_String::P_String(const TCHAR* InValue)
    {
        Set(InValue);
    }

    P_String::~P_String()
    {
        delete[] Data_;
    }

    P_String::P_String(const P_String& Other)
    {
        Set(Other);
    }

    P_String::P_String(P_String&& Other) noexcept
        : Data_(std::exchange(Other.Data_, nullptr)),
          Length_(std::exchange(Other.Length_, 0)),
          Size_(std::exchange(Other.Size_, 0))
    {

    }

    P_String& P_String::operator=(const P_String& Other)
    {
        if (this != &Other)
        {
            Set(Other);
        }

        return *this;
    }

    P_String& P_String::operator=(P_String&& Other) noexcept
    {
        if (this != &Other)
        {
            delete[] Data_;

            Data_ = std::exchange(Other.Data_, nullptr);
            Length_ = std::exchange(Other.Length_, 0);
            Size_ = std::exchange(Other.Size_, 0);
        }

        return *this;
    }

    P_String& P_String::operator=(const TCHAR* Other)
    {
        Set(Other);

        return *this;
    }

    TCHAR* P_String::operator[](const Size_T Index) const noexcept
    {
        return &Data_[Index];
    }

    void P_String::Clear()
    {
        Reserve(1);

        Length_ = 0;
        Data_[0] = '\0';
    }

    void P_String::Append(const TCHAR* Other)
    {
        Insert(Length_, Other);
    }

    void P_String::Append(const P_String& Other)
    {
        Append(Other.CStr());
    }

    void P_String::Append(const TCHAR Character)
    {
        Append(1, Character);
    }

    void P_String::Append(const Size_T Count, const TCHAR Character)
    {
        Insert(Length_, Count, Character);
    }

    void P_String::Remove(const Size_T Index, const Size_T Count)
    {
        Replace(Index, Count, nullptr);
    }

    void P_String::Remove(const Size_T Index)
    {
        Remove(Index, 1);
    }

    void P_String::Insert(const Size_T Index, const TCHAR* Other)
    {
        Replace(Index, 0, Other);
    }

    void P_String::Insert(const Size_T Index, const P_String& Other)
    {
        Insert(Index, Other.CStr());
    }

    void P_String::Insert(const Size_T Index, const TCHAR Character)
    {
        Insert(Index, 1, Character);
    }

    void P_String::Insert(const Size_T Index, const Size_T Count, const TCHAR Character)
    {
        CheckLength(Count);
        CheckCharacter(Character, Count);

        const std::string Text(Count, Character);
        Insert(Index, Text.c_str());
    }

    void P_String::Replace(const Size_T Index, const Size_T Count, const TCHAR* Other)
    {
        if (Index > Length_)
        {
            throw std::out_of_range("P_String index is out of range");
        }


        // Snapshot before reallocating or moving data: other may alias Data_.
        const std::string Text = ReadText(Other);
        const Size_T Removed = std::min(Count, Length_ - Index);

        const Size_T Retained = Length_ - Removed;

        if (Text.size() > MaxLength - Retained)
        {
            throw std::length_error("P_String exceeds P_STRING_MAX_LENGTH");
        }

        const Size_T NewLength = Retained + Text.size();

        Reserve(NewLength + 1);

        std::memmove(Data_ + Index + Text.size(), Data_ + Index + Removed,
                     (Length_ - Index - Removed + 1) * sizeof(TCHAR));

        if (!Text.empty())
        {
            std::memcpy(Data_ + Index, Text.data(), Text.size() * sizeof(TCHAR));
        }

        Length_ = NewLength;
        Data_[Length_] = '\0';
    }

    void P_String::Replace(const Size_T Index, const Size_T Count, const P_String& Other)
    {
        Replace(Index, Count, Other.CStr());
    }

    void P_String::Replace(const Size_T Index, const TCHAR Character) const
    {
        Replace(Index, 1, Character);
    }

    void P_String::Replace(const Size_T Index, Size_T Count, TCHAR Character) const
    {
        // Replace count existing characters with count copies of character.
        if (Index > Length_)
        {
            throw std::out_of_range("P_String index is out of range");
        }

        Count = std::min(Count, Length_ - Index);

        CheckCharacter(Character, Count);

        if (Count)
        {
            std::fill_n(Data_ + Index, Count, Character);
        }
    }

    void P_String::Set(const TCHAR* Other)
    {
        const std::string Text = ReadText(Other);

        Reserve(Text.size() + 1);

        if (!Text.empty())
        {
            std::memcpy(Data_, Text.data(), Text.size() * sizeof(TCHAR));
        }

        Length_ = Text.size();
        Data_[Length_] = '\0';
    }

    void P_String::Set(const P_String& Other)
    {
        Set(Other.CStr());
    }

    void P_String::Set(const TCHAR Character)
    {
        Set(1, Character);
    }

    void P_String::Set(const Size_T Count, const TCHAR Character)
    {
        CheckLength(Count);
        CheckCharacter(Character, Count);

        Reserve(Count + 1);
        std::fill_n(Data_, Count, Character);

        Length_ = Count;
        Data_[Length_] = '\0';
    }

    void P_String::SetLength(const Size_T Length)
    {
        CheckLength(Length);
        Reserve(Length + 1);

        if (Length > Length_)
        {
            std::fill(Data_ + Length_, Data_ + Length, ' ');
        }

        Length_ = Length;
        Data_[Length_] = '\0';
    }

    void P_String::SetSize(Size_T Size)
    {
        Size = std::max<Size_T>(Size, 1);

        if (Size > MaxLength + 1)
        {
            throw std::length_error("P_String capacity exceeds the maximum");
        }

        if (Size == Size_)
        {
            return;
        }

        const Size_T Length = std::min(Length_, Size - 1);

        auto Data = std::make_unique<TCHAR[]>(Size);

        if (Length)
        {
            std::copy_n(Data_, Length, Data.get());
        }

        Data[Length] = '\0';
        delete[] Data_;

        Data_ = Data.release();
        Size_ = Size;
        Length_ = Length;
    }

    void P_String::Resize(const Size_T Size)
    {
        SetLength(Size);
    }

    void P_String::Shrink()
    {
        SetSize(Length_ + 1);
    }

    void P_String::Reserve(Size_T Size)
    {
        Size = std::max<Size_T>(Size, 1);

        if (Size > MaxLength + 1)
        {
            throw std::length_error("P_String capacity exceeds the maximum");
        }

        if (Size > Size_)
        {
            SetSize(Size);
        }
    }

    void P_String::Trim()
    {
        TrimRight();
        TrimLeft();
    }

    void P_String::TrimLeft()
    {
        Size_T Count = 0;

        while (Count < Length_ && IsSpace(Data_[Count]))
        {
            ++Count;
        }

        if (Count)
        {
            Remove(0, Count);
        }
    }

    void P_String::TrimRight()
    {
        while (Length_ && IsSpace(Data_[Length_ - 1]))
        {
            --Length_;
        }

        EnsureNullTerminator();
    }

    void P_String::ToLower() const
    {
        for (Size_T i = 0; i < Length_; ++i)
        {
            Data_[i] = Lower(Data_[i]);
        }
    }

    void P_String::ToUpper() const
    {
        for (Size_T i = 0; i < Length_; ++i)
        {
            Data_[i] = Upper(Data_[i]);
        }
    }

    void P_String::Capitalize() const
    {
        // Uppercase the first character, lowercase the remaining characters.
        ToLower();

        if (Length_)
        {
            Data_[0] = Upper(Data_[0]);
        }
    }

    void P_String::Reverse() const
    {
        if (Length_)
        {
            std::reverse(Data_, Data_ + Length_);
        }
    }

    void P_String::Swap(P_String& Other)
    {
        std::swap(Data_, Other.Data_);
        std::swap(Length_, Other.Length_);
        std::swap(Size_, Other.Size_);
    }

    bool P_String::Equals(const P_String& Other) const
    {
        return Length_ == Other.Length_ &&
               (Length_ == 0 || std::equal(Data_, Data_ + Length_, Other.Data_));
    }

    bool P_String::Equals(const TCHAR* Other) const
    {
        return Equals(Other, Other ? std::strlen(Other) : 0);
    }

    bool P_String::Equals(const TCHAR* Other, const Size_T Length) const
    {
        // Explicit length means compare against exactly that many readable characters.
        if (Length_ != Length)
        {
            return false;
        }

        if (!Length)
        {
            return true;
        }

        return Other && std::equal(Data_, Data_ + Length_, Other);
    }

    bool P_String::EqualsIgnoreCase(const P_String& Other) const
    {
        return EqualsIgnoreCase(Other.CStr(), Other.Length_);
    }

    bool P_String::EqualsIgnoreCase(const TCHAR* Other) const
    {
        return EqualsIgnoreCase(Other, Other ? std::strlen(Other) : 0);
    }

    bool P_String::EqualsIgnoreCase(const TCHAR* Other, const Size_T Length) const
    {
        if (Length_ != Length)
        {
            return false;
        }

        if (!Length)
         {
             return true;
         }

        if (!Other)
        {
            return false;
        }

        for (Size_T i = 0; i < Length; ++i)
        {
            if (Lower(Data_[i]) != Lower(Other[i]))
            {
                return false;
            }
        }

        return true;
    }

    bool P_String::StartsWith(const P_String& Other) const
    {
        return StartsWith(Other.CStr());
    }

    bool P_String::StartsWith(const TCHAR* Other) const
    {
        const Size_T Length = Other ? std::strlen(Other) : 0;

        if (!Length)
        {
            return true;
        }

        return Length <= Length_ && std::equal(Other, Other + Length, Data_);
    }

    bool P_String::EndsWith(const P_String& Other) const
    {
        return EndsWith(Other.CStr());
    }

    bool P_String::EndsWith(const TCHAR* Other) const
    {
        const Size_T length = Other ? std::strlen(Other) : 0;

        if (!length)
        {
            return true;
        }

        return length <= Length_ && std::equal(Other, Other + length, Data_ + Length_ - length);
    }

    bool P_String::Contains(const P_String& Other) const
    {
        return Contains(Other.CStr());
    }

    bool P_String::Contains(const TCHAR* Other) const
    {
        const Size_T length = Other ? std::strlen(Other) : 0;

        if (!length)
        {
            return true;
        }

        if (length > Length_)
        {
            return false;
        }

        return std::search(Data_, Data_ + Length_, Other, Other + length) != Data_ + Length_;
    }

    bool P_String::IsEmpty() const
    {
        return Length_ == 0;
    }

    const TCHAR* P_String::Get() const
    {
        return CStr();
    }

    TCHAR* P_String::Get()
    {
        EnsureNullTerminator();

        return Data_;
    }

    const TCHAR* P_String::CStr() const
    {
        return Data_ ? Data_ : "";
    }

    TCHAR* P_String::CStr()
    {
        EnsureNullTerminator();

        return Data_;
    }

    Size_T P_String::Length() const
    {
        return Length_;
    }

    Size_T P_String::Size() const
    {
        return Size_;
    }

    void P_String::EnsureNullTerminator()
    {
        Reserve(Length_ + 1);
        Data_[Length_] = '\0';
    }
}
