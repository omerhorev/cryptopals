//
// Created by omerh on 01/12/2019.
//

#include <gtest/gtest.h>
#include <math/number.h>

using num_t = math::num512_t;

TEST(numbers, add)
{
    num_t a = 1;
    num_t b = a++;

    b += 1;
    b += a;

    ASSERT_EQ(++a, b - a);

}