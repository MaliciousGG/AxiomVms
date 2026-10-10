#pragma once

#include "Core/Containers/TArray.h"

namespace AxiomVms
{
    template<typename T>
    TArray<T>::TArray()
        : Data_(nullptr)
    {}

    template<typename T>
    TArray<T>::TArray(std::initializer_list<T> list)
        : Data_(nullptr)
    {
        Reserve(list.size());

        for (const T& element : list)
        {
            Add(element);
        }
    }

    template<typename T>
    TArray<T>::~TArray()
    {
        Empty();
    }

    template<typename T>
    TArray<T>::TArray(const TArray& other)
        : Data_(nullptr)
        , Size_(other.Size_)
        , Capacity_(other.Capacity_)
    {
        if (Capacity_ == 0)
        {
            return;
        }

        Data_ = new T[Capacity_];

        for (std::size_t i = 0; i < Size_; ++i)
        {
            Data_[i] = other.Data_[i];
        }
    }

    template<typename T>
    TArray<T>::TArray(TArray&& other) noexcept
        : Data_(other.Data_)
        , Size_(other.Size_)
        , Capacity_(other.Capacity_)
    {
        other.Data_ = nullptr;
        other.Size_ = 0;
        other.Capacity_ = 0;
    }

    template<typename T>
    TArray<T>& TArray<T>::operator=(const TArray &other)
    {
        if (this != &other)
        {
            TArray temp(other);

            Swap(temp);
        }

        return *this;
    }

    template<typename T>
    TArray<T>& TArray<T>::operator=(TArray&& other) noexcept
    {
        if (this == &other)
        {
            return *this;
        }

        delete[] Data_;

        Data_ = other.Data_;
        Size_ = other.Size_;
        Capacity_ = other.Capacity_;

        other.Data_ = nullptr;
        other.Size_ = 0;
        other.Capacity_ = 0;

        return *this;
    }

    template<typename T>
    T& TArray<T>::operator[](std::size_t index) noexcept
    {
        return Data_[index];
    }

    template<typename T>
    const T& TArray<T>::operator[](std::size_t index) const noexcept
    {
        return Data_[index];
    }

    template<typename T>
    T& TArray<T>::At(std::size_t index)
    {
        if (index > Size_)
        {
            throw std::out_of_range("Index out of range.");
        }

        return Data_[index];
    }

    template<typename T>
    const T& TArray<T>::At(std::size_t index) const
    {
        if (index > Size_)
        {
            throw std::out_of_range("Index out of range.");
        }

        return Data_[index];
    }


    template<typename T>
    std::size_t TArray<T>::Num() const noexcept
    {
        return Size_;
    }

    template<typename T>
    std::size_t TArray<T>::Size() const noexcept
    {
        return Size_;
    }

    template<typename T>
    std::size_t TArray<T>::Max() const noexcept
    {
        return Capacity_;
    }

    template<typename T>
    bool TArray<T>::IsEmpty() const noexcept
    {
        return Size_ == 0;
    }

    template<typename T>
    void TArray<T>::Swap(TArray &other) noexcept
    {
        std::swap(Data_, other.Data_);
        std::swap(Size_, other.Size_);
        std::swap(Capacity_, other.Capacity_);
    }


    template<typename T>
    void TArray<T>::Reserve(const std::size_t newCapacity) noexcept
    {
        if (newCapacity <= Capacity_)
        {
            return;
        }

        auto* newData = new T[newCapacity];

        for (std::size_t i = 0; i < Size_; ++i)
        {
            newData[i] = std::move(Data_[i]);
        }

        delete[] Data_;

        Data_ = newData;
        Capacity_ = newCapacity;
    }

    template<typename T>
    void TArray<T>::Resize(const std::size_t newSize)
    {
        if (newSize == Size_)
        {
            return;
        }

        if (newSize > Capacity_)
        {
            Reserve(newSize);
        }

        Size_ = newSize;
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

        ::operator delete(Data_);

        Data_ = nullptr;
        Size_ = 0;
        Capacity_ = 0;
    }


    template<typename T>
    void TArray<T>::Shrink()
    {
        if (Size_ == Capacity_)
        {
            return;
        }

        if (Size_ == 0)
        {
            Empty();

            return;
        }

        Reallocate(Size_);
    }

    template<typename T>
    template<typename ... ArgumentTypes>
    T& TArray<T>::Emplace(ArgumentTypes&&... arguments)
    {
        if (Size_ == Capacity_)
        {
            Grow();
        }

        new (&Data_[Size_]) T(std::forward<ArgumentTypes>(arguments)...);

        T& newElement = Data_[Size_];
        ++Size_;

        return newElement;
    }

    template<typename T>
    void TArray<T>::Add(const T& element)
    {
        if (Size_ == Capacity_)
        {
            Grow();
        }

        new (&Data_[Size_]) T(element);
        ++Size_;
    }

    template<typename T>
    void TArray<T>::Add(T&& element)
    {
        if (Size_ == Capacity_)
        {
            Grow();
        }

        new (&Data_[Size_]) T(std::move(element));
        ++Size_;
    }


    template<typename T>
    void TArray<T>::Pop()
    {
        if (Size_ == 0)
        {
            return;
        }

        --Size_;
    }

    template<typename T>
    T& TArray<T>::PopBack()
    {
        return Data_[--Size_];
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
        return Data_ + Size_;
    }

    template<typename T>
    const T* TArray<T>::End() const noexcept
    {
        return Data_ + Size_;
    }

    template<typename T>
    const T* TArray<T>::CEnd() const noexcept
    {
        return Data_ + Size_;
    }

    template<typename T>
    void TArray<T>::Grow()
    {
        const std::size_t newCapacity = Capacity_ == 0 ? 1: Capacity_ * 2;

        Reallocate(newCapacity);
    }

    template<typename T>
    void TArray<T>::Reallocate(std::size_t newCapacity)
    {
        if (newCapacity < Size_)
        {
            newCapacity = Size_;
        }

        if (newCapacity != Capacity_)
        {
            auto* newData = static_cast<T*>(::operator new(newCapacity * sizeof(T)));

            for (std::size_t i = 0; i < Size_; ++i)
            {
                new (&newData[i]) T(std::move(Data_[i]));

                Data_[i].~T();
            }

            ::operator delete(Data_);

            Data_ = newData;
            Capacity_ = newCapacity;
        }
    }


    template<typename T>
    void TArray<T>::DestroyElements() noexcept
    {
        for (std::size_t i = 0; i < Size_; ++i)
        {
            Data_[i].~T();
        }
    }
}