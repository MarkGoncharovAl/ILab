#pragma once
#include <iostream>

namespace clM
{
    class Hash_RabKar
    {
    public:
        using hash_type = unsigned int;
        //1) move + copy ctor
        //2) operator ==()

        //!Should be realized!
        hash_type HashFunction (const std::string& str);
        bool LastCompare (const char* lhs , const char* rhs , size_t size);
        double Findings () const noexcept;

        virtual ~Hash_RabKar () {};

    private:
        hash_type increase_ = 31;
        hash_type module_ = (1 << 25);

        double found = 0;
        double all = 0;
    };
}