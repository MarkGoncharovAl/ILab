#pragma once
#include "MySycl.hpp"
#include "../Libs/Logging/Logging.hpp"
#include "Prime.hpp"
#include <bitset>
#include <array>
#include <algorithm>

namespace Msycl
{

    class sha256_data : private MySycl
    {
    public:
        static constexpr size_t inner_sz = 64;
        static constexpr size_t outer_sz = 8;
        using block_t = std::bitset<inner_sz>;

        sha256_data () = default;
        void start (std::array<block_t , outer_sz>& ar, size_t size);

    private:
        template <size_t start, size_t end>
        std::array<unsigned, end - start> GetPrimeNums() const;
    };

    using sha256_t = std::array<sha256_data::block_t , sha256_data::outer_sz>;

    //preparing string
    void sha256 (sha256_t& bits);
}

/*
Main idea was used from
https://habr.com/ru/post/258181/#:~:text=%D0%A1%D1%82%D1%80%D1%83%D0%BA%D1%82%D1%83%D1%80%D0%B0%20%D0%B1%D0%B8%D1%82%D0%BA%D0%BE%D0%B9%D0%BD%2D%D0%B1%D0%BB%D0%BE%D0%BA%D0%B0-,SHA%2D256,%D1%81%D0%BB%D0%BE%D0%B2%20%E2%80%94%20%D0%BE%D1%82%20A%20%D0%B4%D0%BE%20H.
*/