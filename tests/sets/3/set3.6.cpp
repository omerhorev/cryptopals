//
// Created by omerh on 11/11/2019.
//

#include <model/prng.h>
#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <utils/random.h>
#include <breaks/prng.h>

using namespace std::chrono;


TEST(sets_3_6, run)
{
    auto time = system_clock::now();

    // Simulate passage of time...
    time += milliseconds(utils::random::instance().get<uint32_t>(40, 1000));

    auto seed = uint32_t(duration_cast<milliseconds>(time.time_since_epoch()).count());
    auto number = model::mt19937(seed).generate();

    // Simulate passage of time...
    time += milliseconds(utils::random::instance().get<uint32_t>(40, 1000));

    // The actual attack:
    auto timestamp = uint32_t(duration_cast<milliseconds>(time.time_since_epoch()).count());

    // Lets just try some seeds until we get the right one
    bool found = false;

    for (int i = 0; i < 10000; ++i)
    {
        if (model::mt19937(timestamp--).generate() == number)
        {
            SUCCEED();
            found = true;
        }
    }

    if (!found) FAIL();
}