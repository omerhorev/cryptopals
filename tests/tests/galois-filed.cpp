//
// Created by omerh on 25/07/2019.
//

#include <gtest/gtest.h>
#include "utils/hex.h"
#include "model/internal/galois-field.h"

using namespace utils;
using namespace model;
using namespace model::internal;

TEST(galois_filed, multiplication)
{
    ASSERT_EQ(galois::mul(1, 1), 1);
    ASSERT_EQ(galois::mul(1, 2), 2);
    ASSERT_EQ(galois::mul(3, 2), 6);
    ASSERT_EQ(galois::mul(10, 25), 250);
    ASSERT_EQ(galois::mul(10, 26), 228);
    ASSERT_EQ(galois::mul(7, 11), 49);
}