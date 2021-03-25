#include <iostream>
#include <vector>

std::pair<std::vector<int>, std::vector<int>> 
func()
{
    std::vector a = {1, 2, 3};
    std::vector b = {1, 2, 3};
    return std::make_pair(a, b);
}

int main()
{
    auto&& get = func();
    for (auto&& elem : get.first)
        std::cout << elem << std::endl;
}