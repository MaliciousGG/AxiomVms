#pragma once

#include <algorithm>
#include <initializer_list>
#include <memory>
#include <new>
#include <stdexcept>
#include <utility>
#include <Core/Assert.h>

#include "Core/Containers/TArray.h"

namespace Axiom
{
    template<typename T>
    TArray<T>::TArray()
        : Data_(nullptr)
    {}

    template<typename T>
    TArray<T>::TArray(std::initializer_list<T> InList)
        : Data_(nullptr)
    {
        try
        {
            Reserve(InList.size());

            for (const T& Element : InList)
            {
                Emplace(Element);
            }
        }
        catch (...)
        {
            Empty();

            throw;
        }
    }

    template<typename T>
    TArray<T>::~TArray()
    {
        Empty();
    }

    template<typename T>
    TArray<T>::TArray(const TArray& Other)
        : Data_(nullptr)
    {
        try
        {
            Reserve(Other.Capacity_);

            for (Size_T i = 0; i < Other.Size_; ++i)
            {
                Emplace(Other.Data_[i]);
            }
        }
        catch (...)
        {
            Empty();

            throw;
        }
    }

    template<typename T>
    TArray<T>::TArray(TArray&& Other) noexcept
        : Data_(Other.Data_)
        , Size_(Other.Size_)
        , Capacity_(Other.Capacity_)
    {
        Other.Data_ = nullptr;
        Other.Size_ = 0;
        Other.Capacity_ = 0;
    }

    template<typename T>
    TArray<T>& TArray<T>::operator=(const TArray& Other)
    {
        if (this != &Other)
        {
            TArray Temp(Other);

            Swap(Temp);
        }

        return *this;
    }

    template<typename T>
    TArray<T>& TArray<T>::operator=(TArray&& Other) noexcept
    {
        if (this != &Other)
        {
            Empty();

            Data_ = Other.Data_;
            Size_ = Other.Size_;
            Capacity_ = Other.Capacity_;

            Other.Data_ = nullptr;
            Other.Size_ = 0;
            Other.Capacity_ = 0;
        }

        return *this;
    }

    template<typename T>
    T& TArray<T>::operator[](Size_T Index) noexcept
    {
        return Data_[Index];
    }

    template<typename T>
    const T& TArray<T>::operator[](Size_T Index) const noexcept
    {
        return Data_[Index];
    }

    template<typename T>
    T& TArray<T>::At(Size_T Index)
    {
        if (Index >= Size_)
        {
            throw std::out_of_range("Index out of range.");
        }

        return Data_[Index];
    }

    template<typename T>
    const T& TArray<T>::At(Size_T Index) const
    {
        if (Index >= Size_)
        {
            throw std::out_of_range("Index out of range.");
        }

        return Data_[Index];
    }

    template<typename T>
    uint32 TArray<T>::Num() const noexcept
    {
        return static_cast<uint32>(Size_);
    }

    template<typename T>
    Size_T TArray<T>::Size() const noexcept
    {
        return Size_;
    }

    template<typename T>
    Size_T TArray<T>::Max() const noexcept
    {
        return Capacity_;
    }

    template<typename T>
    bool TArray<T>::IsEmpty() const noexcept
    {
        return Size_ == 0;
    }

    template<typename T>
    void TArray<T>::Swap(TArray& Other) noexcept
    {
        std::swap(Data_, Other.Data_);
        std::swap(Size_, Other.Size_);
        std::swap(Capacity_, Other.Capacity_);
    }

    template<typename T>
    void TArray<T>::Reserve(const Size_T NewCapacity)
    {
        if (NewCapacity > Capacity_)
        {
            Reallocate(NewCapacity);
        }
    }

    template<typename T>
    void TArray<T>::Resize(const Size_T NewSize)
    {
        if (NewSize == Size_)
        {
            return;
        }

        if (NewSize < Size_)
        {
            // Destroy removed elements in reverse order.
            for (Size_T Index = Size_; Index > NewSize; --Index)
            {
                Data_[Index - 1].~T();
            }

            Size_ = NewSize;

            return;
        }

        Reserve(NewSize);

        const Size_T OldSize = Size_;

        Size_T ConstructedEnd = OldSize;

        try
        {
            // Construct the additional elements without changing the logical size.
            for (; ConstructedEnd < NewSize; ++ConstructedEnd)
            {
                ::new (static_cast<void*>(Data_ + ConstructedEnd)) T();
            }
        }
        catch (...)
        {
            std::destroy(Data_ + OldSize,Data_ + ConstructedEnd);

            throw;
        }

        Size_ = NewSize;
    }

    template<typename T>
    void TArray<T>::Clear() noexcept
    {
        DestroyElements();

        Size_ = 0;
    }

    template<typename T>
    void TArray<T>::Empty() noexcept
    {
        Clear();
        
        if (Data_ != nullptr)
        {
            std::allocator<T> Allocator;
            
            Allocator.deallocate(Data_, Capacity_);
        }
        
        Data_ = nullptr;
        Capacity_ = 0;
    }

    template<typename T>
    void TArray<T>::Shrink()
    {
        if (Size_ == 0)
        {
            Empty();
        }
        else if (Size_ != Capacity_)
        {
            Reallocate(Size_);
        }
    }

    template<typename T>
    template<typename... Args>
    T& TArray<T>::Emplace(Args&&... InArgs)
    {
        if (Size_ < Capacity_)
        {
            ::new (static_cast<void*>(Data_ + Size_)) T(std::forward<Args>(InArgs)...);
            
            return Data_[Size_++];
        }

        std::allocator<T> Allocator;

        const Size_T Limit = std::allocator_traits<std::allocator<T>>::max_size(Allocator);
        
        if (Size_ >= Limit)
        {
            throw std::length_error("TArray capacity exceeded.");
        }

        const Size_T NewCapacity = Capacity_ == 0 ? 1 : (Capacity_ > Limit / 2 ? Limit : Capacity_ * 2);

        T* NewData = Allocator.allocate(NewCapacity);

        // Construct first because arguments may reference existing elements.
        try
        {
            ::new (static_cast<void*>(NewData + Size_)) T(std::forward<Args>(InArgs)...);
        }
        catch (...)
        {
            Allocator.deallocate(NewData, NewCapacity);

            throw;
        }

        Size_T Constructed = 0;

        try
        {
            for (; Constructed < Size_; ++Constructed)
            {
                ::new (static_cast<void*>(NewData + Constructed)) T(std::move_if_noexcept(Data_[Constructed]));
            }
        }
        catch (...)
        {
            std::destroy(NewData, NewData + Constructed);

            // Its construction succeeded before relocation started.
            std::destroy_at(NewData + Size_);

            Allocator.deallocate(NewData, NewCapacity);

            throw;
        }

        DestroyElements();
        
        if (Data_ != nullptr)
        {
            Allocator.deallocate(Data_, Capacity_);
        }
            
        Data_ = NewData;
        Capacity_ = NewCapacity;

        return Data_[Size_++];
    }

    template<typename T>
    void TArray<T>::Add(const T& InElement)
    {
        Emplace(InElement);
    }

    template<typename T>
    void TArray<T>::Add(T&& InElement)
    {
        Emplace(std::move(InElement));
    }

    template<typename T>
    void TArray<T>::Pop()
    {
        if (Size_ != 0)
        {
            Data_[--Size_].~T();
        }
    }

    template<typename T>
    T TArray<T>::PopBack()
    {
        if (Size_ == 0)
        {
            throw std::out_of_range("Cannot pop an empty TArray.");
        }

        T Result(std::move_if_noexcept(Data_[Size_ - 1]));

        Pop();

        return Result;
    }

    template <typename T>
    T& TArray<T>::Front() noexcept
    {
        return Data_[0];
    }

    template<typename T>
    T& TArray<T>::Back() noexcept
    {
        return Data_[Size_ - 1];
    }

    template<typename T>
    T* TArray<T>::GetData() noexcept
    {
        return Data_;
    }

    template<typename T>
    const T* TArray<T>::GetData() const noexcept
    {
        return Data_;
    }

    template<typename T>
    T* TArray<T>::Begin() noexcept
    {
        return Data_;
    }

    template<typename T>
    const T* TArray<T>::Begin() const noexcept
    {
        return Data_;
    }

    template<typename T>
    const T* TArray<T>::CBegin() const noexcept
    {
        return Data_;
    }

    template<typename T>
    T* TArray<T>::End() noexcept
    {
        return Size_ == 0 ? Data_ : Data_ + Size_;
    }

    template<typename T>
    const T* TArray<T>::End() const noexcept
    {
        return Size_ == 0 ? Data_ : Data_ + Size_;
    }

    template<typename T>
    const T* TArray<T>::CEnd() const noexcept
    {
        return Size_ == 0 ? Data_ : Data_ + Size_;
    }

    template<typename T>
    void TArray<T>::Grow()
    {
        std::allocator<T> Allocator;

        const Size_T Limit = std::allocator_traits<std::allocator<T>>::max_size(Allocator);

        if (Capacity_ >= Limit)
        {
            throw std::length_error("TArray capacity exceeded.");
        }

        Reallocate(Capacity_ == 0 ? 1 : (Capacity_ > Limit / 2 ? Limit : Capacity_ * 2));
    }

    template<typename T>
    void TArray<T>::Reallocate(Size_T NewCapacity)
    {
        NewCapacity = std::max(NewCapacity, Size_);

        if (NewCapacity == Capacity_)
        {
            return;
        }

        if (NewCapacity == 0)
        {
            Empty();

            return;
        }

        std::allocator<T> Allocator;

        if (NewCapacity > std::allocator_traits<std::allocator<T>>::max_size(Allocator))
        {
            throw std::length_error("TArray capacity exceeded.");
        }

        T* NewData = Allocator.allocate(NewCapacity);

        Size_T Constructed = 0;

        try
        {
            for (; Constructed < Size_; ++Constructed)
            {
                ::new (static_cast<void*>(NewData + Constructed)) T(std::move_if_noexcept(Data_[Constructed]));
            }
        }
        catch (...)
        {
            std::destroy(NewData, NewData + Constructed);

            Allocator.deallocate(NewData, NewCapacity);

            throw;
        }

        DestroyElements();

        if (Data_ != nullptr)
        {
            Allocator.deallocate(Data_, Capacity_);
        }

        Data_ = NewData;
        Capacity_ = NewCapacity;
    }

    template<typename T>
    void TArray<T>::DestroyElements() noexcept
    {
        for (Size_T i = Size_; i > 0; --i)
        {
            Data_[i - 1].~T();
        }
    }
}
