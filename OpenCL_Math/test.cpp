#include <iostream>
#include <gtest/gtest.h>
#include "geometry/geometry.hpp"

//!Create tests for operator *, operator + and so on for vector

static clM::Dirichle dirichle;

int main (int argc , char* argv [])
{
    testing::InitGoogleTest (&argc , argv);
    return RUN_ALL_TESTS ();
}

TEST (Ctor , check)
{
    matrix::Matrix<float> matr ({
        {1, 5, 0, 0},
        {0, 2, 6, 0},
        {0, 0, 3, 0},
        {7, 0, 0, 4} });
    clM::Matrix<float> matr2 (matr);
    std::cout << matr2 << std::endl;
}

TEST (Multiple , easy)
{
    matrix::Matrix<float> matr ({
        {1, 0, 6},
        {0, 2, 0},
        {0, 0, 3} });
    clM::Matrix clmatr (matr);
    clM::Vector vec (std::vector<float> {4 , 5 , 6});
    auto&& check = dirichle.Multiply (clmatr , vec);
    for (auto&& elem : check)
        std::cout << elem << std::endl;
}