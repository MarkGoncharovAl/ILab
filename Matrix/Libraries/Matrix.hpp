#pragma once

#include <iostream>
#include <cmath>
#include <vector>
#include "../Common_libs/errors.hpp"
#include "Matrix_Class.hpp"
#include "../Common_libs/iterators.hpp"

namespace MF
{

    // For double only, because
    //  1) if I have int - it will be difficult to realise
    //  LU decompos, because I've used 1 / 2 and waiting 0.5, not 0
    //  In that case I will have to rewrite LU only for int
    //  Or I have to static_cast<double>... I don't wanna do this
    //
    //  2) it's so weird to use determ with std::string, sf::Vector
    //  And other specific types
    enum class Determ_sign
    {
        nothing,
        minus,
        zero
    };

    template <class T>
    double Determ(const Matrix_t<T> &matrix);

} // namespace MF

//Matrix Funcitons
namespace MF_help
{
    constexpr double approx = 0.00001;
    using VecRaw_t = std::vector<Matrix_t<double>::SetterRaw_t>;

    VecRaw_t TriangleMatrix(Matrix_t<double> &matrix, MF::Determ_sign *swappes = nullptr);
    void NormalizeMatrix(VecRaw_t &matrix);

    //  About Next function
    //  1 2 3
    //  2 3 4
    //  --> Func(str[0], str[1], 1)
    //   1   2   3
    //  0.5  0 -0.5
    //  !WRITE CHEKING base == current_str
    bool ZeroOutElem(const Matrix_t<double>::SetterRaw_t &base,
                     Matrix_t<double>::SetterRaw_t current_str,
                     size_t num);

    enum class CZUMout
    {
        nothing,
        swapped,
        zero
    };

    CZUMout CreateZeroUnderMatrix(VecRaw_t &vec,
                                  size_t num);

    template <class T>
    std::vector<typename Matrix_t<T>::SetterRaw_t>
    GetVectorRaws(Matrix_t<T> &matrix);
} // namespace MF_help

template <class T>
std::vector<typename Matrix_t<T>::SetterRaw_t>
MF_help::GetVectorRaws(Matrix_t<T> &matrix)
{
    size_t max_size = matrix.GetSizeRaw();

    std::vector<typename Matrix_t<T>::SetterRaw_t> data{max_size};
    for (size_t i = 0; i < max_size; ++i)
        data[i] = matrix[i];

    return data;
}

template <class T>
double MF::Determ(const Matrix_t<T> &matrix)
{
    if (!matrix.IsSquare())
    {
        matrix.dump();
        WARNING("Determinant can't be found on not square matrix! Upper is dump of this matrix:");
        return 0;
    }

    Determ_sign swappes = Determ_sign::nothing;
    double result = 1;

    Matrix_t<double> temp{matrix};
    MF_help::VecRaw_t tr_matrix = MF_help::TriangleMatrix(temp, &swappes);

    // Only dumping is below!
    // std::cout << "\nStarting!\n";
    // for (auto &var : tr_matrix)
    // {
    //     for (size_t i = 0; i < tr_matrix.size(); ++i)
    //         std::cout << var[i] << "\t";
    //     std::cout << std::endl;
    // }
    switch (swappes)
    {
    case Determ_sign::zero:
        return 0;
    case Determ_sign::minus:
        result = -1;
        break;
    case Determ_sign::nothing:
        break;
    }

    for (size_t i = 0; i < tr_matrix.size(); ++i)
        result *= tr_matrix[i][i];

    return result;
}