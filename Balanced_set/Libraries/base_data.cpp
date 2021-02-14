#include "base_data.hpp"

int operator-(const base_data<int>::iterator &last, const base_data<int>::iterator &first)
{
    return last.GetLeftCount() - first.GetLeftCount() + 1;
}