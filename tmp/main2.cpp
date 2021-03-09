#include <iostream>

int main()
{
    int i = 0;
    std::cout << i + 1; //? -> 1

    i++;
    std::cout << i; //? -> 1

    ++i;
    std::cout << i; //? -> 1 + 1

    i = i + 1;
    std::cout << i; //? -> 3
}