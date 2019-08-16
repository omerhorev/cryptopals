//
// Created by omerh on 16/08/2019.
//

#include <gtest/gtest.h>
#include <model/padding.h>
#include "model/internal/padding/pkcs7.h"

TEST(sets, set_2_1)
{
    const size_t block_size = 20;
    const size_t data_length = 16;

    auto string = "YELLOW SUBMARINE";
    unsigned char data[block_size] = {0};

    strcpy((char*)data, string);

    model::padding::pkcs7::encode(data, sizeof(data), data_length, block_size);

    ASSERT_TRUE(memcmp(data, string, data_length) == 0);
    ASSERT_EQ(data[16], 4);
    ASSERT_EQ(data[17], 4);
    ASSERT_EQ(data[18], 4);
    ASSERT_EQ(data[19], 4);
}