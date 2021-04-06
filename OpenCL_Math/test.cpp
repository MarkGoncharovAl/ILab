#include <iostream>
#include <gtest/gtest.h>
#include "geometry/geometry.hpp"
#include "Libs/Random/Random.hpp"

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
    //std::cout << matr2 << std::endl;
}

TEST (Multiple , easy)
{
    matrix::Matrix<float> matr ({
        {1, 0, 6},
        {0, 2, 0},
        {0, 0, 3} });
    clM::Matrix clmatr (matr);
    clM::Vector vec (std::vector<float> {4 , 5 , 6});
    auto&& check = dirichle.Mult (clmatr , vec);
    // for (auto&& elem : check)
    //     std::cout << elem << std::endl;
}

TEST (Multiply , ord)
{
    MLib::Random rand (-100 , 100);
    matrix::Matrix<float> matr ({
        {static_cast<float>(rand.get ()), static_cast<float>(rand.get ()), static_cast<float>(rand.get ())},
        {static_cast<float>(rand.get ()), static_cast<float>(rand.get ()), static_cast<float>(rand.get ())},
        {static_cast<float>(rand.get ()), static_cast<float>(rand.get ()), static_cast<float>(rand.get ())} });

    clM::Matrix<float> my_matr (matr);
    clM::Vector vector (std::vector<float>{static_cast<float>(rand.get ()) , static_cast<float>(rand.get ()) , static_cast<float>(rand.get ())});

    auto check = matr;
    check.multiplication (matrix::Matrix<float> ({ {vector[0], 0, 0},{vector[1], 0, 0}, {vector[2] , 0, 0} }));
    auto&& my_asnwer = dirichle.Mult (my_matr , vector);

    // std::cout << matr << std::endl;
    // std::cout << my_matr << std::endl;
    // for (auto&& elem : vector)
    //     std::cout << elem << " ";

    // std::cout << "\n" << check;
    // for (auto&& elem : my_asnwer)
    //     std::cout << elem << "\n";
    // std::cout << "A\n";

    for (size_t i = 0; i < 3; ++i)
        ASSERT_EQ (check (i , 0) , my_asnwer[i]);
}

TEST (Operations , easy)
{
    clM::Vector vec1 (std::vector<float>{1 , 2 , 3});
    clM::Vector vec2 (std::vector<float>{4 , 5 , 6});
    clM::Vector vec3 (std::vector<float>{3 , 3 , 3});

    auto&& vec4 = vec2 - vec1;
    for (size_t i = 0; i < vec1.size (); ++i)
        EXPECT_EQ (vec4[i] , vec3[i]);

    clM::Vector vec5 (std::vector<float>{7 , 8 , 9});
    vec1 = vec3 + vec2;

    for (size_t i = 0; i < vec1.size (); ++i)
        EXPECT_EQ (vec1[i] , vec5[i]);
}

TEST(Multiply, hard)
{
    clM::Vector vec1 (std::vector<float>{1 , 2 , 3});
    clM::Vector vec2 (std::vector<float>{2 , 4 , 6});
    auto vec3 = dirichle.Mult(2, vec1);
     for (size_t i = 0; i < vec1.size (); ++i)
        EXPECT_EQ (vec2[i] , vec3[i]);
}