#include <iostream>
#include "MC_OpenCL/MC_OpenCL.hpp"
#include <Logging.hpp>
#include "geometry/geometry.hpp"
#include <array>
#include <vector>
#include <cmath>

constexpr size_t size_boundaries = 2;

static float GetPeriod ();
static std::array<cl_float2 , size_boundaries> GetBoundaries ();
static std::vector<float> GetBondCond (float age , std::array<cl_float2 , size_boundaries> const& bound);

static std::pair<matrix::Matrix<float> , std::vector<float>>
PrepareEquation (std::vector<float> const& bondCond);

static void PrintResult (matrix::Matrix<float> const& matr);
static void PrintResult (clM::Vector const& vec);

int main ()
{
    try
    {
        auto&& age = GetPeriod ();
        auto&& boundaries = GetBoundaries ();
        auto&& bondCond = GetBondCond (age , boundaries);
        auto&& answer = PrepareEquation (bondCond);

        PrintResult (answer.first.solve (answer.second).first); //CPU - Dima's matrix)

        clM::Matrix<float> cl_matrix (answer.first); //diagonal fast matrix
        clM::Vector cl_vector (std::move (answer.second)); //convenient wrapper
        auto&& final_result = clM::Dirichle {}.Solve (cl_matrix , cl_vector);

        PrintResult (final_result); //GPU
    }
    catch (cl::Error& err)
    {
        err.what ();
    }
    catch (std::exception& err)
    {
        err.what ();
    }

    return 0;
}

std::pair<matrix::Matrix<float> , std::vector<float>>
PrepareEquation (std::vector<float> const& bondCond)
{
    size_t size_cond = bondCond.size ();
    size_t size_raw = size_cond / 4 - 1;
    size_t size_solving = std::pow (size_raw , 2);

    matrix::Matrix<float> solving (size_solving , size_solving);
    for (auto&& iter = solving.begin (); iter != solving.end (); ++iter)
        *iter = 0;
    std::vector<float> result (size_solving , 0);

    //preparing free coeffs
    for (size_t i = 0; i < size_raw; ++i)
        result[i] -= bondCond[i + 1]; //up
    for (size_t i = 0; i < size_raw; ++i)
        result[size_raw - 1 + i * size_raw] -= bondCond[size_raw + 2 + i]; //right
    for (size_t i = 0; i < size_raw; ++i)
        result[i * size_raw] -= bondCond[size_cond - 1 - i]; //left
    for (size_t i = 0; i < size_raw; ++i)
        result[size_raw * (size_raw - 1) + i] -= bondCond[size_cond - size_raw - 2 - i]; //down

    // for (auto&& elem : result)
    //     std::cout << elem << "\n";

    //preparing diag matrix
    for (size_t i = 0; i < size_solving; ++i)
    {
        solving (i , i) = -4;
        if (i > 0 && i % size_raw != 0)
            solving (i , i - 1) = 1;
        if (i >= size_raw)
            solving (i , i - size_raw) = 1;
        if (i < size_solving - 1 && i % size_raw != size_raw - 1)
            solving (i , i + 1) = 1;
        if (i < size_solving - size_raw)
            solving (i , i + size_raw) = 1;
    }

    //std::cout << solving;

    return std::make_pair (std::move (solving) , std::move (result));
}

float GetPeriod ()
{
    float age = 0;
    std::cin >> age;
    if (!std::cin)
    {
        LOG_fatal << "Can't read age of the function!";
        throw std::runtime_error ("Can't read age of the function!");
    }

    return age;
}

std::array<cl_float2 , size_boundaries> GetBoundaries ()
{
    std::array<cl_float2 , size_boundaries> boundaries;
    for (auto& boundary : boundaries)
        for (auto& elem : boundary.s)
            std::cin >> elem;

    if (!std::cin)
    {
        LOG_fatal << "Can't properly read boundaries!";
        throw std::runtime_error ("Can't properly read boundaries!");
    }

    return boundaries;
}

std::vector<float> GetBondCond (float age , std::array<cl_float2 , size_boundaries> const& bound)
{
    size_t size = 2 * static_cast<size_t>((std::fabs (bound[1].s[0] - bound[0].s[0]) + std::fabs (bound[1].s[1] - bound[0].s[1])) / age);
    std::vector<float> data;
    data.reserve (size);

    for (size_t i = 0; i < size; ++i)
    {
        float elem = 0;
        std::cin >> elem;
        data.push_back (elem);
    }

    if (!std::cin)
    {
        LOG_fatal << "Can't properly read conditions in boundaries!";
        throw std::runtime_error ("Can't properly read conditions in boundaries!");
    }

    return data;
}

void PrintResult (matrix::Matrix<float> const& matr)
{
    std::cout << "CPU result:\n";
    size_t size_raw = std::sqrt (matr.getLines ());
    for (size_t i = 0 , j = 0; i < matr.getLines (); ++i , ++j)
    {
        if (j == size_raw)
        {
            std::cout << "\n";
            j = 0;
        }
        std::cout << matr (i , 0) << " ";
    }
    std::cout << "\n";
}

void PrintResult (const clM::Vector& vec)
{
    std::cout << "GPU result:\n";
    size_t size_raw = std::sqrt (vec.size ());
    for (size_t i = 0 , j = 0; i < vec.size (); ++i , ++j)
    {
        if (j == size_raw)
        {
            std::cout << "\n";
            j = 0;
        }
        std::cout << vec[i] << " ";
    }
    std::cout << "\n";
}