#pragma once

#include <Core/Types.h>

#define P_STRING_MAX_LENGTH 1024

namespace Axiom
{
    class P_String
    {
    public:

        P_String();

        explicit P_String(const TCHAR* InValue);
        ~P_String();

        P_String(const P_String& Other);
        P_String(P_String&& Other) noexcept;

        P_String& operator=(const P_String& Other);
        P_String& operator=(P_String&& Other) noexcept;
        P_String& operator=(const TCHAR* Other);
        TCHAR* operator[](Size_T Index) const noexcept;

        void Clear();

        void Append(const TCHAR* Other);
        void Append(const P_String& Other);
        void Append(TCHAR Character);
        void Append(Size_T Count, TCHAR Character);

        void Remove(Size_T Index, Size_T Count);
        void Remove(Size_T Index);

        void Insert(Size_T Index, const TCHAR* Other);
        void Insert(Size_T Index, const P_String& Other);
        void Insert(Size_T Index, TCHAR Character);
        void Insert(Size_T Index, Size_T Count, TCHAR Character);

        void Replace(Size_T Index, Size_T Count, const TCHAR* Other);
        void Replace(Size_T Index, Size_T Count, const P_String& Other);
        void Replace(Size_T Index, TCHAR Character) const;
        void Replace(Size_T Index, Size_T Count, TCHAR Character) const;

        void Set(const TCHAR* Other);
        void Set(const P_String& Other);
        void Set(TCHAR Character);
        void Set(Size_T Count, TCHAR Character);

        void SetLength(Size_T Length);
        void SetSize(Size_T Size);

        void Resize(Size_T Size);
        void Shrink();
        void Reserve(Size_T Size);

        void Trim();
        void TrimLeft();
        void TrimRight();

        void ToLower() const;
        void ToUpper() const;

        void Capitalize() const;

        void Reverse() const;

        void Swap(P_String& Other);

        bool Equals(const P_String& Other) const;
        bool Equals(const TCHAR* Other) const;
        bool Equals(const TCHAR* Other, Size_T Length) const;

        bool EqualsIgnoreCase(const P_String& Other) const;
        bool EqualsIgnoreCase(const TCHAR* Other) const;
        bool EqualsIgnoreCase(const TCHAR* Other, Size_T Length) const;

        bool StartsWith(const P_String& Other) const;
        bool StartsWith(const TCHAR* Other) const;

        bool EndsWith(const P_String& Other) const;
        bool EndsWith(const TCHAR* Other) const;

        bool Contains(const P_String& Other) const;
        bool Contains(const TCHAR* Other) const;

        bool IsEmpty() const;

        const TCHAR* Get() const;
        TCHAR* Get();

        const TCHAR* CStr() const;
        TCHAR* CStr();

        Size_T Length() const;
        Size_T Size() const;

    private:
        void EnsureNullTerminator();

        TCHAR* Data_{};

        Size_T Length_{};
        Size_T Size_{};
    };
}
