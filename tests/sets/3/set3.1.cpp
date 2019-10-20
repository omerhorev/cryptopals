//
// Created by omerh on 29/09/2019.
//

#include <gtest/gtest.h>
#include <utils/random.h>
#include <utils/base64.h>
#include <model/block.h>
#include <model/padding.h>
#include <breaks/internal/ecb-block-parser.h>

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

    bool is_padding_valid(const unsigned char cipher[], size_t length)
    {
        auto temp = new unsigned char[length];
        bool padding_good = true;

        try
        {
            decrypt(cipher, length, temp, length);
        }
        catch (model::internal::padding::pkcs7::invalid_padding &e)
        {
            padding_good = false;
        }

        delete[] temp;

        return padding_good;
    };
};

TEST_F(set_3_1, run)
{
    auto base64 = "MDAwMDAwTm93IHRoYXQgdGhlIHBhcnR5IGlzIGp1bXBpbmc=";
    unsigned char data[256];
    unsigned char cipher[256];

    auto data_length = utils::base64::decode(base64, strlen(base64), data, sizeof(data));
    auto cipher_length = encrypt(data, data_length, cipher, sizeof(cipher));
    breaks::internal::ecb_block_parser helper(cipher, cipher_length, model::aes128_cbc::block_size);

    const auto block_size = model::aes128_cbc::block_size;

    //
    // Lets figure out the last char of the intermediate state
    // We know that the the P[last] = I[last] ^ C-prev[last]
    //
    // So, if we change C-prev[last] (and name it C'-prev) so I[last] ^ C'[last] == 1 (meaning, P'[last] == 1) we
    // would'nt get padding error, that means that we can iterate over C'-prev[last] and check the padding unit we
    // discover the right value.
    //
    // After discovering it, we know that:
    //      P'[last] = I[last] ^ C'-prev[last]           ->
    //      1 =        I[last] ^ C'-prev[last]                  ->
    //      1 ^ C'-prev[last] = I[last]
    //
    // And...
    //      P[last] = I[last] ^ C-prev[last]             ->
    //      P[last] ^ I[last] = C-prev[last]             ->
    //      I[last] = P[last] ^ C-prev[last]
    //
    // So...
    //      1 ^ C'-prev[last] = I[last]                  ->
    //      1 ^ C'-prev[last] = P[last] ^ C-prev[last]   ->
    //   ____________________________________________________
    //  |                                                    |
    //  |   P[last] = C-prev[last] ^ C'-prev[last] ^ 1       |
    //  |____________________________________________________|
    //

    std::array<unsigned char, block_size> base_mask = {0};

    unsigned char original_prev_block_last_byte = helper[0].data()[block_size - 1];

    for (int i = 0; i < CHAR_MAX; ++i)
    {
        auto _C = (unsigned char) i;
        helper[0].data()[block_size - 1] = _C;

        if (is_padding_valid(helper[0].data(), block_size * 2))
        {
            unsigned char _P = 1;
            unsigned char I = _C ^ _P;
            unsigned char P = I ^ original_prev_block_last_byte;

            std::cout << "p = " << char(P) << std::endl;
        }
    }

    //
    // Given P'-prev[last] ^ I[last] = 1 and I[last] is constant, we need to change P'-prev[last] so it will increase
    //
    //     P'-prev[last] ^ I[last] = 1                  ->
    //     P'-prev[last] ^ I[last] ^ 1 = 0
    //
    // Therefore, after finding the first byte (and actually and byte) in the padding (the byte with padding=1)
    // we need to move one (ie, padding=2, P[last]=2, P[last-1]=2)
    //
    // So, after finding the valid padding we will set the base mask so the P[i]=0)
    // And so we can work on the base mask and mask it will result the correct values
    //
    // For example, lets mark the base mask as M for each block. We already know that:
    //
    //     P[last] = C-prev[last] ^ C'-prev[last] ^ 1
    //
    // Therefore, we can write it as follow (given i is the index for the byte and the and p is the padding length)
    //
    //     P[last] = C-prev[i] ^ M[i] ^ p
    //
    // ASSERT_TRUE(is_padding_valid(cipher, cipher_length));
}

