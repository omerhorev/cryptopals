//
// Created by omerh on 29/09/2019.
//

#include <gtest/gtest.h>
#include <utils/random.h>
#include <utils/base64.h>
#include <model/block.h>
#include <model/padding.h>
#include <breaks/internal/ecb-block-parser.h>
#include <breaks/padding-oracle.h>

class set_3_1 : public ::testing::Test
{
public:
    unsigned char _random_key[16] = {0};
    unsigned char _random_iv[16] = {0};

    void SetUp()
    {
        utils::random::instance().fill(_random_key);
        utils::random::instance().fill(_random_iv);
    }

    size_t encrypt(const unsigned char msg[], size_t msg_length, unsigned char buffer[], size_t buffer_length)
    {
        model::aes128_cbc model;
        model.initialize(_random_key, sizeof(_random_key), _random_iv, sizeof(_random_iv));

        if (buffer_length < msg_length)
        {
            throw std::length_error("Buffer is too small for the message");
        }

        std::copy_n(msg, msg_length, buffer);

        auto l = model::padding::pkcs7::encode(buffer, buffer_length, msg_length, model::aes128_cbc::block_size);
        model.encrypt(buffer, l);

        return l;
    }

    size_t decrypt(const unsigned char cipher[], size_t cipher_length, unsigned char buffer[], size_t buffer_length)
    {
        model::aes128_cbc model;
        model.initialize(_random_key, sizeof(_random_key), _random_iv, sizeof(_random_iv));

        auto temp = new unsigned char[cipher_length];

        std::copy_n(cipher, cipher_length, temp);
        model.decrypt(temp, cipher_length);

        auto l = model::padding::pkcs7::decode(temp, cipher_length, model::aes128_cbc::block_size);

        if (buffer_length < l)
        {
            throw std::length_error("The buffer is too small");
        }

        std::copy_n(temp, l, buffer);

        delete[] temp;

        return l;
    }

    bool is_padding_valid(const unsigned char cipher[], size_t length, size_t expected_padding_length)
    {
        unsigned char temp[length];
        bool padding_good = true;

        try
        {
            decrypt(cipher, length, temp, length);
        }
        catch (model::internal::padding::pkcs7::invalid_padding &e)
        {
            // Lets make sure we didn't ruin anything else...
            // The reason it only works with padding length greater than one is that when the padding is 1 the
            // padding length determined during decryption is derived from the first byte, so we cant really
            // rely on it to be good...
            if (e.pad_index_error != expected_padding_length - 1 && expected_padding_length > 1)
            {
                throw std::runtime_error(
                        "Expected a possible padding error at 'expected_padding_length', got a different error instead");
            }

            padding_good = false;
        }

        return padding_good;
    };

};

TEST_F(set_3_1, run)
{
    auto vectors = {
            "MDAwMDAwTm93IHRoYXQgdGhlIHBhcnR5IGlzIGp1bXBpbmc=",
            "MDAwMDAxV2l0aCB0aGUgYmFzcyBraWNrZWQgaW4gYW5kIHRoZSBWZWdhJ3MgYXJlIHB1bXBpbic=",
            "MDAwMDAyUXVpY2sgdG8gdGhlIHBvaW50LCB0byB0aGUgcG9pbnQsIG5vIGZha2luZw==",
            "MDAwMDAzQ29va2luZyBNQydzIGxpa2UgYSBwb3VuZCBvZiBiYWNvbg==",
            "MDAwMDA0QnVybmluZyAnZW0sIGlmIHlvdSBhaW4ndCBxdWljayBhbmQgbmltYmxl",
            "MDAwMDA1SSBnbyBjcmF6eSB3aGVuIEkgaGVhciBhIGN5bWJhbA==",
            "MDAwMDA2QW5kIGEgaGlnaCBoYXQgd2l0aCBhIHNvdXBlZCB1cCB0ZW1wbw==",
            "MDAwMDA3SSdtIG9uIGEgcm9sbCwgaXQncyB0aW1lIHRvIGdvIHNvbG8=",
            "MDAwMDA4b2xsaW4nIGluIG15IGZpdmUgcG9pbnQgb2g=",
            "MDAwMDA5aXRoIG15IHJhZy10b3AgZG93biBzbyBteSBoYWlyIGNhbiBibG93"
    };

    for (auto vector : vectors)
    {
        unsigned char data[256];
        unsigned char cipher[256];
        unsigned char plain[256];

        const auto data_length = utils::base64::decode(vector, strlen(vector), data, sizeof(data));
        const auto cipher_length = encrypt(data, data_length, cipher, sizeof(cipher));
        const auto block_size = model::aes128_cbc::block_size;

        breaks::padding_oracle<block_size> b(cipher,
                                             cipher_length,
                                             [&](const unsigned char cipher[], size_t length,
                                                 size_t expected_padding_length) -> bool {
                                                 return is_padding_valid(cipher, length, expected_padding_length);
                                             });

        b.run(plain, cipher_length, _random_iv);

        ASSERT_TRUE(std::equal(data, data + data_length, plain));
    }
}

