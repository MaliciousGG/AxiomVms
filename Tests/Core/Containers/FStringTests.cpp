#include "Core/Containers/FString.h"

#include <gtest/gtest.h>

namespace Axiom::Tests
{
    using namespace Axiom;

    TEST(P_StringTests, DefaultConstructorContainsNullTerminator)
    {
        const P_String String;
        ASSERT_EQ(String.Size(), 1);
        ASSERT_EQ(*String.Get(), L'\0');
    }

    TEST(P_StringTests, DefaultConstructedIsEmpty)
    {
        const P_String String;
        ASSERT_TRUE(String.IsEmpty());
    }

    TEST(P_StringTests, TCHARConstructor)
    {
        P_String String("Hello");
        ASSERT_EQ(String.Size(), 6);
        ASSERT_EQ(String.Length(), 5);
        ASSERT_EQ(*String.Get(), L'H');
        ASSERT_EQ(String.Get()[5], L'\0');
    }

    TEST(P_StringTests, TCHARAppendToExistingP_String)
    {
        P_String String("Hello");
        String.Append("World");
        ASSERT_EQ(String.Size(), 11);
        ASSERT_EQ(String.Length(), 10);
        ASSERT_EQ(*String.Get(), L'H');
        ASSERT_EQ(String.Get()[10], L'\0');

        for (Size_T i = 0; i < String.Length(); ++i)
        {
            ASSERT_FALSE(String.Get()[i] == L'\0');
        }
    }

    TEST(P_StringTests, OperatorAtIndexReturnsTCHARSTAR)
    {
        const P_String String("Hello");
        ASSERT_EQ(*String[0], L'H');
        ASSERT_EQ(*String[1], L'e');
        ASSERT_EQ(*String[2], L'l');
        ASSERT_EQ(*String[3], L'l');
        ASSERT_EQ(*String[4], L'o');
        ASSERT_EQ(*String[5], L'\0');
    }
}