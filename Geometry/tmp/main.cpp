#include <iostream>
#include <array>
#include <array>

template <typename T>
class container
{
public:
template <typename Iter>
    container(Iter beg, Iter end);
};


template <typename Iter>
container(Iter beg, Iter end) 
-> container<typename std::iterator_traits<Iter>::value_type>;

template <typename T>
template <typename Iter>
container<T>::container(Iter beg, Iter end)
{
    std::cout << "AAAA";
}


int main()
{
    std::array arr = {1, 2, 3};
    container con{arr.begin(), arr.end()};
}