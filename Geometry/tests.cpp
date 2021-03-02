#include "Base/Point.hpp"
#include "Base/Vector.hpp"
#include "Base/Surface.hpp"
#include "Base/Geometry.hpp"
#include <gtest/gtest.h>

int main (int argc , char* argv [])
{
    ::testing::InitGoogleTest (&argc , argv);
    return RUN_ALL_TESTS ();
}

TEST (Point , operators)
{
    MLib::Point pt = std::array { 1, 2, 3 , 4 };
    pt += pt;
    MLib::Point out = std::array { 2, 4, 6 , 8 };

    ASSERT_TRUE (pt == out);
}

TEST (ScalMult , first)
{
    MLib::Vector vec = std::array { 1, 2, 3 };
    ASSERT_TRUE (MLib::ScalMult (vec , vec) == (1 + 4 + 9));
}

TEST (VecMult , first)
{
    MLib::Vector vec1 = std::array { 0, 1, 1 };
    MLib::Vector vec2 = std::array { 1, 0, 1 };

    MLib::Vector out1 = std::array { 0, 0, 0 };
    MLib::Vector out2 = std::array { 1, 1, -1 };

    auto check1 = MLib::VecMult (vec1 , vec1);
    auto check2 = MLib::VecMult (vec1 , vec2);
    ASSERT_TRUE (check1 == out1);
    ASSERT_TRUE (check2 == out2);
}

TEST (GetSize , first)
{
    MLib::Surface surf { MLib::Point (std::array{0, 0, 0}), MLib::Point (std::array{1, 0, 0}), MLib::Point (std::array{0, 1, 0}) };
    auto sideUp = surf.GetSide (MLib::Point { std::array{0, 0, 1} });
    auto sideDown = surf.GetSide (MLib::Point { std::array{0, 0, -1} });
    auto sideLay = surf.GetSide (MLib::Point { std::array{8, 7, 0} });
    ASSERT_TRUE (sideUp == SurfSize<int>::Back);
    ASSERT_TRUE (sideDown == SurfSize<int>::Front);
    ASSERT_TRUE (sideLay == SurfSize<int>::Laying);
}

TEST (InterSect , first)
{
    MLib::Surface surf { MLib::Point (std::array{0, 0, 0}), MLib::Point (std::array{1, 0, 0}), MLib::Point (std::array{0, 1, 0}) };
    std::array points = { MLib::Point (std::array{0, 0, 1}), MLib::Point (std::array{0, 1, 1}), MLib::Point (std::array{1, 0, 1}) };
    std::array points2 = { MLib::Point (std::array{0, 0, 1}), MLib::Point (std::array{0, 1, 1}), MLib::Point (std::array{1, 0, 0}) };
    std::array points3 = { MLib::Point (std::array{0, 0, 1}), MLib::Point (std::array{0, 1, 1}), MLib::Point (std::array{1, 0, -1}) };
    ASSERT_TRUE (surf.InterSect (points) == false);
    ASSERT_TRUE (surf.InterSect (points2) == true);
    ASSERT_TRUE (surf.InterSect (points3) == true);
}

TEST(InterSect, second)
{
    std::array points = { MLib::Point (std::array{0, 0, 0}), MLib::Point (std::array{0, 3, 0}), MLib::Point (std::array{3, 0, 0}) };
    std::array points2 = { MLib::Point (std::array{0, 0, 1}), MLib::Point (std::array{1, 1, 0}), MLib::Point (std::array{1, 0, 1}) };
    std::array points3 = { MLib::Point (std::array{1, 0, 100}), MLib::Point (std::array{1, 1, -1}), MLib::Point (std::array{0, 1, 100}) };
    std::array points4 = { MLib::Point (std::array{1, 0, 100}), MLib::Point (std::array{1, 1, 1}), MLib::Point (std::array{0, 1, 100}) };
    MLib::Triangle<double> tr1 = points;
    MLib::Triangle<double> tr2 = points2;
    MLib::Triangle<double> tr3 = points3;
    MLib::Triangle<double> tr4 = points4;

    std::array check = {MLib::Intersect(tr1, tr2), MLib::Intersect(tr1, tr3), !MLib::Intersect(tr1, tr4)}; 
    for (auto elem : check)
    ASSERT_TRUE(elem);
}