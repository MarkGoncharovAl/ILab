#pragma once
#include <iostream>
#include <cassert>
#include "../Errors/Errors.hpp"

//!Check nullptr constructors

namespace buf_help_functions
{
    template <class T>
    void destroy_arr(T *start, T *end) noexcept
    {
        //check for noexcept destructor
        assert(noexcept(T().~T()));
        while (start < end)
        {
            start->~T();
            start++;
        }
    }

    template <class T>
    void construct_elem(T *place, const T &data)
    {
        new (place) T{data};
    }

} // namespace buf_help_functions

//!You can only be related after this class!
template <class Type>
class Buffer
{
protected:
    Buffer(size_t cap);
    virtual ~Buffer() noexcept;

    Buffer(const Buffer &);
    Buffer() = default;
    Buffer(Buffer &&);

    Buffer &operator=(const Buffer &) = delete;
    explicit operator bool() const noexcept { return data_ != nullptr && used_ == capacity_; }
    bool Empty() const noexcept { return data_ == nullptr; }

    void Swap(Buffer<Type> &) noexcept;
    ////////////
    //  returns nullptr if wasn't done properly
    ////////////
    Type *Construct(const Type &);
    void ConstructAll(const Type &);
    void Realloc(size_t cap);

    Type *data_ = nullptr;
    size_t used_ = 0;
    size_t capacity_ = 0;

private:
    void DestroyAll() noexcept;
};

template <class T>
Buffer<T>::Buffer(size_t cap)
    : data_((cap == 0) ? nullptr : static_cast<T *>(::operator new(sizeof(T) * cap))),
      used_(0),
      capacity_(cap)
{
    if (cap != 0 && data_ == nullptr)
        throw std::bad_alloc{};
}

template <class T>
Buffer<T>::Buffer(const Buffer<T> &that)
    : data_((that.capacity_ == 0) ? nullptr : static_cast<T *>(::operator new(sizeof(T) * that.capacity_))),
      used_(0),
      capacity_(that.capacity_)
{
    if (capacity_ != 0 && data_ == nullptr)
        throw std::bad_alloc{};

    for (size_t i = 0; i < that.used_; ++i)
        Construct(that.data_[i]);
}

template <class T>
Buffer<T>::Buffer(Buffer<T> &&that)
    : data_(nullptr),
      used_(0),
      capacity_(0)
{
    std::swap(data_, that.data_);
    std::swap(used_, that.used_);
    std::swap(capacity_, that.capacity_);
}

template <class T>
void Buffer<T>::Swap(Buffer<T> &that) noexcept
{
    std::swap(data_, that.data_);
    std::swap(used_, that.used_);
    std::swap(capacity_, that.capacity_);
}

template <class T>
void Buffer<T>::DestroyAll() noexcept
{
    buf_help_functions::destroy_arr(data_, data_ + used_ * sizeof(T));
    ::operator delete(data_);
}

template <class T>
Buffer<T>::~Buffer() noexcept
{
    DestroyAll();
}

template <class T>
T *Buffer<T>::Construct(const T &data)
{
    if (used_ >= capacity_)
        return nullptr;

    buf_help_functions::construct_elem(data_ + used_, data);
    //----------------------------------
    used_++;
    return data_ + used_ - 1;
}

template <class T>
void Buffer<T>::ConstructAll(const T &data)
{
    while (used_ < capacity_)
    {
        buf_help_functions::construct_elem(data_ + used_, data);
        //----------------------------------
        used_++;
    }
}

template <class T>
void Buffer<T>::Realloc(size_t cap)
{
    T *new_data = (cap == 0) ? nullptr : static_cast<T *>(::operator new(sizeof(T) * cap));

    if (cap != 0 && new_data == nullptr)
        throw std::bad_alloc{};

    //----------------------------------
    DestroyAll(); //noexcept

    data_ = new_data; //pointers
    used_ = 0;
    capacity_ = cap;
}