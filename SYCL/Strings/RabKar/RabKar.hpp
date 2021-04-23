#pragma once
#include "../MySycl/MySycl.hpp"
#include "../Common_libs/Logging/Logging.hpp"
#include "Hash_RabKar.hpp"
#include <unordered_map>
#include <queue>

namespace Msycl
{
    //private Hash_RabKar - used only for interface 
    class RabKar final : private MySycl , private Hash_RabKar
    {
    private:

        cls::buffer<char, 1> buffer_base_;
        std::string& base_;

        double gpu_time_ = 0;
        double compare_ = 0;

        using PrepareHashOfBuffer_t = std::unordered_map < size_t , std::vector<hash_type> >;
        PrepareHashOfBuffer_t
            PrepareHashOfBuffer (std::vector<std::string>& patterns);


        void GetVecHashes (size_t pattern_size , size_t global_size , std::vector<hash_type>& hash_buffer);

    public:
        RabKar (std::string& base);

        std::vector<size_t> FindPatterns (std::vector<std::string>& patterns);

        void HashEffect () const;
        double GetKernelTime () const noexcept { return gpu_time_; }
        double GetCompareTime () const noexcept { return compare_; }
    };
}