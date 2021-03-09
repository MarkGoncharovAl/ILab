#include "MC_OpenCL.hpp"

namespace po = boost::program_options;

clM::OpenCL::OpenCL (cl::Device&& device, const std::string& kernel_file)
    : device_ (device)
{
    if (device_ == cl::Device{})
    {
        WARNING("Can't create data from cl::Device{}\n");
        throw std::invalid_argument("bad device!");
    }

    context_ = cl::Context{device_};
    queue_ = cl::CommandQueue{context_, device_, CL_QUEUE_PROFILING_ENABLE};

    std::string data_file = clMFunc::ReadFromFile(kernel_file);
    program_ = cl::Program(context_, data_file, true);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void clM::CheckReturnError (cl_int err , const char* file , size_t line)
{
    switch (err)
    {
    case CL_SUCCESS:
        break;
    case CL_INVALID_VALUE:
        MLib::print_error ("INVALID VALUE" , file , line);
        break;
    case CL_INVALID_DEVICE:
        MLib::print_error ("INVALID DEVICE" , file , line);
        break;
    case CL_OUT_OF_HOST_MEMORY:
        MLib::print_error ("OUT_OF_HOST_MEMORY" , file , line);
        break;
    case CL_OUT_OF_RESOURCES:
        MLib::print_error ("OUT_OF_RESOURCES" , file , line);
        break;
    default:
        MLib::print_error ("WEIRD MISTAKE IN CHECKING ERROR" , file , line);
        break;
    }
}

cl::Device clM::ChooseDevice (int argc , char* argv [])
{
    if (argc == 1)
    {
        std::cout << "You should choose device!\n"
            << "To view help write --help\n";
        return cl::Device {};
    }

    //argc > 1
    auto&& devices = clMFunc::GetDevices ();
    try
    {
        po::options_description desc ("Allowed options");
        desc.add_options ()
            ("help,h" , "information about devices")
            ("device,d" , po::value<int> () , "set device");

        po::variables_map vm;
        po::store (po::parse_command_line (argc , argv , desc) , vm);
        po::notify (vm);

        if (vm.count ("help"))
        {
            std::cout << desc;
            std::cout << "\nAvailable devices and platforms!\n";
            for (size_t i = 0; i < devices.size (); ++i)
            {
                std::cout << "Number: " << i << "\n";
                std::cout << "Platform:\t";
                std::cout << devices[i].first.getInfo<CL_PLATFORM_NAME> () << "\n";
                std::cout << "Device:\t\t";
                std::cout << devices[i].second.getInfo<CL_DEVICE_NAME> () << "\n\n";
            }
            return cl::Device {};
        }

        if (vm.count ("device"))
        {
            int num = vm["device"].as<int> ();
            std::cout << "\nDevice " << num << " was set:\n";
            std::cout << "Platform:\t";
            std::cout << devices[num].first.getInfo<CL_PLATFORM_NAME> () << "\n";
            std::cout << "Device:\t\t";
            std::cout << devices[num].second.getInfo<CL_DEVICE_NAME> () << "\n\n";
            return devices[num].second;
        }

        //nothing was changed!
        std::cout << desc << "\n";
        return cl::Device {};

    }
    catch (std::exception& err)
    {
        WARNING ("Mistake in cheking arguments!");
        throw err;
    }
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

clMFunc::Devices_t
clMFunc::GetDevices ()
{
    //Checking every platform
    std::vector<cl::Platform> platforms;
    cl::Platform::get (&platforms);
    if (platforms.size () == 0)
        throw std::runtime_error ("No platforms found. Check OpenCL installation!");

    std::vector<std::pair<cl::Platform , cl::Device>> output;
    for (size_t i = 0; i < platforms.size (); ++i)
    {
        std::vector<cl::Device> devices = {};
        bool pass = false;

        try
        {
            platforms[i].getDevices (CL_DEVICE_TYPE_ALL , &devices);
        }
        catch (cl::Error& err)
        {
            err.what ();
            pass = true;
        }

        if (!pass)
        {
            for (const auto& device : devices)
            {
                if (device.getInfo<CL_DEVICE_COMPILER_AVAILABLE> ())
                    output.emplace_back (platforms[i] , device);
            }
        }
    }

    return output;
}

std::string clMFunc::ReadFromFile(const std::string &file)
{
    std::ifstream file_stream(file);
    if (!file_stream.is_open())
        throw std::runtime_error("Can't find file: \"" + file + "\"");

    return std::string{std::istreambuf_iterator<char>{file_stream}, std::istreambuf_iterator<char>{}};
}