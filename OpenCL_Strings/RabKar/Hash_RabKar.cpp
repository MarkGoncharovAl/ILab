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