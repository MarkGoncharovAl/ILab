#include <gtest/gtest.h>
#include "Common_libs/random.hpp"
#include <iostream>
#include "Libraries/Matrix.hpp"

int main(int argc, char *argv[])
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(COPY_CONST, Ordinary)
{
    Matrix_t<int> arr1(3, 6);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 6; ++j)
            arr1.Set(i, j, i + j);
    }

    Matrix_t<int> arr2{arr1};
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 6; ++j)
            EXPECT_EQ(arr1[i][j], arr2[i][j]);
    }
}
TEST(COPY_CONST, Empty_data)
{
    Matrix_t<int> arr1;
    std::cout << "THERE SHOULD BE WARNING!\n";
    Matrix_t<int> arr2{arr1};
    EXPECT_FALSE(arr1);
    EXPECT_FALSE(arr2);
}
TEST(COPY_CONST, Same_data)
{
    Matrix_t<int> arr1;
    std::cout << "THERE SHOULD BE WARNING!\n";
    arr1 = arr1;

    EXPECT_FALSE(arr1);

    Matrix_t<int> arr2(1, 1);
    arr2.Set(0, 0, 6);
    arr2 = arr2;
    EXPECT_TRUE(arr2);
    EXPECT_TRUE(arr2[0][0] == 6);
}

TEST(COPY_CONST, operator_eq)
{
    Matrix_t<int> arr1{2, 3};
    arr1.InitElems({{1, 2, 3}, {4, 5, 6}});

    Matrix_t<int> arr2;
    arr2 = arr1;
    //arr2.dump();
    EXPECT_EQ(arr1, arr2);
}

TEST(MOVE_CONST, Checking)
{
    Matrix_t<int> arr(3, 6);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 6; ++j)
            arr.Set(i, j, i + j);
    }

    Matrix_t<int> arr2{std::move(arr)};
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 6; ++j)
            EXPECT_EQ(arr2[i][j], i + j);
    }
}

TEST(INIT_MEM, Cheking)
{
    Matrix_t<int> arr;
    arr.InitMemory(3, 6);
    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 6; ++j)
            arr.Set(i, j, i + j + 1);
    }
    //it will give a problem if memory
    //wasn't initialized

    //arr.InitMemory(140, 0);
    //EXPECT_FALSE(arr);
}

TEST(INIT_ELEMS, to_check)
{
    Matrix_t<int> arr(3, 6);
    Matrix_t<int> empty;
    //empty.InitElems({{2}});

    arr.InitElems({{1, 2, 3, 4, 5, 6},
                   {2, 3, 4, 5, 6, 7},
                   {3, 4, 5, 6, 7, 8}});
    //arr.dump();
}

TEST(EQUALS, Empty)
{
    Matrix_t<int> arr1;
    Matrix_t<int> arr2;
    ASSERT_TRUE(arr1 == arr2);
}

TEST(EQUALS, Not_equal)
{
    Matrix_t<int> arr1{2, 1};
    Matrix_t<int> arr2{1, 2};
    ASSERT_FALSE(arr1 == arr2);
}

TEST(EQUALS, Ordinary)
{
    Matrix_t<int> arr1{1, 2};
    arr1.InitElems({{1, 2}});
    Matrix_t<int> arr2{1, 2};
    arr2.InitElems({{1, 2}});
    ASSERT_TRUE(arr1 == arr2);
}

TEST(EQUALS, Ordinary_Not_equal)
{
    Matrix_t<int> arr1{1, 2};
    arr1.InitElems({{1, 2}});
    Matrix_t<int> arr2{1, 2};
    arr2.InitElems({{1, 4}});
    ASSERT_TRUE(arr1 != arr2);
}

TEST(ZeroOutElem, FALSE)
{
    Matrix_t<double> arr1{3, 6};
    Matrix_t<double> arr2{2, 5};
    Matrix_t<double> arr3{2, 5};
    EXPECT_FALSE(MF_help::ZeroOutElem(arr2[1], arr1[0], 7));
    ASSERT_FALSE(MF_help::ZeroOutElem(arr2[1], arr3[0], 7));
}

TEST(ZeroOutElem, Zero_Out)
{
    Matrix_t<double> arr1{2, 3};
    Matrix_t<double> arr2{3, 3};
    Matrix_t<double> arr3{3, 3};

    EXPECT_FALSE(MF_help::ZeroOutElem(arr2[1], arr1[0], 7));
    ASSERT_FALSE(MF_help::ZeroOutElem(arr2[1], arr3[0], 7));
}

TEST(ZeroOutElem, Ordinary1)
{
    Matrix_t<double> arr{2, 3};
    arr.InitElems({{1, 2, 3},
                   {2, 3, 4}});

    Matrix_t<double> control1{2, 3};
    control1.InitElems({{1, 2, 3},
                        {0.5, 0, -0.5}});

    EXPECT_TRUE(MF_help::ZeroOutElem(arr[0], arr[1], 1));
    ASSERT_TRUE(control1 == arr);
}

TEST(ZeroOutElem, Ordinary2)
{
    Matrix_t<double> arr{2, 3};
    arr.InitElems({{1, 2, 3},
                   {2, 3, 4}});

    Matrix_t<double> control1{2, 3};
    control1.InitElems({{-0.5, 2 - 9.0 / 4.0, 0},
                        {2, 3, 4}});

    EXPECT_TRUE(MF_help::ZeroOutElem(arr[1], arr[0], 2));
    ASSERT_TRUE(control1 == arr);
}

TEST(CreateZeroUnderMatrix, Check)
{
    Matrix_t<double> temp{3, 3};
    temp.InitElems({{0, 1, 2},
                    {1, 2, 3},
                    {2, 3, 4}});

    std::vector<Matrix_t<double>::SetterRaw_t>
        data{temp.GetSizeRaw()};
    for (size_t i = 0; i < temp.GetSizeRaw(); ++i)
        data[i] = std::move(temp[i]);

    EXPECT_TRUE(MF_help::CreateZeroUnderMatrix(data, 0) == MF_help::CZUMout::swapped);

    temp.InitElems({{0, 0, 0},
                    {0, 1, 0},
                    {0, 0, 0}});

    EXPECT_TRUE(MF_help::CreateZeroUnderMatrix(data, 1) == MF_help::CZUMout::zero);
    EXPECT_TRUE(MF_help::CreateZeroUnderMatrix(data, 1) == MF_help::CZUMout::zero);
    EXPECT_TRUE(MF_help::CreateZeroUnderMatrix(data, 2) == MF_help::CZUMout::zero);
}

TEST(ITERATORS, Easy)
{
    Matrix_t<int> arr{3, 6};

    int num = 0;
    for (auto iter = arr.begin(); iter != arr.end(); ++iter)
    {
        *iter = num;
        num++;
    }

    Matrix_t<int> correct{3, 6};
    correct.InitElems({{0, 1, 2, 3, 4, 5},
                       {6, 7, 8, 9, 10, 11},
                       {12, 13, 14, 15, 16, 17}});

    //arr.dump();

    ASSERT_TRUE(arr == correct);
}

TEST(ITERATORS, Row)
{
    Matrix_t<int> arr{3, 6};
    arr.InitElems({{0, 1, 2, 3, 4, 5},
                   {6, 7, 8, 9, 10, 11},
                   {12, 13, 14, 15, 16, 17}});

    auto row = arr[1];
    int num = 6;

    for (auto iter = row.begin(); iter != row.end(); ++iter)
    {
        ASSERT_EQ(num, *iter);
        num++;
    }
}

TEST(ALGEBRA, Easy)
{
    Matrix_t<int> arr{2, 2};
    arr.InitElems({{1, 2},
                   {3, 4}});
    //Matrix_t<int> arr1;
    //arr += arr1;

    //Matrix_t<int> arr1{2, 3};
    //arr += arr1;

    Matrix_t<int> arr1{2, 2};
    arr1.InitElems({{1, 2},
                    {3, 4}});

    Matrix_t<int> correct{2, 2};
    correct.InitElems({{2, 4},
                       {6, 8}});

    EXPECT_TRUE(arr + arr1 == correct);

    Matrix_t<int> arr2{2, 2};
    arr2.InitElems({{1, 2},
                    {3, 4}});

    correct.InitElems({{1, 2},
                       {3, 4}});

    EXPECT_TRUE(arr + arr1 - arr2 == correct);
}

TEST(ALGEBRA, Transposing)
{
    Matrix_t<int> arr{2, 2};
    arr.InitElems({{1, 2},
                   {3, 4}});

    Matrix_t<int> correct{2, 2};
    correct.InitElems({{1, 3},
                       {2, 4}});

    EXPECT_TRUE(arr.transpose() == correct);

    Matrix_t<int> arr2{3, 3};
    arr2.InitElems({{1, 2, 3},
                    {4, 5, 6},
                    {7, 8, 9}});

    Matrix_t<int> correct2{3, 3};
    correct2.InitElems({{1, 4, 7},
                        {2, 5, 8},
                        {3, 6, 9}});

    EXPECT_TRUE(arr2.transpose() == correct2);
}

//////////////////////////////////////////////
//////////////////////////////////////////////
//  !Final tests for determing

TEST(DETERM, Controlling)
{
    //Matrix_t<double> arr;
    //MF::Determ(arr);

    //Matrix_t<double> arr{2, 3};
    //MF::Determ(arr);

    Matrix_t<int> arr{2, 2};
    arr.InitElems({{0, 1},
                   {1, -6745635}});
    EXPECT_NEAR(MF::Determ(arr), -1, 0.0000001);

    Matrix_t<int> arr3{3, 3};
    arr3.InitElems({{1, 2, 3},
                    {0, 1, 0},
                    {3, 4, 5}});

    EXPECT_NEAR(MF::Determ(arr3), -4, 0.0000001);

    arr3.InitElems({{1, 2, 3},
                    {1, 1, 0},
                    {3, 4, 5}});

    EXPECT_NEAR(MF::Determ(arr3), -2, 0.0000001);
}

TEST(DETERM, Hard_Small_Tests)
{
    Matrix_t<double> arr3{3, 3};
    arr3.InitElems({{1, 2, 3},
                    {1, 2, 3},
                    {37645, -5434, 5.7643}});

    EXPECT_NEAR(MF::Determ(arr3), 0, 0.0000001);

    arr3.InitElems({{0, 0, 0},
                    {0, 0, 0},
                    {0, 0, 0}});

    EXPECT_NEAR(MF::Determ(arr3), 0, 0.0000001);

    arr3.InitElems({{1, 2, 3},
                    {5, 2, 3},
                    {9, 2, 3}});

    EXPECT_NEAR(MF::Determ(arr3), 0, 0.0000001);

    Matrix_t<double> arr4{6, 6};
    for (double &elem : arr4)
        elem = 0;
    //arr4.dump();

    for (int i = 0; i < 6; ++i)
        arr4[i][i] = 1;

    Random random(0, 5);
    auto raw = arr4[random.get()];
    for (auto iter = raw.begin(); iter != raw.end(); ++iter)
        *iter = 2;

    //arr4.dump();
    //
    //  1 0 0 0 0 0
    //  0 1 0 0 0 0
    //  0 0 1 0 0 0
    //  0 0 0 1 0 0
    //  0 0 0 0 1 0
    //  2 2 2 2 2 2
    EXPECT_NEAR(MF::Determ(arr4), 2, 0.0000001);
}

// TEST(DETERM, Hard_Big_Tests)
// {
//     Random gen{-10000, 10000};

//     Matrix_t<double> arr{20, 20};

//     //!We will do 2 same raws

//     for (double &elem : arr)
//         elem = gen.get();

//     int rand_raw = (Random{1, 19}).get();
//     for (int i = 0; i < 20; ++i)
//         arr[0][i] = arr[rand_raw][i];

//     //arr.dump();
//     EXPECT_NEAR(MF::Determ(arr), 0, 0.0000001);

//     //!We will do 2 same collumns

//     for (double &elem : arr)
//         elem = gen.get();

//     int rand_col = (Random{0, 18}).get();
//     for (int i = 0; i < 20; ++i)
//         arr[i][19] = arr[i][rand_col];

//     //arr.dump();
//     EXPECT_NEAR(MF::Determ(arr), 0, 0.0000001);
// }

// TEST(DETERM, REALLY_BIG1)
// {
//     Matrix_t<int> big1{1000, 1000, 0};
//     Random generator{-100000, 100000};

//     for (size_t i = 0; i < 1000; ++i)
//     {
//         big1[i][i] = 1;
//         for (size_t j = i + 1; j < 1000; ++j)
//             big1[i][j] = generator.get();
//     }
//     ASSERT_NEAR(MF::Determ(big1), 1, 0.00001);
// }

// TEST(DETERM, REALLY_BIG2)
// {
//     //треугольные начинаем перемещать строки и столбцы
//     Matrix_t<double> big2{1000, 1000, 0};
//     Random generator{1, 100000};

//     for (size_t i = 0; i < 1000; ++i)
//     {
//         big2[i][i] = 1;
//         for (size_t j = i + 1; j < 1000; ++j)
//             big2[i][j] = generator.get();
//     }

//     Random gen2{1, 499};
//     int a = gen2.get();
//     int b = a + gen2.get();

//     for (int i = 0; i < 1000; ++i)
//     {
//         int temp = big2[a][i];
//         big2.Set(a, i, big2[b][i]);
//         big2.Set(b, i, temp);
//     }
//     EXPECT_NEAR(MF::Determ(big2), -1, 0.00001);

//     for (int i = 0; i < 1000; ++i)
//     {
//         int temp = big2[a][i];
//         big2.Set(a, i, big2[b][i]);
//         big2.Set(b, i, temp);
//     }

//     ASSERT_NEAR(MF::Determ(big2), 1, 0.00001);
// }

// TEST(DETERM, REALLY_BIG3)
// {
//     //2 пропорциональные строки
//     Matrix_t<double> big3{1000, 1000, 1};

//     Random generator{1, 499};
//     int a = generator.get();
//     int b = a + generator.get();

//     Random gen{-100, 100};
//     for (auto iter = big3.begin(); iter != big3.end(); ++iter)
//         *iter = gen.get();

//     auto Raw1 = big3[a];
//     auto Raw2 = big3[b];
//     const double coeff = gen.get();

//     for (size_t i = 0; i < big3.GetSizeCol(); ++i)
//     {
//         Raw2[i] = Raw1[i] * coeff;
//     }

//     EXPECT_NEAR(MF::Determ(big3), 0, MF::approx);
// }

// TEST(DETERM, REALLY_BIG4)
// {
//     //строка - лин комбинация двух других
//     Matrix_t<double> big4{1000, 1000, 1};

//     Random generator{1, 200};
//     int a = generator.get();
//     int b = a + generator.get();
//     int c = b + generator.get();

//     Random gen{-80, 80};
//     for (auto iter = big4.begin(); iter != big4.end(); ++iter)
//         *iter = gen.get();

//     auto Raw1 = big4[a];
//     auto Raw2 = big4[b];
//     auto Raw3 = big4[c];
//     const double coeff1 = gen.get();
//     const double coeff2 = gen.get();

//     for (size_t i = 0; i < big4.GetSizeCol(); ++i)
//     {
//         Raw3[i] = Raw1[i] * coeff1 + Raw2[i] * coeff2;
//     }

//     EXPECT_NEAR(MF::Determ(big4), 0, MF::approx);
// }

// TEST(DETERM, REALLY_BIG5)
// {
//     //transposing

//     Matrix_t<double> arr{100, 100, 0};
//     Random gen{-100, 100};
//     for (auto iter = arr.begin(); iter != arr.end(); ++iter)
//         *iter = gen.get();

//     Matrix_t<double> copy = arr.transpose();
//     EXPECT_NEAR(MF::Determ(arr), MF::Determ(copy), MF::approx);
// }

TEST(TriangleMatrix, Easy1)
{
    Matrix_t<double> arr{1, 3, 0};
    arr.InitElems({{1, 2, 3}});

    auto triangle = MF_help::TriangleMatrix(arr);
    Matrix_t<double> out{1, 3, 0};
    out.InitElems({{1, 2, 3}});
    for (int i = 0; i < 3; ++i)
        EXPECT_EQ(triangle[0][i], out[0][i]);
}

TEST(TriangleMatrix, Easy2)
{
    Matrix_t<double> arr{2, 3, 0};
    arr.InitElems({{1, 2, 3},
                   {2, 3, 4}});

    auto triangle = MF_help::TriangleMatrix(arr);
    Matrix_t<double> out{2, 3, 0};
    out.InitElems({{1, 2, 3},
                   {0, -1, -2}});

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_EQ(triangle[i][j], out[i][j]);
}

TEST(TriangleMatrix, Easy3)
{
    Matrix_t<double> arr{2, 3, 0};
    arr.InitElems({{1, 2, 3},
                   {2, 4, 6}});

    auto triangle = MF_help::TriangleMatrix(arr);
    Matrix_t<double> out{2, 3, 0};
    out.InitElems({{1, 2, 3},
                   {0, 0, 0}});

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 3; ++j)
            EXPECT_EQ(triangle[i][j], out[i][j]);
}

TEST(TriangleMatrix, Easy4)
{
    Matrix_t<double> arr{3, 2, 0};
    arr.InitElems({{1, 2},
                   {2, 3},
                   {3, 4}});

    auto triangle = MF_help::TriangleMatrix(arr);
    Matrix_t<double> out{3, 2, 0};
    out.InitElems({{1, 2},
                   {0, -1},
                   {0, 0}});

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 2; ++j)
        {
            //  std::cout << triangle[i][j] << "\t";
            EXPECT_EQ(triangle[i][j], out[i][j]);
        }
        //std::cout << std::endl;
    }
}

TEST(TriangleMatrix, Ord1)
{
    Matrix_t<double> arr{2, 4, 0};
    arr.InitElems({{0, 2, 4, 6},
                   {1, 2, 3, 4}});

    auto triangle = MF_help::TriangleMatrix(arr);
    Matrix_t<double> out{2, 4, 0};
    out.InitElems({{1, 2, 3, 4},
                   {0, 2, 4, 6}});

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 4; ++j)
            EXPECT_EQ(triangle[i][j], out[i][j]);
}

TEST(TriangleMatrix, Ord2)
{
    Matrix_t<double> arr{4, 2, 0};
    arr.InitElems({{0, 2},
                   {1, 2},
                   {2, 3},
                   {3, 4}});

    auto triangle = MF_help::TriangleMatrix(arr);
    Matrix_t<double> out{4, 2, 0};
    out.InitElems({{1, 2},
                   {0, 2},
                   {0, 0},
                   {0, 0}});

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 2; ++j)
            EXPECT_EQ(triangle[i][j], out[i][j]);
}

TEST(TriangleMatrix, Ord3)
{
    Matrix_t<double> arr{4, 2, 0};
    arr.InitElems({{0, 2},
                   {0, 2},
                   {0, 3},
                   {3, 4}});

    auto triangle = MF_help::TriangleMatrix(arr);
    Matrix_t<double> out{4, 2, 0};
    out.InitElems({{3, 4},
                   {0, 2},
                   {0, 0},
                   {0, 0}});

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 2; ++j)
            EXPECT_EQ(triangle[i][j], out[i][j]);
}

// TEST(TriangleMatrix, Ord4)
// {
//     Matrix_t<double> arr{4, 2, 0};
//     arr.InitElems({{0, 2},
//                    {0, 3},
//                    {0, 4},
//                    {0, 5}});

//     auto triangle = MF_help::TriangleMatrix(arr);
//     Matrix_t<double> out{4, 2, 0};
//     out.InitElems({{0, 2},
//                    {0, 0},
//                    {0, 0},
//                    {0, 0}});

//     for (int i = 0; i < 4; ++i)
//         for (int j = 0; j < 2; ++j)
//             EXPECT_EQ(triangle[i][j], out[i][j]);
// }

TEST(NormalizeMatrix, Easy1)
{
    Matrix_t<double> arr{2, 4, 0};
    arr.InitElems({{0, 2, 4, 6},
                   {1, 2, 3, 4}});

    auto triangle = MF_help::TriangleMatrix(arr);
    MF_help::NormalizeMatrix(triangle);

    Matrix_t<double> out{2, 4, 0};
    out.InitElems({{1, 2, 3, 4},
                   {0, 1, 2, 3}});

    for (int i = 0; i < 2; ++i)
        for (int j = 0; j < 4; ++j)
            EXPECT_EQ(triangle[i][j], out[i][j]);
}

TEST(NormalizeMatrix, Easy2)
{
    Matrix_t<double> arr{4, 2, 0};
    arr.InitElems({{0, 2},
                   {1, 2},
                   {2, 3},
                   {3, 4}});

    auto triangle = MF_help::TriangleMatrix(arr);
    MF_help::NormalizeMatrix(triangle);

    Matrix_t<double> out{4, 2, 0};
    out.InitElems({{1, 2},
                   {0, 1},
                   {0, 0},
                   {0, 0}});

    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 2; ++j)
            EXPECT_EQ(triangle[i][j], out[i][j]);
}

TEST(ITERATORS, operator_input)
{
    std::ifstream in{"../Tests/1.txt"};
    EXPECT_EQ(in.is_open(), true);

    Matrix_t<int> reading{2, 3};
    Matrix_t<int> checking{2, 3};
    in >> reading;
    checking.InitElems({{1, 2, 3}, {4, 5, 6}});

    //reading.dump();
    EXPECT_EQ(reading, checking);
    in.close();
}