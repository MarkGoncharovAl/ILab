#include <iostream>
#include <CL/sycl.hpp>

namespace Msycl
{
    namespace cls = cl::sycl;

    class MySycl
    {
    public:
        MySycl();
        virtual ~MySycl() {}

        cls::string_class GetDevicName () const;

        template <class T>
        auto submit(T&& elem) {queue_.submit(std::forward<T>(elem));}

    private:
        cls::queue queue_;
    };

    /*must has:
    1) typename value_type
    2) data()
    3) size()
    */
    template <class Container>
    auto CreateLinBuf(Container& cont) -> cls::buffer<typename Container::value_type, 1>
    {
        return cls::buffer<typename Container::value_type, 1>(cont.data(), cls::range<1>(cont.size()));
    }
}