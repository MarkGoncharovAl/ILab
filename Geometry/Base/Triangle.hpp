#pragma once
#include "Point.hpp"
#include "Vector.hpp"

namespace MLib
{
    template<typename T>
    class Triangle
    {
    public:
        static constexpr size_t Dimension = 3;

    private:
        std::array<Point<T , Dimension> , 3> point_;
        Vector<T , Dimension> norm_;

    public:
        Triangle () = default;
        Triangle (const Triangle&) = default;
        Triangle (Triangle&&) noexcept = default;

        Triangle (const std::array< Point<T , Dimension> , 3>& pts);
        Triangle (const std::array<T , 9>& arr);

        template<typename U>
        Triangle (const std::array< Point<U , Dimension> , 3>& pts);

        Triangle (std::array< Point<T , Dimension> , 3>&& pts);

        Triangle& operator = (const Triangle& that) = default;
        Triangle& operator = (Triangle&& that) = default;

        enum class Side { Back , Front , Laying };
        Side GetSide (const Point<T , Dimension>& pt) const;

        bool InterSect (const Triangle<T>& points) const;
    };


    template<typename T>
    Triangle<T>::Triangle (const std::array< Point<T , Dimension> , 3>& pts) :
        point_ (pts) ,
        norm_ (std::move (VecMult (Vector (pts[1] , pts[2]) , Vector (pts[0] , pts[2]))))
    {}
    template<typename T>
    template<typename U>
    Triangle<T>::Triangle (const std::array< Point<U , Dimension> , 3>& pts)
    {
        std::array<Point<T , Dimension> , Dimension> arr = { MLib::Point{pts[0]}, MLib::Point{pts[1]}, MLib::Point{pts[2]} };

        point_ = std::move (arr);
        norm_ = std::move (VecMult (Vector (point_[1] , point_[2]) , Vector (point_[0] , point_[2])));
    }

    template<typename T>
    Triangle<T>::Triangle (std::array< Point<T , Dimension> , 3>&& pts) :
        point_ (std::move (pts)) ,
        norm_ (std::move (VecMult (Vector (pts[1] , pts[2]) , Vector (pts[0] , pts[2]))))
    {}

    template<typename T>
    Triangle<T>::Triangle (const std::array<T , 9>& arr) :
        point_ { std::array{Point{std::array{arr[0], arr[1], arr[2]}}, Point{std::array{arr[3], arr[4], arr[5]}}, Point{std::array{arr[6], arr[7], arr[8]}}} } ,
        norm_ ()
    {
        norm_ = std::move (VecMult (Vector (point_[1] , point_[2]) , Vector (point_[0] , point_[2])));
    }

    template<typename T>
    Triangle<T>::Side Triangle<T>::GetSide (const Point<T , Dimension>& pt) const
    {
        T out = ScalMult (norm_ , Vector { point_[0], pt });
        if (out > 0)
            return Side::Front;
        if (out < 0)
            return Side::Back;
        return Side::Laying;
    }

    template<typename T>
    bool Triangle<T>::InterSect (const Triangle<T>& tr) const
    {
        std::array<Side , Dimension> sides;
        for (size_t i = 0; i < Dimension; ++i)
            sides[i] = GetSide (tr.point_[i]);
        for (size_t i = 0; i < Dimension - 1; ++i)
            if (sides[i] != sides[i + 1])
                return true;

        return false;
    }

} // namespace MLib

template <typename T>
using TrSize = MLib::Triangle<T>::Side;