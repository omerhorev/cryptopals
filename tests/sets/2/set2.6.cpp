//
// Created by omerh on 26/08/2019.
//

#include <gtest/gtest.h>
#include <random>
#include <model/block.h>
#include <utils/base64.h>
#include <model/padding.h>
#include <breaks/byte-at-a-time-ecb-decryption.h>
#include <utils/random.h>

// The fixture for testing class Foo.
class set_2_6 : public ::testing::Test
{
private:
    unsigned char _random_key[16] = {0};
    const char *_hidden_message_base64 = "Um9sbGluJyBpbiBteSA1LjAKV2l0aCBteSByYWctdG9wIGRvd24gc28gbXkgaGFpciBjYW4gYmxvdwpUaGUgZ2lybGllcyBvbiBzdGFuZGJ5IHdhdmluZyBqdXN0IHRvIHNheSBoaQpEaWQgeW91IHN0b3A/IE5vLCBJIGp1c3QgZHJvdmUgYnkK";
    unsigned char _hidden_message[138 + 1];;

public:

    void SetUp() override
    {
        utils::random::instance().fill(_random_key);

        auto hidden_message_length = utils::base64::decode(_hidden_message_base64,
                                                           strlen(_hidden_message_base64),
                                                           _hidden_message,
                                                           sizeof(_hidden_message));

        ASSERT_EQ(hidden_message_length, sizeof(_hidden_message) - 1);

        _hidden_message[hidden_message_length] = 0;
    }

    void TearDown() override
    {
    }

    size_t encryption_oracle(const unsigned char *message,
                             size_t message_length,
                             unsigned char *buffer,
                             size_t buffer_length)
    {
        auto random_noise_length = utils::random::instance().get<size_t>(0, 32);

        model::aes128_ecb model;
        model.initialize(_random_key, sizeof(_random_key));

        if (buffer_length < message_length + random_noise_length)
        {
            return 0;
        }

        utils::random::instance().fill(buffer, random_noise_length);
        std::copy_n(message, message_length, buffer + random_noise_length);

        auto hidden_message_length = utils::base64::decode(_hidden_message_base64,
                                                           strlen(_hidden_message_base64),
                                                           buffer + message_length + random_noise_length,
                                                           buffer_length - message_length - random_noise_length);

        if (hidden_message_length == 0)
        {
            return 0;
        }

        auto enc_length = model::padding::pkcs7::encode(buffer,
                                                        buffer_length,
                                                        message_length + hidden_message_length + random_noise_length,
                                                        model::aes128_ecb::block_size);

        if (enc_length == 0)
        {
            return 0;
        }

        model.encrypt(buffer, enc_length);

        return enc_length;
    }

    const char *get_hidden_message()
    {
        return (const char *) _hidden_message;
    }
};


TEST_F(set_2_6, run)
{

    breaks::byte_in_a_time_ecb_decryption b([&](const unsigned char *data,
                                                size_t data_length,
                                                unsigned char *buffer,
                                                size_t buffer_length) {
        return encryption_oracle(data, data_length, buffer, buffer_length);
    });

    char extracted_hidden_message[256] = {0};
    size_t extracted_hidden_message_length = b.run((unsigned char *) extracted_hidden_message,
                                                   sizeof(extracted_hidden_message));

    extracted_hidden_message[extracted_hidden_message_length] = 0;

    ASSERT_STREQ(extracted_hidden_message, get_hidden_message());
}