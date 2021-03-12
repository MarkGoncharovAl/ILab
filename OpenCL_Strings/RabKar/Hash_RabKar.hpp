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
        bool LastCompare (const char* lhs , const char* rhs , size_t size);
        size_t Findings () const noexcept;

        virtual ~Hash_RabKar() {};

    private:
        //we should choose max_size as big as we can
        //it would multiplicate on size_ASCII -> multiplications of them
        //must be lesser than sizeof(hash_type)
        static constexpr hash_type max_size_ = (1 << 31);

        static constexpr hash_type size_ASCII = (1 << 8);
        size_t found = 0;
        size_t all = 0;
    };
}