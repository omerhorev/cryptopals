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

TEST(galois_filed, exponent2)
{
    ASSERT_EQ(galois::exponent2(0), 1);
    ASSERT_EQ(galois::exponent2(1), 2);
    ASSERT_EQ(galois::exponent2(2), galois::mul(2, 2));
    ASSERT_EQ(galois::exponent2(3), galois::mul(2, galois::mul(2, 2)));
    ASSERT_EQ(galois::exponent2(4), galois::mul(2, galois::mul(2, galois::mul(2, 2))));
    ASSERT_EQ(galois::exponent2(5), galois::mul(2, galois::mul(2, galois::mul(2, galois::mul(2, 2)))));
}