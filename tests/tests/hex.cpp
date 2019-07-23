//
// Created by omerh on 20/07/2019.
//

#include <utils/hex.h>
#include "gtest/gtest.h"

using namespace utils;

TEST(hex, encoding)
{
    const char *test_vector[][2] = {
            {"",              ""},
            {"f",             "66"},
            {"fo",            "666F"},
            {"foo",           "666F6F"},
            {"foob",          "666F6F62"},
            {"fooba",         "666F6F6261"},
            {"foobar",        "666F6F626172"},
            {"ajdjhf7sd8df7", "616A646A686637736438646637"}
    };


    for (auto &iter: test_vector)
    {
        auto test_vector_buffer = iter[0];
        auto test_vector_hex = iter[1];

        char encoded[64] = {};
        size_t encoded_length;

        char decoded[64] = {};
        size_t decoded_length;


        encoded_length = hex::encode((const unsigned char *) test_vector_buffer,
                                     strlen(test_vector_buffer),
                                     encoded,
                                     sizeof(encoded));

        ASSERT_STREQ(encoded, test_vector_hex);
        ASSERT_EQ(encoded_length, strlen(test_vector_hex));

        decoded_length = hex::decode(test_vector_hex,
                                     strlen(test_vector_hex),
                                     (unsigned char *) decoded,
                                     sizeof(decoded));

        ASSERT_STREQ(decoded, test_vector_buffer);
        ASSERT_EQ(decoded_length, strlen(test_vector_buffer));
    }
}