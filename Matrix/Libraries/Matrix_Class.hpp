#pragma once
#include "Buffer.hpp"
#include <memory>
#include <functional>
#include <fstream>
#include "../Common_libs/iterators.hpp"

//there were a lot thinking about:
//shoud I made size_x and size_y as templates...
//like in std::array...
//but, there will be a lot of matrix and a lot of code...
//therefore didn't do this

template <class Type_t>
class Matrix_t final : private Buffer<Type_t>
{
public:
    //  !Constructors
    //////////////////////////////////////////////////
    Matrix_t();
    Matrix_t(size_t init_row, size_t init_co, const Type_t &base_value = Type_t{});

    template <class Another_Type>
    Matrix_t(const Matrix_t<Another_Type> &that);
    Matrix_t(const Matrix_t &);
    Matrix_t(Matrix_t &&) = default;

    Matrix_t &operator=(const Matrix_t &);

    ~Matrix_t() {}

    void InitMemory(size_t row, size_t col);
    void InitElems(std::initializer_list<std::initializer_list<Type_t>> init);
    //////////////////////////////////////////////////

    //  !Checkers
    //////////////////////////////////////////////////
    explicit operator bool() const noexcept { return !this->Empty(); }
    bool IsSquare() const noexcept;
    bool operator==(const Matrix_t &that) const;
    bool operator!=(const Matrix_t &that) const { return !(operator==(that)); }
    //////////////////////////////////////////////////

    //  !Setters
    //////////////////////////////////////////////////
    void Set(size_t row, size_t col, const Type_t &init);

    class SetterRaw_t final
    {
    public:
        //default appropriate
        SetterRaw_t() noexcept = default;
        SetterRaw_t(Type_t *init_data, size_t init_max) noexcept;
        SetterRaw_t(const SetterRaw_t &that) = default;
        SetterRaw_t(SetterRaw_t &&that) = default;
        SetterRaw_t &operator=(const SetterRaw_t &) = default;
        SetterRaw_t &operator=(SetterRaw_t &&) = default;

        explicit operator bool() const noexcept { return data_ != nullptr; }

        Type_t &operator[](size_t num);
        Type_t operator[](size_t num) const;

        size_t size() const noexcept { return size_; }

        MC_iterator<Type_t> cbegin() const noexcept(noexcept(MC_iterator<Type_t>{nullptr})) { return MC_iterator<Type_t>{data_}; }
        MC_iterator<Type_t> cend() const noexcept(noexcept(MC_iterator<Type_t>{nullptr})) { return MC_iterator<Type_t>{data_ + size_}; }
        M_iterator<Type_t> begin() noexcept(noexcept(M_iterator<Type_t>{nullptr})) { return M_iterator<Type_t>{data_}; }
        M_iterator<Type_t> end() noexcept(noexcept(MC_iterator<Type_t>{nullptr})) { return M_iterator<Type_t>{data_ + size_}; }

    private:
        Type_t *data_ = nullptr;
        size_t size_ = 0;
    };

    SetterRaw_t operator[](size_t num);
    //////////////////////////////////////////////////

    //  !Getters
    //////////////////////////////////////////////////
    class GetterRaw_t final
    {
    public:
        //default appropriate
        GetterRaw_t() noexcept = default;
        GetterRaw_t(Type_t *init_data, size_t init_max) noexcept;
        GetterRaw_t(const GetterRaw_t &that) = default;
        GetterRaw_t(GetterRaw_t &&that) = default;
        GetterRaw_t &operator=(const GetterRaw_t &) = default;
        GetterRaw_t &operator=(GetterRaw_t &&) = default;

        explicit operator bool() const noexcept { return data_ != nullptr; }

        Type_t operator[](size_t num) const;
        size_t size() const noexcept { return size_; }

        MC_iterator<Type_t> cbegin() const noexcept(noexcept(MC_iterator<Type_t>{nullptr})) { return MC_iterator<Type_t>{data_}; }
        MC_iterator<Type_t> cend() const noexcept(noexcept(MC_iterator<Type_t>{nullptr})) { return MC_iterator<Type_t>{data_ + size_}; }

    private:
        Type_t *data_ = nullptr;
        size_t size_ = 0;
    };

    GetterRaw_t operator[](size_t num) const;

    size_t GetSizeRaw() const noexcept { return size_.row; }
    size_t GetSizeCol() const noexcept { return size_.col; }
    //////////////////////////////////////////////////

    //  !Algebra
    //////////////////////////////////////////////////
    void operator+=(const Matrix_t<Type_t> &that);
    void operator-=(const Matrix_t<Type_t> &that);

    Matrix_t operator+(const Matrix_t<Type_t> &that) const &;
    Matrix_t operator+(const Matrix_t<Type_t> &that) &&;
    Matrix_t operator-(const Matrix_t<Type_t> &that) const &;
    Matrix_t operator-(const Matrix_t<Type_t> &that) &&;

    Matrix_t &transpose() &;
    //////////////////////////////////////////////////

    //  !Debuggers
    //////////////////////////////////////////////////
    void dump() const;
    //////////////////////////////////////////////

    //  !Iterators
    //////////////////////////////////////////////////
    MC_iterator<Type_t> cbegin() const noexcept(noexcept(MC_iterator<Type_t>{nullptr})) { return MC_iterator<Type_t>{data_}; }
    MC_iterator<Type_t> cend() const noexcept(noexcept(MC_iterator<Type_t>{nullptr})) { return MC_iterator<Type_t>{data_ + size_.row * size_.col}; }

    M_iterator<Type_t> begin() noexcept(noexcept(M_iterator<Type_t>{nullptr})) { return M_iterator<Type_t>{data_}; }
    M_iterator<Type_t> end() noexcept(noexcept(M_iterator<Type_t>{nullptr})) { return M_iterator<Type_t>{data_ + size_.row * size_.col}; }

    MS_iterator<Type_t> sbegin() noexcept(noexcept(MS_iterator<Type_t>{nullptr, nullptr})) { return MS_iterator<Type_t>{data_, data_ + size_.row * size_.col}; }

    MS_iterator<Type_t> send() noexcept(noexcept(MS_iterator<Type_t>{nullptr, nullptr}))
    {
        Type_t *end = data_ + size_.row * size_.col;
        return MS_iterator<Type_t>{end, end};
    }
    //////////////////////////////////////////////////

private:
    using Buffer<Type_t>::data_;
    using Buffer<Type_t>::used_;
    using Buffer<Type_t>::capacity_;

    struct size_matrix_t
    {
        size_t row = 0;
        size_t col = 0;
    };

    size_matrix_t size_;

    //  dannger but fast - for programmer only!
    Type_t &get_object(size_t row, size_t col);
};

//  !Constructors
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
template <class Type_t>
Matrix_t<Type_t>::Matrix_t()
    : Buffer<Type_t>(),
      size_({0, 0})
{
}

template <class Type_t>
Matrix_t<Type_t>::
    Matrix_t(size_t init_row, size_t init_col, const Type_t &base_data /*= Type_T{}*/)
    : Buffer<Type_t>(init_col * init_row),
      size_({init_row, init_col})
{
    for (size_t i = 0; i < init_col * init_row; ++i)
        this->Construct(base_data);
}

template <class Type_t>
Matrix_t<Type_t>::Matrix_t(const Matrix_t &that)
    : Buffer<Type_t>(that.size_.row * that.size_.col),
      size_({that.size_.row, that.size_.col})
{
    if (that.Empty())
    {
        WARNING("Using Constructor of copy with null value!");
        return;
    }

    for (auto iter = that.cbegin(); iter != that.cend(); ++iter)
        this->Construct(*iter);
}

template <class Type_t>
template <class Another_Type>
Matrix_t<Type_t>::Matrix_t(const Matrix_t<Another_Type> &that)
    : Buffer<Type_t>(that.GetSizeRaw() * that.GetSizeCol()),
      size_({that.GetSizeRaw(), that.GetSizeCol()})
{
    if (!that)
    {
        WARNING("Using Constructor of copy with null value!");
        return;
    }

    for (auto iter = that.cbegin(); iter != that.cend(); ++iter)
        this->Construct(static_cast<Type_t>(*iter));
}

template <class Type_t>
Matrix_t<Type_t> &Matrix_t<Type_t>::operator=(const Matrix_t &that)
{
    Matrix_t<Type_t> tmp{that};
    //----------------------------------
    this->Swap(tmp);
    size_.row = that.size_.row;
    size_.col = that.size_.col;

    return *this;
}

template <class Type_t>
void Matrix_t<Type_t>::InitMemory(size_t row, size_t col)
{
    this->Realloc(row * col);
    this->ConstructAll(Type_t{});
    //----------------------------------
    size_.row = row;
    size_.col = col;
}

template <class Type_t>
void Matrix_t<Type_t>::InitElems(
    std::initializer_list<std::initializer_list<Type_t>> init)
{
    if (this->Empty())
        ERROR("Can't InitElems with not initialized matrix. INITIALIZE IT!");
    if (init.size() != size_.row)
        ERROR("Can't initialize matrix - ROWS!");

    Type_t *cur_data = data_;
    for (auto row_iter = init.begin(); row_iter != init.end(); ++row_iter)
    {
        if (row_iter->size() != size_.col)
            ERROR("Can't initialize matrix - COLS!");

        for (auto col_iter = row_iter->begin();
             col_iter != row_iter->end();
             ++col_iter)
        {
            *cur_data = *col_iter;
            cur_data++;
        }
    }
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

//  !Checkers
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
template <class Type_t>
bool Matrix_t<Type_t>::IsSquare() const noexcept
{
    return (!this->Empty() && size_.row == size_.col);
}

template <class Type_t>
bool Matrix_t<Type_t>::operator==(const Matrix_t &that) const
{
    if (this->Empty() && that.Empty())
        return true;

    if (that.Empty() ||
        this->Empty() ||
        size_.row != that.size_.row ||
        size_.col != that.size_.col)
        return false;

    Type_t *data1 = data_;
    Type_t *data2 = that.data_;
    for (size_t i = 0; i < size_.row * size_.col; ++i)
    {
        if (*data1 != *data2) //exception can be found there
            return false;

        data1++;
        data2++;
    }

    return true;
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

//  !Setters
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
template <class Type_t>
void Matrix_t<Type_t>::Set(size_t row, size_t col, const Type_t &init)
{
    if (data_ == nullptr)
    {
        WARNING("Can't set in nullptr massive!");
        return;
    }
    if (row >= size_.row || col >= size_.col)
    {
        WARNING("Can't set in out_massive data!");
        return;
    }

    get_object(row, col) = init;
}

template <class Type_t>
Matrix_t<Type_t>::SetterRaw_t::
    SetterRaw_t(Type_t *init_data, size_t init_max) noexcept
    : data_(init_data),
      size_(init_max)
{
}

template <class Type_t>
typename Matrix_t<Type_t>::SetterRaw_t
Matrix_t<Type_t>::operator[](size_t row)
{
    if (data_ == nullptr || row >= size_.row)
        throw std::out_of_range(std::string("Tried to get away from massive ROW: ") + __FILE__ + " in line " + std::to_string(__LINE__));

    return SetterRaw_t{data_ + size_.col * row, size_.col};
}

template <class Type_t>
Type_t Matrix_t<Type_t>::
    SetterRaw_t::operator[](size_t num) const
{
    if (num >= size_)
    {
        WARNING("Tried to get away of masive COLUMN!");
        return Type_t{};
    }
    return data_[num];
}

template <class Type_t>
Type_t &Matrix_t<Type_t>::
    SetterRaw_t::operator[](size_t num)
{
    if (num >= size_)
        throw std::out_of_range(std::string("Tried to get away of masive COLUMN!") + __FILE__ + " in line " + std::to_string(__LINE__));

    return data_[num];
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

//  !Getters
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

template <class Type_t>
Matrix_t<Type_t>::GetterRaw_t::
    GetterRaw_t(Type_t *init_data, size_t init_max) noexcept
    : data_(init_data),
      size_(init_max)
{
}

template <class Type_t>
Type_t Matrix_t<Type_t>::
    GetterRaw_t::operator[](size_t num) const
{
    if (num >= size_)
    {
        WARNING("Tried to get away of masive COLUMN!");
        return Type_t{};
    }
    return data_[num];
}

template <class Type_t>
typename Matrix_t<Type_t>::GetterRaw_t
Matrix_t<Type_t>::operator[](size_t num) const
{
    if (data_ == nullptr || num >= size_.row)
        ERROR("Tried to get away from massive ROW");
    return GetterRaw_t{data_ + size_.col * num, size_.col};
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

//  !Algebra
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
template <class Type_t>
void Matrix_t<Type_t>::operator+=(const Matrix_t<Type_t> &that)
{
    if (!data_ || !that.data_)
    {
        WARNING("Not defined matrixes!");
        return;
    }
    if (size_.row != that.size_.row || size_.col != that.size_.col)
    {
        WARNING("Matrixes have different sizes!");
        return;
    }

    //!Now everything is ok
    const Type_t *reader = that.data_;
    Type_t *writer = data_;
    size_t full_size = size_.row * size_.col;

    for (size_t i = 0; i < full_size; ++i, ++reader, ++writer)
        *writer += *reader;
}
template <class Type_t>
void Matrix_t<Type_t>::operator-=(const Matrix_t<Type_t> &that)
{
    if (!data_ || !that.data_)
    {
        WARNING("Not defined matrixes!");
        return;
    }
    if (size_.row != that.size_.row || size_.col != that.size_.col)
    {
        WARNING("Matrixes have different sizes!");
        return;
    }

    //!Now everything is ok
    const Type_t *reader = that.data_;
    Type_t *writer = data_;
    size_t full_size = size_.row * size_.col;

    //exceptions are below
    for (size_t i = 0; i < full_size; ++i, ++reader, ++writer)
        *writer -= *reader;
}

template <class Type_t>
Matrix_t<Type_t> Matrix_t<Type_t>::
operator+(const Matrix_t<Type_t> &that) const &
{
    Matrix_t<Type_t> out_matrix{*this};
    out_matrix += that;
    return out_matrix;
}
template <class Type_t>
Matrix_t<Type_t> Matrix_t<Type_t>::
operator+(const Matrix_t<Type_t> &that) &&
{
    operator+=(that);
    return *this;
}
template <class Type_t>
Matrix_t<Type_t> Matrix_t<Type_t>::
operator-(const Matrix_t<Type_t> &that) const &
{
    Matrix_t<Type_t> out_matrix{*this};
    out_matrix -= that;
    return out_matrix;
}
template <class Type_t>
Matrix_t<Type_t> Matrix_t<Type_t>::
operator-(const Matrix_t<Type_t> &that) &&
{
    operator-=(that);
    return *this;
}

template <class Type_t>
Matrix_t<Type_t> &Matrix_t<Type_t>::transpose() &
{
    if (!IsSquare())
    {
        WARNING("Transposing wasn't done!");
        return *this;
    }

    for (size_t i = 0; i < size_.row; ++i)
    {
        for (size_t j = i + 1; j < size_.col; ++j)
        {
            std::swap(get_object(i, j), get_object(j, i));
        }
    }

    return *this;
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

//  !Debuggers
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
template <class Type_t>
void Matrix_t<Type_t>::dump() const
{
    if (data_ != nullptr)
    {
        Type_t *writer = data_;
        std::cout << "Dump is starting!\n";

        for (size_t i1 = 0; i1 < size_.row; ++i1)
        {
            for (size_t i2 = 0; i2 < size_.col; ++i2)
            {
                std::cout << *writer << "\t";
                writer++;
            }
            std::cout << std::endl;
        }
        std::cout << "Dump has ended!\n";
    }
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////

template <class Type_t>
Type_t &Matrix_t<Type_t>::get_object(size_t row, size_t col)
{
    return data_[row * size_.col + col];
}

//!Operator input
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
template <class T>
std::ifstream &operator>>(std::ifstream &in, Matrix_t<T> &matrix)
{
    for (auto &elem : matrix)
    {
        in >> elem;
        if (!in.good())
        {
            WARNING("Operator >> for matrix wasn't done peoprly!\n");
            return in;
        }
    }

    return in;
}
template <class T>
std::istream &operator>>(std::istream &in, Matrix_t<T> &matrix)
{
    for (auto &elem : matrix)
    {
        in >> elem;
        if (!in.good())
        {
            WARNING("Operator >> for matrix wasn't done peoprly!\n");
            return in;
        }
    }

    return in;
}
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////