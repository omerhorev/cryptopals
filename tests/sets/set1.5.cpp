// Created by omerh on 26/07/2019.
//

#include <fstream>
#include <utils/debug.h>
#include "gtest/gtest.h"
#include "model/xor-byte-key.h"

using namespace model;

TEST(sets, set_1_5)
{
    const char *plain = "Burning 'em, if you ain't quick and nimble\nI go crazy when I hear a cymbal";

    unsigned char cipher[256] = {0};

    const unsigned char key[] = {'I', 'C', 'E'};

    const auto test_vectors =
            h2b("0b3637272a2b2e63622c2e69692a23693a2a3c6324202d623d63343c2a26226324272765272a282b2f20430a652e2c652a3124333a653e2b2027630c692b20283165286326302e27282f");

    xor_byte_key::encrypt((const unsigned char *) plain, cipher, strlen(plain), key, sizeof(key));
    ASSERT_TRUE(memcmp(cipher, &test_vectors, sizeof(test_vectors)) == 0);
}2