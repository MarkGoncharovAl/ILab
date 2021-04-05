#include "MC_OpenCL.hpp"

namespace po = boost::program_options;

clM::OpenCL::OpenCL (const cl::Device& device , const std::string& kernel_file)
{
    ConstructObject (device , kernel_file);
}

clM::OpenCL::OpenCL (int argc , char* argv [] , std::string const& kernel_file)
{
    auto&& device = ChooseDevice (argc , argv);
    ConstructObject (device , kernel_file);
}

clM::OpenCL::OpenCL (std::string const& kernel_file)
{
    auto&& device = ChooseFirstDevice ();
    ConstructObject(device, kernel_file);
}

void clM::OpenCL::ConstructObject (const cl::Device& device , const std::string& kernel_file)
{
    if (device == cl::Device {})
    {
        LOG_error << "Can't create data from cl::Device{}\n";
        throw std::invalid_argument ("bad device!");
    }
    device_ = device;

    context_ = cl::Context { device_ };
    queue_ = cl::CommandQueue { context_, device_, CL_QUEUE_PROFILING_ENABLE };

    program_ = cl::Program (context_ , clMFunc::ReadKernelFile (kernel_file) , true);
}

////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////

cl::Device clM::ChooseDevice (int argc , char* argv [])
{
    if (argc == 1)
    {
        LOG_trace << "Default device 0\n";
        std::cout << "Default device 0\n";
        return ChooseFirstDevice ();
    }

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
                clMFunc::PrintInfoAboutDevice (i , devices[i].first , devices[i].second);

            return cl::Device {};
        }

        if (vm.count ("device"))
        {
            int num = vm["device"].as<int> ();
            clMFunc::PrintInfoAboutDevice (num , devices[num].first , devices[num].second);
            return devices[num].second;
        }

        //weird case
        std::cout << desc << "\n";
        LOG_warning << "Can't translate command line arguments";
        return cl::Device {};

    }
    catch (std::exception& err)
    {
        LOG_error << "Mistake in cheking arguments: " << err.what ();
        throw err;
    }
}

cl::Device clM::ChooseFirstDevice ()
{
    auto&& devices = clMFunc::GetDevices ();
    clMFunc::PrintInfoAboutDevice (0 , devices[0].first , devices[0].second);
    return devices[0].second;
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

void clMFunc::PrintInfoAboutDevice (size_t num , cl::Platform const& platform , cl::Device const& device)
{
    std::cout << "Number: " << num << "\n";
    std::cout << "Platform:\t";
    std::cout << platform.getInfo<CL_PLATFORM_NAME> () << "\n";
    std::cout << "Device:\t\t";
    std::cout << device.getInfo<CL_DEVICE_NAME> () << "\n\n";
}

std::string clMFunc::ReadKernelFile (std::string const& kernel_file)
{
    std::ifstream file_stream (kernel_file);
    if (!file_stream.is_open ())
    {
        LOG_fatal << "Can't find file: " << kernel_file;
        throw std::runtime_error ("Can't find file: " + kernel_file);
    }
    return std::string { std::istreambuf_iterator<char>{file_stream}, std::istreambuf_iterator<char>{} };
}
