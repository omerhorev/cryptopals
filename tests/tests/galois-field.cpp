//
// Created by omerh on 25/07/2019.
//

#include <gtest/gtest.h>
#include "utils/hex.h"
#include "model/internal/galois-field.h"

using namespace utils;
using namespace model;
using namespace model::internal;

TEST(galois_field, multiplication)
{
    galois256 test_vectors[][3] =
            {
                    {(galois256) 1,  (galois256) 1,  (galois256) 1},
                    {(galois256) 1,  (galois256) 2,  (galois256) 2},
                    {(galois256) 3,  (galois256) 2,  (galois256) 6},
                    {(galois256) 10, (galois256) 25, (galois256) 250},
                    {(galois256) 10, (galois256) 26, (galois256) 228},
                    {(galois256) 7,  (galois256) 11, (galois256) 49},
            };

    for (auto &nums : test_vectors)
    {
        ASSERT_EQ(nums[0] * nums[1], nums[2]);
    }
}

TEST(galois_field, exponent2)
{
    galois256 g_2 = (galois256) 2;
    ASSERT_EQ(g_2 ^ ((galois256) 0), ((galois256) 1));
    ASSERT_EQ(g_2 ^ ((galois256) 1), ((galois256) 2));
    ASSERT_EQ(g_2 ^ ((galois256) 2), g_2 * g_2);
    ASSERT_EQ(g_2 ^ ((galois256) 3), g_2 * g_2 * g_2);
    ASSERT_EQ(g_2 ^ ((galois256) 4), g_2 * g_2 * g_2 * g_2);
    ASSERT_EQ(g_2 ^ ((galois256) 5), g_2 * g_2 * g_2 * g_2 * g_2);
}