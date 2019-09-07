//
// Created by omerh on 16/08/2019.
//

#include <gtest/gtest.h>
#include <model/block.h>
#include <utils/files.h>

TEST(set_2_2, run)
{
    auto key = (unsigned char *) "YELLOW SUBMARINE";
    const unsigned char iv[16] = {0};

    unsigned char plain[5000] = {0};

    auto plain_length = utils::files::read_base64("data/sets/2/2.txt", plain, sizeof(plain));

    model::aes128_cbc model;

    model.initialize(key, 16, iv, 16);

    // Decrypts with two steps to make sure the cbc works
    model.decrypt(plain, 16);
    model.decrypt(plain + 16, plain_length - 16);

    ASSERT_TRUE(memcmp((const char *) plain + 0, "I'm back", strlen("I'm back")) == 0);
    ASSERT_TRUE(memcmp((const char *) plain + 16, " ringin' the", strlen("ringin' the")) == 0);
}