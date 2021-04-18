#include "MySycl.hpp"

namespace Msycl
{
    MySycl::MySycl ()
        : queue_ (cls::default_selector {})
    {}

    cls::string_class MySycl::GetDevicName () const
    {
        return queue_.get_device().get_info<cls::info::device::name>();
    }
}