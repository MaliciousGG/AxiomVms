#pragma once

#include <algorithm>
#include <cstddef>
#include <initializer_list>
#include <new>
#include <stdexcept>
#include <utility>
#include "Core/Types.h"

namespace AxiomVms
{
    template<typename T>
    class TArray
    {
    public:
        TArray();

        explicit TArray(std::initializer_list<T> list);
        ~TArray();

        TArray(const TArray& other);
        TArray(TArray&& other) noexcept;

        TArray& operator=(const TArray& other);
        TArray& operator=(TArray&& other) noexcept;

        T& operator[](std::size_t index) noexcept;
        const T& operator[](std::size_t index) const noexcept;

        T& At(std::size_t index);
        const T& At(std::size_t index) const;

        [[nodiscard]]
        std::size_t Num() const noexcept;

        [[nodiscard]]
        std::size_t Size() const noexcept;

        [[nodiscard]]
        std::size_t Max() const noexcept;

        [[nodiscard]]
        bool IsEmpty() const noexcept;

        void Swap(TArray& other) noexcept;
        void Reserve(std::size_t newCapacity) noexcept;
        void Resize(std::size_t newSize);
        void Clear() noexcept;
        void Empty() noexcept;
        void Shrink();

        template<typename... ArgumentTypes>
        T& Emplace(ArgumentTypes&&... arguments);

        void Add(const T& element);
        void Add(T&& element);

        void Pop();
        T& PopBack();
        T& Back() noexcept;

        [[nodiscard]]
        T* GetData() noexcept;

        [[nodiscard]]
        const T* GetData() const noexcept;

        T* Begin() noexcept;

        const T* Begin() const noexcept;

        const T* CBegin() const noexcept;

        T* End() noexcept;

        const T* End() const noexcept;

        const T* CEnd() const noexcept;
    private:
        T* Data_{nullptr};
        std::size_t Size_{0};
        std::size_t Capacity_{0};

        void Grow();
        void Reallocate(std::size_t newCapacity);
        void DestroyElements() noexcept;
    };
}

#include "Core/Containers/TArray.inl"
