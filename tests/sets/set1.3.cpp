//
// Created by omerh on 22/07/2019.
//

#include <utils/hex.h>
#include <breaks/xor-break.h>
#include "gtest/gtest.h"

using namespace utils;

TEST(sets, set_1_3)
{
    unsigned char data1[34];
    char key = 0;

    hex::decode("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736", 68, data1, sizeof(data1));

    breaks::xor_break b;

    b.single_xor_cipher_break(data1, sizeof(data1), key);

    ASSERT_EQ(key, 0x58);
}