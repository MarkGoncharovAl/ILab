#include "BSort.hpp"
#include <cmath>
#include <fstream>
#include <sstream>
#include "../Common_libs/Time/Time.hpp"
#include "../Common_libs/Errors.hpp"
#include "../Common_libs/Color.hpp"

namespace Msycl
{
    class TRASH_NAME;

    BSort::BSort ()
        : MySycl ()
    {}
    size_t BSort::PrepareData (std::vector<int>& data, Sort sort) const
    {
        size_t old_size = data.size ();
        size_t new_size = 1;
        while (new_size <= old_size)
            new_size <<= 1;

        int pushing_num = 0;
        if (sort == Sort::Incr)
            pushing_num = std::numeric_limits<int>::max ();
        else
            pushing_num = std::numeric_limits<int>::min ();

        data.reserve (new_size);
        for (size_t i = old_size; i < new_size; ++i)
            data.push_back (pushing_num);

        return old_size;
    }

    void BSort::sort (std::vector<int>& data , Sort sort /* = Sort::Incr*/)
    {
        size_t old_size = PrepareData (data, sort);
        auto&& buffer = Msycl::CreateLinBuf (data);

        size_t iteration_size = data.size () / 2; //like global_size
        unsigned numStages = GetNumStages (data.size ());

        MLib::Time time;

        for (unsigned curStage = 0; curStage < numStages; ++curStage)
        {
            for (unsigned stage_pass = 0; stage_pass < curStage + 1; ++stage_pass)
            {
                //needed buffer, iteration_size, cur_Stage, stage_pass
                submit ([&](cls::handler& cgh) {
                    auto&& buffer_acc = buffer.get_access<cls::access_mode::read_write> (cgh);
                    cgh.parallel_for<class TRASH_NAME> (cls::range<1>{iteration_size} , [=](cls::id<1> i) {
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

        auto&& gpu_time = time.Get().count();
        std::cout << "SYCL time:\t" << gpu_time << "\tmc\n";
        data.resize(old_size);
    }

    unsigned BSort::GetNumStages (size_t size) const
    {
        return std::ceil (std::log2 (size));
    }
} 