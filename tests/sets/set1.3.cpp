//
// Created by omerh on 22/07/2019.
//

#include <utils/hex.h>
#include <breaks/xor-break.h>
#include <model/xor-byte-key.h>
#include <utils/string-score.h>
#include "gtest/gtest.h"

using namespace utils;

TEST(sets, set_1_3)
{
    unsigned char data1[34];
    unsigned char key = 0;

    hex::decode("1b37373331363f78151b7f2b783431333d78397828372d363c78373e783a393b3736", 68, data1, sizeof(data1));


    unsigned char dec[34];
    model::xor_byte_key::decrypt(data1, dec, 34, 0x58);
    utils::string_score::score_by_frequency((const char*)dec, 34);


    breaks::xor_break b;

    b.single_xor_cipher_break(data1, sizeof(data1), key);

    ASSERT_EQ(key, 0x58);
}