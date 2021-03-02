#pragma once
#include "Point.hpp"
#include "Vector.hpp"

namespace MLib
{
    template<typename T>
    class Surface
    {
        public:
        static constexpr size_t Dimension = 3;

        private:
        Point<T , Dimension> point_;
        Vector<T , Dimension> norm_;

        public:
        Surface () = default;
        Surface (const Surface&) = default;
        Surface (Surface&&) noexcept = default;

        Surface (const Point<T , Dimension>& pt1 , const Point<T , Dimension>& pt2 , const Point<T , Dimension>& pt3);
        Surface (const Point<T , Dimension>& pt , const Vector<T , Dimension>& vec);
        Surface (Point<T , Dimension>&& pt , Vector<T , Dimension>&& vec);

        Surface& operator = (const Surface& that) = default;

        enum class Side { Back , Front , Laying };
        Side GetSide (const Point<T , Dimension>& pt);

        template <size_t count>
        bool InterSect (const std::array<Point<T , Dimension> , count>& points);
    };

    template<typename T>
    Surface<T>::Surface (const Point<T , Dimension>& pt , const Vector<T , Dimension>& vec)
        : point_ (pt) ,
        norm_ (vec)
    {}
    template<typename T>
    Surface<T>::Surface (Point<T , Dimension>&& pt , Vector<T , Dimension>&& vec)
        : point_ (pt) ,
        norm_ (vec)
    {}
    template<typename T>
    Surface<T>::Surface (const Point<T , Dimension>& pt1 , const Point<T , Dimension>& pt2 , const Point<T , Dimension>& pt3)
        : point_ (pt1) ,
        norm_ (std::move (VecMult (Vector (pt2 , pt3) , Vector (pt1 , pt3))))
    {}

    template<typename T>
    Surface<T>::Side Surface<T>::GetSide (const Point<T , Dimension>& pt)
    {
        T out = ScalMult (norm_ , Vector { point_, pt });
        if (out > 0)
            return Side::Front;
        if (out < 0)
            return Side::Back;
        return Side::Laying;
    }

    template<typename T>
    template <size_t count>
    bool Surface<T>::InterSect (const std::array<Point<T , Dimension> , count>& points)
    {
        std::array<Side , count> sides;
        for (size_t i = 0; i < count; ++i)
            sides[i] = GetSide (points[i]);
        for (size_t i = 0; i < count - 1; ++i)
            if (sides[i] != sides[i + 1])
                return true;
        return false;
    }

} // namespace MLib

template <typename T>
using SurfSize = MLib::Surface<T>::Side;