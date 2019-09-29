//
// Created by omerh on 29/09/2019.
//

#include <gtest/gtest.h>
#include <utils/random.h>
#include <utils/base64.h>
#include <model/block.h>
#include <model/padding.h>

class set_3_1 : public ::testing::Test
{
private:
    unsigned char _random_key[16] = {0};

protected:
    unsigned char _random_iv[16] = {0};

    void SetUp()
    {
        utils::random::instance().fill(_random_key);
        utils::random::instance().fill(_random_iv);
    }

protected:
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
};

TEST_F(set_3_1, run)
{
    const char *base64 = "MDAwMDAwTm93IHRoYXQgdGhlIHBhcnR5IGlzIGp1bXBpbmc=";
    unsigned char data[256];
    unsigned char plain[256];
    unsigned char cipher[256];
    auto data_length = utils::base64::decode(base64, strlen(base64), data, sizeof(data));

    auto cipher_length = encrypt(data, data_length, cipher, sizeof(cipher));

    auto plain_length = decrypt(cipher, cipher_length, plain, sizeof(plain));
}

