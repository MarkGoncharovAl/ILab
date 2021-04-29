#include <iostream>
#include <CL/sycl.hpp>

namespace Msycl
{
    namespace cls = cl::sycl;

    //concept is used for checking for data continuity - for buffer
    template <typename Iter>
    concept IterBuf =
        std::is_same_v <typename std::iterator_traits<Iter>::iterator_category ,
        std::random_access_iterator_tag>;

    class MySycl
    {
    public:
        MySycl ()
            : queue_ (cls::default_selector {})
        {}
        virtual ~MySycl () {}

        cls::string_class GetDevicName () const
        {
            return queue_.get_device ().get_info<cls::info::device::name> ();
        }

        template <class T>
        auto submit (T&& elem) { queue_.submit (std::forward<T> (elem)); }

    private:
        cls::queue queue_;
    };

    template <typename Iter>
    using IterVal = typename std::iterator_traits<Iter>::value_type;
    
    template <IterBuf Iter>
    cls::buffer<IterVal<Iter> , 1>
        CreateLinBuf (Iter begin , Iter end)
    {
        cls::range<1> range (std::distance (begin , end));
        return cls::buffer<IterVal<Iter> , 1> (&(*begin) , std::move (range));
    }
}