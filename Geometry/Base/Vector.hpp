#pragma once
#include <iostream>
#include <array>
#include "../Common_libs/Errors.hpp"
#include "../Common_libs/Color.hpp"
#include <fstream>

namespace MLib
{
    template <typename T , size_t dim>
    class Vector
    {
        private:

        std::array<T , dim> data_;

        public:

        bool Empty () const noexcept { return data_.empty (); }
        explicit operator bool () const noexcept { return !Empty (); }

        constexpr T operator[] (int num) const { return data_[num]; }
        void operator += (const Vector& that);
        void operator -= (const Vector& that);

        Vector () = default;
        Vector (const Vector& that) = default;
        Vector (Vector&& that) noexcept = default;
        Vector& operator = (const Vector& that) = default;
        Vector& operator = (Vector&& that) = default;

        Vector (const std::array<T , dim>& init_data);
        Vector (std::array<T , dim>&& init_data);
        Vector (const Point<T , dim>& lhs , const Point<T , dim>& rhs);

        ~Vector () noexcept = default;
    };

    //!Useful extra functions!
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////
    template <typename T , size_t dim>
    T ScalMult (const Vector<T , dim>& lhs , const Vector<T , dim>& rhs);
    template <typename T>
    Vector<T , 3> VecMult (const Vector<T , 3>& lhs , const Vector<T , 3>& rhs);
    ///////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////


    template <typename T , size_t dim>
    Vector<T , dim>::Vector (const std::array<T , dim>& init_data)
        : data_ (init_data)
    {}

    template <typename T , size_t dim>
    Vector<T , dim>::Vector (std::array<T , dim>&& init_data)
        : data_ (std::move (init_data))
    {}

    template <typename T , size_t dim>
    Vector<T , dim>::Vector (const Point<T , dim>& lhs , const Point<T , dim>& rhs)
    {
        for (size_t i = 0; i < dim; ++i)
            data_[i] = rhs[i] - lhs[i];
    }

    //!Operators
    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////
    template <typename T , size_t dim>
    void Vector<T , dim>::operator+=(const Vector<T , dim>& that)
    {
        MLib::DoEveryElem<T> (data_ , that.data_ , 
        [] (T& elem , const T& elem2) { elem += elem2; });
    }

    template <typename T , size_t dim>
    void Vector<T , dim>::operator-=(const Vector<T , dim>& that)
    {
        MLib::DoEveryElem<T> (data_ , that.data_ , 
        [] (T& elem , const T& elem2) { elem -= elem2; });
    }
}

template <typename T , size_t dim>
bool operator == (const MLib::Vector<T , dim>& lhs , const MLib::Vector<T , dim>& rhs)
{
    for (size_t i = 0; i < dim; ++i)
        if (lhs[i] != rhs[i])
            return false;
    return true;
}
template <typename T , size_t dim>
bool operator != (const MLib::Vector<T , dim>& lhs , const MLib::Vector<T , dim>& rhs)
{
    return !(lhs == rhs);
}

template <typename T , size_t dim>
MLib::Vector<T , dim> operator + (const MLib::Vector<T , dim>& lhs , const MLib::Vector<T , dim>& rhs)
{
    MLib::Vector out { lhs };
    out += rhs;
    return out;
}

template <typename T , size_t dim>
MLib::Vector<T , dim> operator - (const MLib::Vector<T , dim>& lhs , const MLib::Vector<T , dim>& rhs)
{
    MLib::Vector out { lhs };
    out -= rhs;
    return out;
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

//!DUMPING
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
namespace MLib_Vector
{
    template < typename stream_t , typename T , size_t dim>
    void Dump (stream_t& stream , const MLib::Vector<T , dim>& Vector)
    {
        if (!Vector)
        {
            stream << MLib::Color::Red << "Can't dump empty Vector!\n" << MLib::Color::Reset;
            return;
        }

        stream << MLib::Color::Bold_Green << "Starting dump of Vector!\n"
            << MLib::Color::Bold_Green << "| Node\t| Data\t|\n"
            << MLib::Color::Green << "-----------------\n";

        for (size_t i = 0; i < dim; ++i)
        {
            stream << "| " << i << "\t| " << Vector[i] << "\t|\n";
        }

        stream << MLib::Color::Bold_Green << "Ended dump of Vector!\n" << MLib::Color::Reset;
    }
} // namespace MLib_Vector

template <typename T , size_t dim>
std::ostream& operator << (std::ostream& stream , const MLib::Vector<T , dim>& Vector)
{
    MLib_Vector::Dump (stream , Vector);
    return stream;
}

template <typename T , size_t dim>
std::ofstream& operator << (std::ofstream& stream , const MLib::Vector<T , dim>& Vector)
{
    MLib_Vector::Dump (stream , Vector);
    return stream;
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

namespace MLib
{
    template <typename T , size_t dim>
    T ScalMult (const Vector<T , dim>& lhs , const Vector<T , dim>& rhs)
    {
        if (lhs && rhs)
        {
            T out = lhs[0] * rhs[0];
            for (size_t i = 1; i < dim; ++i)
                out += lhs[i] * rhs[i];

            return out;
        }

        //!lhs or rhs wasn't initialized!
        WARNING ("Can't find ScalMult of uninitiaized vectors!\n");
        return T {};

    }
    template <typename T>
    Vector<T , 3> VecMult (const Vector<T , 3>& lhs , const Vector<T , 3>& rhs)
    {
        std::array<T , 3> out;
        out[0] = lhs[1] * rhs[2] - lhs[2] * rhs[1];
        out[1] = lhs[2] * rhs[0] - lhs[0] * rhs[2];
        out[2] = lhs[0] * rhs[1] - lhs[1] * rhs[0];
        return Vector { std::move (out) };
    }
}