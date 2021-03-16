#pragma once
#include <iostream>

static constexpr size_t GetMaxSize() 
{
    size_t max_size = 0xff;
    for (char i = 0; i < 49; ++i)
        max_size = (max_size << 8) + 0xff;
    return max_size;
}

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
        double Findings () const noexcept;

        virtual ~Hash_RabKar() {};

    private:
        static constexpr size_t max_size_ = GetMaxSize();

        double found = 0;
        double all = 0;
    };
}