#include "Hash_RabKar.hpp"

bool Msycl::Hash_RabKar::LastCompare (const char* lhs ,
                                    const char* rhs ,
                                    size_t size)
{
    all++;
    const char* last_lhs = lhs + size;

    for (; lhs < last_lhs; ++lhs , ++rhs)
        if (*lhs != *rhs)
            return false;

    found++;
    return true;
}

Msycl::Hash_RabKar::hash_type
Msycl::Hash_RabKar::HashFunction(const std::string& str)
{   
    hash_type hash = 1; //start_value

    for (char symbol : str)
        hash = ((hash * increase_) + symbol) % module_;
    return hash;
}

double Msycl::Hash_RabKar::Findings () const noexcept
{
    if (all == 0)
        return 100;
    return (100 * found) / all;
}