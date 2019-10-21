//
// Created by omerh on 14/08/2019.
//

#include <gtest/gtest.h>
#include "model/padding.h"

using namespace model;

TEST(pkcs7_padding, encode_decode)
{
    const char *message = "This is a 32 bytes long message";

    for (size_t block_size = 1; block_size < 32; ++block_size)
    {
        for (auto data_length = 0; data_length < 32; ++data_length)
        {
            unsigned char buffer[64];
            std::copy(message, message + data_length, buffer);

            auto chars_to_add = block_size - (data_length % block_size);
            auto encoded_length = padding::pkcs7::encode(buffer, sizeof(buffer), size_t(data_length),
                                                         size_t(block_size));

            ASSERT_EQ(encoded_length, data_length + chars_to_add)
                                        << "block_size: " << block_size << " data_length: " << data_length;
            ASSERT_TRUE(std::all_of(buffer + data_length, buffer + encoded_length, [&](auto c) {
                return c == chars_to_add;
            }));

            auto actual_size = padding::pkcs7::decode(buffer, encoded_length, block_size);

            ASSERT_EQ(actual_size, data_length) << "block_size: " << block_size << " data_length: " << data_length;

            ASSERT_THROW({
                             buffer[encoded_length - 1] = block_size + 1; // Fuck the buffer
                             actual_size = padding::pkcs7::decode(buffer, encoded_length, block_size);
                         }, model::internal::padding::pkcs7::invalid_padding);

        }
    }
}
