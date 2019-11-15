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
    auto seed = utils::random::instance().get<uint32_t>();
    auto number = model::mt19937(seed).generate();

    auto broken_number = breaks::mt19973::get_seed_from_first_element(number);

    ASSERT_EQ(broken_number, number);
}