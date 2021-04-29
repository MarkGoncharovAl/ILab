#include "sha256/sha256.hpp"
#include <gtest/gtest.h>

int main(int argc, char* argv[])
{
    ::testing::InitGoogleTest (&argc , argv);
    return RUN_ALL_TESTS ();
}

static Msycl::sha256_data sha;

TEST(MA, easy)
{
    Msycl::sha256_t data = {std::bitset<64>(0)};
    Msycl::sha256(data);
}