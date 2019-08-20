//
// Created by omerh on 20/08/2019.
//

#include <gtest/gtest.h>
#include <random>
#include <model/block.h>
#include <utils/base64.h>
#include <model/padding.h>

// The fixture for testing class Foo.
class sets : public ::testing::Test
{
protected:
    unsigned char _random_key[16] = {0};
    const char *_hidden_message_base64 = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";

    void SetUp() override
    {
        std::uniform_int_distribution<unsigned char> byte_dist(0, 0xff);
        std::random_device rd;

        std::generate_n(_random_key, sizeof(_random_key), [&]() {
            return byte_dist(rd);
        });
    }

    void TearDown() override
    {
    }

    size_t ecryption_oracle(const unsigned char message[],
                            size_t message_length,
                            unsigned char buffer[],
                            size_t buffer_length)
    {
        model::aes128_ecb model;
        model.initialize(_random_key, sizeof(_random_key));

        if (buffer_length < message_length)
        {
            return 0;
        }

        std::copy_n(message, message_length, buffer);

        auto hidden_message_length = utils::base64::decode(_hidden_message_base64,
                                                           strlen(_hidden_message_base64),
                                                           buffer + message_length,
                                                           buffer_length - message_length);

        if (hidden_message_length == 0)
        {
            return 0;
        }

        auto encoded_length = model::padding::pkcs7::encode(buffer,
                                                            buffer_length,
                                                            message_length + hidden_message_length,
                                                            model::aes128_ecb::block_size);

        if (encoded_length == 0)
        {
            return 0;
        }

        return encoded_length;
    }
};


TEST(sets, set_2_4)
{
    
}