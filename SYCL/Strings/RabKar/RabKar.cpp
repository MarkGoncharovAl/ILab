#include "RabKar.hpp"

#include "../Common_libs/Time.hpp"

Msycl::RabKar::RabKar (std::string& base) :
    MySycl () ,
    Hash_RabKar () ,
    buffer_base_ (std::move (CreateLinBuf (base.begin(), base.end()))) ,
    base_ (base)
{}


std::vector<size_t> Msycl::RabKar::FindPatterns
(std::vector<std::string>& patterns)
{
    std::vector<size_t> output;
    output.reserve (patterns.size ());

    //preparing hashes tables that will be compared
    auto&& hashes = PrepareHashOfBuffer (patterns);

    MLib::Time time;
    for (std::string& str : patterns)
    {
        //getting main number
        hash_type hash_pattern = HashFunction (str);

        //getting current hash table
        auto&& base_hashes = hashes.find (str.size ());
        if (base_hashes == hashes.end ())
        {
            LOG_warning << "Hash " << str.size () << " wasn't prepared!";
            continue;
        }

        auto& check = base_hashes->second;
        size_t count = 0;

        //comparing hashes
        for (size_t i = 0; i < check.size (); ++i)
        {
            if (check[i] == hash_pattern)
                if (LastCompare (str.data () , base_.data () + i , str.size ()))
                    count++;
        }

        output.emplace_back (count);
    }
    compare_ = time.Get ().count ();

    return output;
}

void Msycl::RabKar::HashEffect () const
{
    std::cout << "Hash found: " << Findings () << "%\n";
}

Msycl::RabKar::PrepareHashOfBuffer_t
Msycl::RabKar::PrepareHashOfBuffer (std::vector<std::string>& patterns)
{
    PrepareHashOfBuffer_t hashes;

    for (const std::string& str : patterns)
    {
        const size_t pattern_size = str.size ();
        size_t global_size = base_.size () - pattern_size + 1;

        //creating hashes for base string
        auto&& current_hash_buffer = hashes.find (pattern_size);
        if (current_hash_buffer == hashes.end ())
        {
            //Preparing hash table
            std::vector<hash_type> hash_buffer (global_size);

            //the same vector, but now pushed
            std::vector<hash_type>& new_hash_buffer =
                hashes.insert (std::make_pair (pattern_size , std::move (hash_buffer))).first->second;

            //filling tables of hashes
            GetVecHashes (pattern_size , global_size , new_hash_buffer);
        }
    }

    return hashes;
}

class GettingHashesInTable;
void Msycl::RabKar::GetVecHashes (size_t pattern_size , size_t global_size , std::vector<hash_type>& hash_buffer)
{
    //creating buffers for output hashes
    auto&& cur_buffer = CreateLinBuf (hash_buffer.begin(), hash_buffer.end());

    MLib::Time time;
    submit ([&](cls::handler& cgh) {

        //accessing for operations
        auto&& cur_buffer_a = cur_buffer.get_access<cls::access_mode::read_write> (cgh);
        auto&& buffer_base_a = buffer_base_.get_access<cls::access_mode::read> (cgh);

        cgh.parallel_for<class GettingHashesInTable> (cls::range<1>{global_size} , [=](cls::id<1> i) {
            const int id = i[0];
            ulong hash = 1;
            ulong last_iter = id + pattern_size;

            for (ulong iter = id; iter < last_iter; ++iter)
                hash = ((hash * 31) + buffer_base_a[iter]) % (1 << 25);

            cur_buffer_a[id] = hash;
        });
    });
    gpu_time_ += time.Get().count();
}