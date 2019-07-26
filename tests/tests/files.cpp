//
// Created by omerh on 25/07/2019.
//

#include <utils/hex.h>
#include <fstream>
#include <utils/files.h>
#include "gtest/gtest.h"
#include <cstring>

using namespace utils;

TEST(utils, files_read_hex)
{
    const size_t max_buffer_length = 256;
    unsigned char read_buffer[max_buffer_length] = {0};
    unsigned char vector_buffer[max_buffer_length] = {0};
    const char *lines[] = {
            "ff763d0dff",
            "ff763d0dff377654",
            "44"
    };

    std::ifstream f;
    f.open("data/tests/example-hex.txt", std::fstream::in);

    ASSERT_TRUE(f.is_open());

    for (auto vector: lines)
    {
        auto vector_buffer_length = hex::decode(vector, strlen(vector), read_buffer, sizeof(read_buffer));
        auto read_buffer_length = files::read_hex_line(f, vector_buffer, sizeof(vector_buffer));

        ASSERT_EQ(vector_buffer_length, read_buffer_length);
        ASSERT_TRUE(memcmp(read_buffer, vector_buffer, vector_buffer_length) == 0);
    }
}