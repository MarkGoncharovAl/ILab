#pragma once
#include <iostream>
#include <array>
#include "../Common_libs/Errors.hpp"
#include "../Common_libs/Color.hpp"
#include "../Common_libs/Arrays.hpp"
#include <fstream>

namespace MLib
{
    template <typename T , size_t dim>
    class Point
    {
    private:

        std::array<T , dim> data_;

    public:

        bool Empty () const noexcept { return data_.empty (); }
        explicit operator bool () const noexcept { return !Empty (); }

        constexpr T operator[] (int num) const { return data_[num]; }
        void operator += (const Point& that);
        void operator -= (const Point& that);

        Point () = default;
        Point (const Point& that) = default;

        template <typename U>
        Point (const Point<U , dim>& that);

        Point (Point&& that) noexcept = default;
        Point& operator = (const Point& that) = default;
        Point& operator = (Point&& that) = default;

        Point (const std::array<T , dim>& init_data);
        Point (std::array<T , dim>&& init_data);

        ~Point () noexcept = default;
    };

    template <typename T , size_t dim>
    Point<T , dim>::Point (const std::array<T , dim>& init_data)
        : data_ (init_data)
    {}

    template <typename T , size_t dim>
    Point<T , dim>::Point (std::array<T , dim>&& init_data)
        : data_ (std::move (init_data))
    {}

    template <typename T , size_t dim>
    template <typename U>
    Point<T , dim>::Point (const Point<U , dim>& that)
    {
        std::array<T , dim> arr;
        for (size_t i = 0; i < dim; ++i)
            arr[i] = static_cast<T>(that[i]);
        
        data_ = std::move(arr);
    }


    //!Operators
    ////////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////////
    template <typename T , size_t dim>
    void Point<T , dim>::operator+=(const Point<T , dim>& that)
    {
        MLib::DoEveryElem<T> (data_ , that.data_ ,
        [](T& elem , const T& elem2) { elem += elem2; });
    }

    template <typename T , size_t dim>
    void Point<T , dim>::operator-=(const Point<T , dim>& that)
    {
        MLib::DoEveryElem<T> (data_ , that.data_ ,
        [](T& elem , const T& elem2) { elem -= elem2; });
    }
}

template <typename T , size_t dim>
bool operator == (const MLib::Point<T , dim>& lhs , const MLib::Point<T , dim>& rhs)
{
    for (size_t i = 0; i < dim; ++i)
        if (lhs[i] != rhs[i])
            return false;
    return true;
}
template <typename T , size_t dim>
bool operator != (const MLib::Point<T , dim>& lhs , const MLib::Point<T , dim>& rhs)
{
    return !(lhs == rhs);
}

template <typename T , size_t dim>
MLib::Point<T , dim> operator + (const MLib::Point<T , dim>& lhs , const MLib::Point<T , dim>& rhs)
{
    MLib::Point out { lhs };
    out += rhs;
    return out;
}

template <typename T , size_t dim>
MLib::Point<T , dim> operator - (const MLib::Point<T , dim>& lhs , const MLib::Point<T , dim>& rhs)
{
    MLib::Point out { lhs };
    out -= rhs;
    return out;
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

//!DUMPING
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
namespace MLib_Point
{
    template < typename stream_t , typename T , size_t dim>
    void Dump (stream_t& stream , const MLib::Point<T , dim>& point)
    {
        if (!point)
        {
            stream << MLib::Color::Red << "Can't dump empty Point!\n" << MLib::Color::Reset;
            return;
        }

        stream << MLib::Color::Bold_Green << "Starting dump of Point!\n"
            << MLib::Color::Bold_Green << "| Node\t| Data\t|\n"
            << MLib::Color::Green << "-----------------\n";

        for (size_t i = 0; i < dim; ++i)
            stream << "| " << i << "\t| " << point[i] << "\t|\n";

        stream << MLib::Color::Bold_Green << "Ended dump of Point!\n" << MLib::Color::Reset;
    }
} // namespace MLib_Point

template <typename T , size_t dim>
std::ostream& operator << (std::ostream& stream , const MLib::Point<T , dim>& point)
{
    MLib_Point::Dump (stream , point);
    return stream;
}

template <typename T , size_t dim>
std::ofstream& operator << (std::ofstream& stream , const MLib::Point<T , dim>& point)
{
    MLib_Point::Dump (stream , point);
    return stream;
}
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////