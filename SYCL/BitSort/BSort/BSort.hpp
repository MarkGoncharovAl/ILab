#pragma once
#include <iostream>
#include <iterator>
#include <vector>
#include <algorithm>
#include "../Common_libs/Time/Time.hpp"

#include "../MySycl/MySycl.hpp"

namespace Msycl
{
    enum class sort_type
    {
        Decr = 0 ,
        Incr
    };

    //sorting iterator must be forward type. Now <concept> isn't on my computer
    //therefore i had to create bycicle (concept std::forward_iterator isn't allowed now...(((
    template<typename Iter>
    concept IterSort =
        std::is_same_v<typename std::iterator_traits<Iter>::iterator_category , std::forward_iterator_tag>
        || std::is_same_v<typename std::iterator_traits<Iter>::iterator_category , std::bidirectional_iterator_tag>
        || std::is_same_v<typename std::iterator_traits<Iter>::iterator_category , std::random_access_iterator_tag>;


    /* USED NAMES
    IterBuf - random_access_iterator
    IterSort - forward_iterator
    IterVal - iterator's value (through std::iterator_traits)
    */

    class BSort : private MySycl
    {
    public:
        BSort () = default;

        /*
        Main sort function. Operating with convenient vector.
        For using every forward_iterator -> use Msycl::sort(Iter, Iter, sort_type);
        */
        template <typename T>
        void sort (std::vector<T>& data , sort_type sort);

    private:
        unsigned GetNumStages (size_t size) const
        {
            return std::ceil (std::log2 (size));
        }

        //returns old size of data
        template <typename T>
        size_t PrepareData (std::vector<T>& data , sort_type sort) const;
    };

    //User's function
    ///////////////////////////////////////////
    //slower but common function
    template <IterSort Iter>
    void sort (Iter begin , Iter end , sort_type type_sort = sort_type::Incr)
    {
        //storing data for quick sorting with convenient data
        std::vector sorting_vec(begin, end);
        BSort {}.sort (sorting_vec , type_sort);
        
        std::copy(sorting_vec.cbegin(), sorting_vec.cend(), begin);
    }

    //fast function without copying
    template <typename T>
    void sort (std::vector<T>& data , sort_type type_sort = sort_type::Incr)
    {
        BSort {}.sort (data , type_sort);
    }
    ///////////////////////////////////////////


    class BitSort; //for named lambda
    template <typename T>
    void BSort::sort (std::vector<T>& data , sort_type sort/* = Incr*/)
    {
        size_t old_size = PrepareData (data , sort);
        auto&& buffer = Msycl::CreateLinBuf (data.begin () , data.end());

        size_t iteration_size = data.size() / 2; //like global_size
        unsigned numStages = GetNumStages (data.size());

        MLib::Time time;

        for (unsigned curStage = 0; curStage < numStages; ++curStage)
        {
            for (unsigned stage_pass = 0; stage_pass < curStage + 1; ++stage_pass)
            {
                //needed buffer, iteration_size, cur_Stage, stage_pass
                submit ([&](cls::handler& cgh) {
                    auto&& buffer_acc = buffer.template get_access<cls::access_mode::read_write> (cgh);
                    cgh.parallel_for<class BitSort> (cls::range<1>{iteration_size} , [=](cls::id<1> i) {
                        uint compare_distance = 1 << (curStage - stage_pass);
                        const int id = i[0];

                        uint left_id = (id % compare_distance) + (id / compare_distance) * 2 * compare_distance;
                        uint right_id = left_id + compare_distance;
                        char direction = static_cast<char>(sort);

                        int left_elem = buffer_acc[left_id];
                        int right_elem = buffer_acc[right_id];

                        if ((id / (1 << curStage)) % 2 == 1)
                            direction = 1 - direction;

                        int greater = right_elem;
                        int lesser = left_elem;
                        if (left_elem > right_elem)
                        {
                            greater = left_elem;
                            lesser = right_elem;
                        }

                        if (direction)
                        {
                            buffer_acc[left_id] = lesser;
                            buffer_acc[right_id] = greater;
                        }
                        else
                        {
                            buffer_acc[left_id] = greater;
                            buffer_acc[right_id] = lesser;

                        }
                    });
                });
            }
        }

        auto&& gpu_time = time.Get ().count ();
        std::cout << "SYCL time:\t" << gpu_time << "\tmc\n";
        data.resize (old_size);
    }

    template <typename T>
    size_t BSort::PrepareData (std::vector<T>& data, sort_type sort) const
    {
        size_t old_size = data.size ();
        size_t new_size = 1;
        while (new_size <= old_size)
            new_size <<= 1;

        T pushing_num = T{};
        if (sort == sort_type::Incr)
            pushing_num = std::numeric_limits<T>::max ();
        else
            pushing_num = std::numeric_limits<T>::min ();

        data.resize (new_size, pushing_num);
        return old_size;
    }
}