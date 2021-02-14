#include "Random.hpp"

namespace MLib
{
    std::random_device Random::rd_;
    std::mt19937 Random::gen_{Random::rd_()};

    Random::Random(int init_start, int init_end)
        : dist_(init_start, init_end)
    {
    }

    int Random::get() { return dist_(gen_); }

} // namespace MLib