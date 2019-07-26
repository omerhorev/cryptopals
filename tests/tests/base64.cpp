//
// Created by omerh on 13/07/2019.
//

#include <iterator>
#include <cstring>
#include "gtest/gtest.h"
#include "utils/base64.h"

using namespace utils;

TEST(utils, base64_test_vectors)
{
    const char *test_vector[][2] = {
            {"",              ""},
            {"f",             "Zg=="},
            {"fo",            "Zm8="},
            {"foo",           "Zm9v"},
            {"foob",          "Zm9vYg=="},
            {"fooba",         "Zm9vYmE="},
            {"foobar",        "Zm9vYmFy"},
            {"ajdjhf7sd8df7", "YWpkamhmN3NkOGRmNw=="}
    };


    for (auto &iter: test_vector)
    {
        auto test_vector_buffer = iter[0];
        auto test_vector_base64 = iter[1];

        char encoded[64] = {};
        size_t encoded_length;

        char decoded[64] = {};
        size_t decoded_length;


        encoded_length = base64::encode((const unsigned char *) test_vector_buffer,
                                        strlen(test_vector_buffer),
                                        encoded,
                                        sizeof(encoded));

        ASSERT_STREQ(encoded, test_vector_base64);
        ASSERT_EQ(encoded_length, strlen(test_vector_base64));

        decoded_length = base64::decode(test_vector_base64,
                                        strlen(test_vector_base64),
                                        (unsigned char *) decoded,
                                        sizeof(decoded));

        ASSERT_STREQ(decoded, test_vector_buffer);
        ASSERT_EQ(decoded_length, strlen(test_vector_buffer));
    }
}

TEST(encoding, self_compare)
{
    const unsigned char *strings[] = {
            (unsigned char *) "5",
            (unsigned char *) "+2",
            (unsigned char *) "+2\251\44",
            (unsigned char *) "\0\1\4\6",
            (unsigned char *) "yax\255a",
    };
    const auto strings_count = sizeof(strings) / sizeof(char *);

    char base64[64];
    unsigned char decoded[64];

    for (auto i = 0; i < strings_count; ++i)
    {
        auto encoded_length = base64::encode(strings[i], (size_t) i + 1, base64, sizeof(base64));
        auto decoded_length = base64::decode(base64, encoded_length, decoded, sizeof(decoded));

        ASSERT_EQ(decoded_length, i+1);
        ASSERT_TRUE(std::memcmp(strings[i], decoded, decoded_length) == 0);
    }
}