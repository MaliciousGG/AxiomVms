#pragma once

#include <initializer_list>
#include "Core/Types.h"

namespace Axiom
{
    template<typename T>
    class TArray
    {
    public:
        TArray();

        explicit TArray(std::initializer_list<T> InList);
        ~TArray();

        TArray(const TArray& Other);
        TArray(TArray&& Other) noexcept;

        TArray& operator=(const TArray& Other);
        TArray& operator=(TArray&& Other) noexcept;

        T& operator[](Size_T Index) noexcept;
        const T& operator[](Size_T Index) const noexcept;

        T& At(Size_T Index);
        const T& At(Size_T Index) const;

        [[nodiscard]]
        uint32 Num() const noexcept;

        [[nodiscard]]
        Size_T Size() const noexcept;

        [[nodiscard]]
        Size_T Max() const noexcept;

        [[nodiscard]]
        bool IsEmpty() const noexcept;

        void Swap(TArray& Other) noexcept;
        void Reserve(Size_T NewCapacity);

        void Resize(Size_T NewSize);
        void Clear() noexcept;
        void Empty() noexcept;
        void Shrink();

        template<typename... Args>
        T& Emplace(Args&&... InArgs);

        void Add(const T& InElement);
        void Add(T&& InElement);

        void Pop();
        T PopBack();
        
        T& Front() noexcept;
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
        Size_T Size_{0};
        Size_T Capacity_{0};

        void Grow();
        void Reallocate(Size_T NewCapacity);
        void DestroyElements() noexcept;
    };
}

#include "Core/Containers/TArray.inl"
