#include "MC_OpenCL.hpp"

namespace po = boost::program_options;

clM::OpenCL::OpenCL (const cl::Device& device , const std::string& kernel_file)
    : device_ (device)
{
    if (device_ == cl::Device {})
    {
        LOG_error << "Can't create data from cl::Device{}\n";
        throw std::invalid_argument ("bad device!");
    }

    context_ = cl::Context { device_ };
    queue_ = cl::CommandQueue { context_, device_, CL_QUEUE_PROFILING_ENABLE };

    program_ = cl::Program (context_ , kernel_file , true);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

void clM::CheckReturnError (cl_int err)
{
    switch (err)
    {
    case CL_SUCCESS:
        break;
    case CL_INVALID_VALUE:
        LOG_fatal << "INVALID VALUE: " << err;
        break;
    case CL_INVALID_DEVICE:
        LOG_fatal << "INVALID DEVICE: " << err;
        break;
    case CL_OUT_OF_HOST_MEMORY:
        LOG_fatal << "OUT_OF_HOST_MEMORY: " << err;
        break;
    case CL_OUT_OF_RESOURCES:
        LOG_fatal << "OUT_OF_RESOURCES: " << err;
        break;
    default:
        LOG_fatal << "WEIRD MISTAKE IN CHECKING ERROR: " << err;
        break;
    }
}

cl::Device clM::ChooseDevice (int argc , char* argv [])
{
    if (argc == 1)
        std::cout << "Default device 0\n";


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

        if (argc == 1)
        {
            std::cout << "\nDevice " << 0 << " was set:\n";
            std::cout << "Platform:\t";
            std::cout << devices[0].first.getInfo<CL_PLATFORM_NAME> () << "\n";
            std::cout << "Device:\t\t";
            std::cout << devices[0].second.getInfo<CL_DEVICE_NAME> () << "\n\n";
            return devices[0].second;
        }

        //weird case
        std::cout << desc << "\n";
        LOG_warning << "Can't translate command line arguments";
        return cl::Device {};

    }
    catch (std::exception& err)
    {
        LOG_error << "Mistake in cheking arguments: " << err.what();
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
