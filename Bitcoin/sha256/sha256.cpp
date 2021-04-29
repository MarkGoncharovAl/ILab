#include "sha256.hpp"

namespace Msycl
{
    void sha256_data::start (sha256_t& ar , size_t size)
    {
        block_t block (size);
        ar.back () = block; //last block is size
        for (auto&& elem : ar)
            std::cout << elem << "\n";
        auto&& hashes = GetPrimeNums<8, 16>();
    }

    template <size_t start , size_t end>
    std::array<unsigned , end - start> sha256_data::GetPrimeNums () const
    {
        auto&& prime_nums = MLib::PrimeArray_v<end>;
        std::array<unsigned , end - start> hashes;
        std::copy (prime_nums.cbegin () + start , prime_nums.cend () , hashes.begin ());
        return hashes;
    }

    void sha256 (sha256_t& bits)
    {
        //i have to get size of non-zero information
        auto last = std::find_if (bits.rbegin () , bits.rend () ,
            [](auto&& elem) { return elem == sha256_data::block_t (0); });

        size_t size = std::distance (bits.rbegin () , last) * 8;
        size += last->_Find_first ();

        //reversing to usual order
        size = sha256_data::inner_sz * sha256_data::outer_sz - size;
        LOG_debug << "Size: " << size;

        //preparing single 1
        if (last->_Find_first () == 0)
        { //0101 0000 -> 0101 1000
            --last;
            last->set (sha256_data::inner_sz - 1 , true);
        }
        else
        { //0100 -> 0110
            last->set (last->_Find_first () - 1 , true);
        }
        LOG_debug << "Iter: " << *last;

        sha256_data {}.start (bits , size);
    }
}