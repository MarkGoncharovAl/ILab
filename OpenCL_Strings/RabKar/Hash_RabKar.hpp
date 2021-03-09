#pragma once
#include <iostream>

namespace clM
{
    class Hash_RabKar
    {
    public:
        using hash_type = size_t;
        //1) move + copy ctor
        //2) operator ==()

        //!Should be realized!
        hash_type HashFunction (const std::string& str);
        bool LastCompare(const char* lhs, const char* rhs, size_t size);
    
    private:
        size_t found = 0;
        size_t all = 0;
    };
}