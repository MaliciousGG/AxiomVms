#include "Core/Containers/TArray.h"

#include <gtest/gtest.h>

#include <cstdint>
#include <memory>

namespace AxiomVms::Tests
{
    TEST(TArrayTests, EmptyArrayIsEmpty)
    {
        const TArray<std::uint8_t> array;
        EXPECT_EQ(array.IsEmpty(), true);
    }

    TEST(TArrayTests, NonEmptyArrayIsNotEmpty)
    {
        const TArray<std::uint8_t> array{1, 2, 3};
        EXPECT_EQ(array.IsEmpty(), false);
    }

    TEST(TArrayTests, ArrayCanBeCopied)
    {
        const TArray<std::uint8_t> array{1, 2, 3};
        const TArray<std::uint8_t> copy(array);

        for (size_t i = 0; i < array.Num(); ++i)
        {
            EXPECT_EQ(array[i], copy[i]);
        }
    }

    TEST(TArrayTests, ArrayHasDifferentMemoryAddress)
    {
        const TArray<std::uint8_t> array{1, 2, 3};
        const TArray<std::uint8_t> copy(array);

        EXPECT_NE(&array, &copy);
    }

    TEST(TArrayTests, ArrayEmptyAfterInitializetion)
    {
        TArray<std::uint8_t> array{1, 2, 3};
        EXPECT_EQ(array.IsEmpty(), false);
        EXPECT_EQ(array.Num(), 3);
        EXPECT_EQ(array[0], 1);

        array.Empty();
        EXPECT_EQ(array.IsEmpty(), true);
        EXPECT_EQ(array.Num(), 0);
    }

}