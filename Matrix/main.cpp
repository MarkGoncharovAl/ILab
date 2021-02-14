#include <iostream>
#include "Libraries/Matrix.hpp"

int main()
{
    size_t size = 0;
    std::cin >> size;
    Matrix_t<double> matrix{size, size};

    for (auto iter = matrix.begin(); iter != matrix.end(); ++iter)
    {
        std::cin >> *iter;
        if (!std::cin.good())
        {
            WARNING("Programm didn't read full info");
            return 1;
        }
    }

    std::cout << MF::Determ(matrix);
}