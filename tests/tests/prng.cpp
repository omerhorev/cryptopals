//
// Created by omerh on 08/11/2019.
//

#include <gtest/gtest.h>
#include <random>
#include <model/prng.h>
#include <utils/random.h>

TEST(prng, test_vector)
{
    // Test a million vectors
    for (int i = 0; i < 100; ++i)
    {
        auto seed = utils::random::instance().get<uint32_t>(0, UINT32_MAX);

        std::mt19937 m(seed);
        model::mt19937 prng(seed);

        for (int j = 0; j < 10000; ++j)
        {

            ASSERT_EQ(m(), prng.generate());
        }
    }
}
