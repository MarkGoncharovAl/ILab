#include "Matrix.hpp"

// For double only, because
//  1) if I have int - it will be difficult to realise
//  LU decompos, because I've used 1 / 2 and waiting 0.5, not 0
//  In that case I will have to rewrite LU only for int
//  Or I have to static_cast<double>... I don't wanna do this
//
//  2) it's so weird to use determ with std::string, sf::Vector
//  And other specific types

bool MF_help::ZeroOutElem(const Matrix_t<double>::SetterRaw_t &base,
                          Matrix_t<double>::SetterRaw_t current_str,
                          size_t num)
{
    if (base.size() != current_str.size() ||
        num >= base.size() ||
        (std::fabs(base[num]) < MF_help::approx &&
         std::fabs(current_str[num]) > MF_help::approx))
    {
        return false;
    }

    if (std::fabs(current_str[num]) < MF_help::approx)
        return true;

    double coef = -1 * current_str[num] / base[num];

    for (size_t i = 0; i < current_str.size(); ++i)
    {
        current_str[i] += coef * base[i];
    }
    return true;
}

MF_help::CZUMout MF_help::CreateZeroUnderMatrix(VecRaw_t &vec,
                                                size_t num)
{
    if (std::fabs(vec[num][num]) > MF_help::approx)
        return CZUMout::nothing;

    for (size_t i = num + 1; i < vec.size(); ++i)
    {
        if (std::fabs(vec[i][num]) > MF_help::approx)
        {
            std::swap(vec[num], vec[i]);
            return CZUMout::swapped;
        }
    }

    return CZUMout::zero;
}

MF_help::VecRaw_t
MF_help::TriangleMatrix(Matrix_t<double> &matrix, MF::Determ_sign *swappes /* = nullptr*/)
{
    if (!matrix)
    {
        WARNING("Can't find determinant of not initialized matrix!");
        return VecRaw_t{};
    }

    //now everything is ok
    size_t min_size = std::min(matrix.GetSizeRaw(), matrix.GetSizeCol());
    size_t col_size = matrix.GetSizeRaw();

    auto data = MF_help::GetVectorRaws(matrix);

    char swap = 0;
    bool zero = false;

    for (size_t i = 0; i < min_size; ++i)
    {
        switch (CreateZeroUnderMatrix(data, i))
        {
        case CZUMout::swapped:
            swap++;
            break;
        case CZUMout::zero:
            zero = true;
            break;
        case CZUMout::nothing:
            break;
        }

        for (size_t j = i + 1; j < col_size; ++j)
        {
            ZeroOutElem(data[i], data[j], i);
        }
    }

    if (swappes != nullptr)
    {
        if (zero)
            *swappes = MF::Determ_sign::zero;
        else
        {
            if (swap % 2 == 0)
                *swappes = MF::Determ_sign::nothing;
            else
                *swappes = MF::Determ_sign::minus;
        }
    }
    // Only dumping is below!
    // std::cout << "\nStarting!\n";
    // for (auto &var : data)
    // {
    //     for (size_t i = 0; i < max_size; ++i)
    //         std::cout << var[i] << "\t";
    //     std::cout << std::endl;
    // }

    return data;
}

void MF_help::NormalizeMatrix(VecRaw_t &matrix)
{
    //!сюда следует подавать только ТРЕУГОЛЬНЫЕ матрицы
    if (matrix.empty())
    {
        WARNING("Can't normalize empty matrix");
        return;
    }

    size_t min_size = std::min(matrix.size(), matrix[0].size());
    size_t raw_size = matrix[0].size();

    for (size_t i = 0; i < min_size; ++i)
    {
        double coef = matrix[i][i];
        if (std::fabs(coef) < MF_help::approx)
            return;

        matrix[i][i] = 1;

        for (size_t j = i + 1; j < raw_size; ++j)
            matrix[i][j] /= coef;
    }
}