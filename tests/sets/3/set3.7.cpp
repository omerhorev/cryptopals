//
// Created by omerh on 15/11/2019.
//

#include <gtest/gtest.h>
#include <model/prng.h>
#include <utils/random.h>
#include <breaks/prng.h>

TEST(sets_3_7, run)
{
    auto seed = utils::random::instance().get<uint32_t>(40, 1000);
    auto prng = model::mt19937(seed);

    std::array<uint32_t, breaks::mt19937::values_in_mt19937_state> recorded_values = {0};

    std::generate(recorded_values.begin(), recorded_values.end(), [&]() { return prng.generate(); });

    auto cloned_prng = breaks::mt19937::clone_mt19937(recorded_values);

    for (auto v : recorded_values)
    {
        ASSERT_EQ(v, cloned_prng.generate());
    }
}