#include "RabKar.hpp"

clM::RabKar::RabKar (const cl::Device& device) :
    OpenCL (device , "../RabKar/RabKar.cl")
{}


std::vector<size_t> clM::RabKar::FindPatterns
(std::string& base , std::vector<std::string>& patterns)
{
    std::vector<size_t> output;
    output.reserve (patterns.size ());

    buffer_base = CreateBuffer (base);

    //preparing hashes tables that will be compared
    PrepareHashOfBuffer (base , patterns);

    for (std::string& str : patterns)
    {
        //getting main number
        hash_type hash_pattern = HashFunction (str);

        //getting current hash table
        auto&& base_hashes = hashes.find (str.size ());
        if (base_hashes == hashes.end ())
        {
            WARNING ("Hash " + std::to_string (str.size ()) + " wasn't prepared!");
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

    return output;
}

void clM::RabKar::RunEvent (const cl::Kernel& kernel ,
                            const cl::NDRange& loc_sz ,
                            const cl::NDRange& glob_sz)
{
    cl::Event event;
    queue_.enqueueNDRangeKernel (kernel , 0 , glob_sz , loc_sz , nullptr , &event);
    event.wait ();
}

void clM::RabKar::HashEffect () const
{
    std::cout << "Hash found: " << Findings () << "%\n";
}

void clM::RabKar::PrepareHashOfBuffer (std::string& base , std::vector<std::string>& patterns)
{
    std::vector<std::thread> threads;

    for (const std::string& str : patterns)
    {
        const size_t pattern_size = str.size ();
        size_t global_size = base.size () - pattern_size + 1;

        //creating hashes for base string
        auto&& current_hash_buffer = hashes.find (pattern_size);
        if (current_hash_buffer == hashes.end ())
        {
            /*
            We have to push hashes into the table:

            1) reserve place in order to get
            information, what threads are used
            */
            hashes.insert (std::make_pair (pattern_size , std::vector<hash_type>{}));

            // start thread:
            std::thread cur_thread ( GetVecHashes, this, pattern_size, global_size);
            threads.push_back(std::move(cur_thread));
        }
    }

    for (auto&& thread : threads)
        thread.join();
}

void clM::RabKar::GetVecHashes (RabKar* pointer, size_t pattern_size , size_t global_size)
{
    std::vector<hash_type> hash_buffer (global_size);

    //creating buffers for output hashes
    cl::Buffer cur_buffer (pointer->context_ , CL_MEM_READ_WRITE , hash_buffer.size () * sizeof (hash_type));
    pointer->queue_.enqueueWriteBuffer (cur_buffer , CL_TRUE , 0 , hash_buffer.size () * sizeof (hash_type) , hash_buffer.data ());

    cl::Kernel kernel(pointer->program_, "PrepareHashOfBuffer");

    //writting hashes into array
    kernel.setArg (0 , pointer->buffer_base);
    kernel.setArg (1 , cur_buffer);
    kernel.setArg (2 , pattern_size);
    pointer->RunEvent (kernel , cl::NDRange{1} , global_size);

    //cheking hashes
    cl::copy (pointer->queue_ , cur_buffer , hash_buffer.begin () , hash_buffer.end ());

    //DANGER ZONE
    pointer->mutex_.lock();
    if (pointer->hashes.insert_or_assign (pattern_size , std::move (hash_buffer)).second == true)
        std::cout << "AAAAAA";
    pointer->mutex_.unlock();
}