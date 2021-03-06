#include "Hash_RabKar.hpp"

bool clM::Hash_RabKar::LastCompare (const char* lhs ,
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

clM::Hash_RabKar::hash_type
clM::Hash_RabKar::HashFunction(const std::string& str)
{   
    hash_type hash = 1; //start_value

    for (char symbol : str)
        hash = (size_ASCII * hash + symbol) % max_size_;
    return hash;
}

size_t clM::Hash_RabKar::Findings () const noexcept
{
    if (all == 0)
        return 100;
    return 100 * found / all;
}