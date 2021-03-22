#include "RabKar.hpp"

clM::RabKar::RabKar (const cl::Device& device)
    : OpenCL (device , "../RabKar/RabKar.cl")
{}


std::vector<size_t> clM::RabKar::FindPatterns
(std::string& base , std::vector<std::string>& patterns)
{
    std::vector<size_t> output;
    output.reserve (patterns.size ());

    cl::Buffer buffer_base = CreateBuffer (base);
    auto&& hashes = PrepareHashOfBuffer (base , patterns , buffer_base);

    for (std::string& str : patterns)
    {
        //getting main number
        hash_type hash_pattern = HashFunction (str);
        auto&& base_hashes = hashes.find (str.size ());
        if (base_hashes == hashes.end ())
        {
            WARNING ("Hash " + std::to_string (str.size ()) + " wasn't prepared!");
            continue;
        }

        auto& check = base_hashes->second;
        size_t count = 0;
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

std::unordered_map
< size_t , std::vector<clM::RabKar::hash_type> >
clM::RabKar::PrepareHashOfBuffer (std::string& base , std::vector<std::string>& patterns , cl::Buffer& buffer_base)
{
    std::unordered_map
        < size_t , std::vector<hash_type> > output;

    cl::Kernel kernel (program_ , "PrepareHashOfBuffer");
    cl::NDRange local_size = 1;

    for (const std::string& str : patterns)
    {
        const size_t pattern_size = str.size ();
        cl::NDRange global_size = base.size () - pattern_size + 1;

        //creating hashes for base string
        auto&& current_hash_buffer = output.find (pattern_size);
        if (current_hash_buffer == output.end ())
        {
            std::vector<hash_type> hash_buffer (*global_size);
            cl::Buffer cur_buffer (context_ , CL_MEM_READ_WRITE , hash_buffer.size () * sizeof (hash_type));
            queue_.enqueueWriteBuffer (cur_buffer , CL_TRUE , 0 , hash_buffer.size () * sizeof (hash_type) , hash_buffer.data ());

            //writting hashes into array
            kernel.setArg (0 , buffer_base);
            kernel.setArg (1 , cur_buffer);
            kernel.setArg (2 , pattern_size);
            RunEvent (kernel , local_size , global_size);

            cl::copy (queue_ , cur_buffer , hash_buffer.begin () , hash_buffer.end ());

            output.insert (std::make_pair (pattern_size , std::move (hash_buffer)));
        }
    }
    return output;
}