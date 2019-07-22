//
// Created by omerh on 22/07/2019.
//

#include <utils/hex.h>
#include <utils/general.h>
#include "gtest/gtest.h"
#include "gtest/gtest-matchers.h"

using namespace utils;

TEST(sets, set_1_2)
{
    unsigned char data1[18];
    unsigned char data2[18];
    unsigned char expected_xor[18];
    unsigned char calculated_xor[18];

    hex::decode("1c0111001f010100061a024b53535009181c", 36, data1, sizeof(data1));
    hex::decode("686974207468652062756c6c277320657965", 36, data2, sizeof(data2));
    hex::decode("746865206b696420646f6e277420706c6179", 36, data2, sizeof(data2));

    auto len = general::xorBuffers(data1, data2, calculated_xor);
    ASSERT_EQ(len, 18);
    ASSERT_TRUE(memcmp(calculated_xor, expected_xor, 18));

}

