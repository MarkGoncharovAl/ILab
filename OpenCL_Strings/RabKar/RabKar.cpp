#include "RabKar.hpp"

#include "kernel.hpp"
#include "../Common_libs/Time.hpp"

clM::RabKar::RabKar (const cl::Device& device) :
    OpenCL (device , kernel_file) ,
    event_data_ () ,
    buffer_base_ () ,
    gpu_time_ (0)
{}


std::vector<size_t> clM::RabKar::FindPatterns
(std::string& base , std::vector<std::string>& patterns)
{
    std::vector<size_t> output;
    output.reserve (patterns.size ());

    buffer_base_ = CreateBuffer (base);

    //preparing hashes tables that will be compared
    auto&& hashes = PrepareHashOfBuffer (base , patterns);

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
                if (LastCompare (str.data () , base.data () + i , str.size ()))
                    count++;
        }

        output.emplace_back (count);
    }
    compare_ = time.Get().count ();

    return output;
}

void clM::RabKar::HashEffect () const
{
    std::cout << "Hash found: " << Findings () << "%\n";
}

clM::RabKar::PrepareHashOfBuffer_t
clM::RabKar::PrepareHashOfBuffer (std::string& base , std::vector<std::string>& patterns)
{
    PrepareHashOfBuffer_t hashes;

    for (const std::string& str : patterns)
    {
        const size_t pattern_size = str.size ();
        size_t global_size = base.size () - pattern_size + 1;

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

    /*
    In this step we have filled queue (called event_data_)
    Now we must copy data from GPU
    */
    CopyDataFromGPU ();
    return hashes;
}

void clM::RabKar::CopyDataFromGPU ()
{
    while (!event_data_.empty ())
    {
        //copying every event
        gpu_time_ += event_data_.front ().CopyDataFromGPU (queue_);
        event_data_.pop ();
    }
}

void clM::RabKar::GetVecHashes (size_t pattern_size , size_t global_size , std::vector<hash_type>& hash_buffer)
{
    //creating buffers for output hashes
    cl::Buffer cur_buffer = CreateBuffer (hash_buffer);
    cl::Kernel kernel (program_ , "PrepareHashOfBuffer");

    //writting hashes into array
    kernel.setArg (0 , buffer_base_);
    kernel.setArg (1 , cur_buffer);
    kernel.setArg (2 , pattern_size);
    EventWaiter waiter { hash_buffer, std::move (cur_buffer) };
    queue_.enqueueNDRangeKernel (kernel , 0 , global_size , cl::NDRange { 1 } , nullptr , waiter.GetEvent ());

    event_data_.push (std::move (waiter));
}

clM::RabKar::EventWaiter::EventWaiter (std::vector<hash_type>& hash , cl::Buffer buffer) :
    hash_ (hash) ,
    buffer_ (buffer) ,
    event_ ()
{}

double clM::RabKar::EventWaiter::CopyDataFromGPU (cl::CommandQueue& queue)
{
    event_.wait ();
    cl::copy (queue , buffer_ , hash_.begin () , hash_.end ());

    return (static_cast<double>
    (event_.getProfilingInfo<CL_PROFILING_COMMAND_END> () - event_.getProfilingInfo<CL_PROFILING_COMMAND_START> ()) / 1000.0);
}