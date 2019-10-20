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

    template<size_t block_size>
    std::array<unsigned char, block_size> generate_iteration_mask(const std::array<unsigned char, block_size> base_mask,
                                                                  unsigned int padding_length)
    {
        //
        // We know that if we encrypt with the base mask like this:
        //        block 1  |  block 2
        //     [   mask   ] [  block   ]
        //
        // It will result a plaintext with all zeros until the 'padding_length-1' byte (from the end).
        // Therefor, if we prepare the iteration mask for padding_length = i we need to modify each value in the mask
        // so the calculation will result the values 'padding-length' until the 'padding_length-1' (from the end)
        //
        std::array<unsigned char, block_size> iter_mask = {0};

        if (padding_length == 0)
        {
            throw std::runtime_error("There is no padding = 0, only padding=block_size in case of no padding");
        }

        for (int i = 0; i < padding_length - 1; ++i)
        {
            iter_mask[block_size - i - 1] = base_mask[block_size - i - 1] ^ padding_length;
        }

        return iter_mask;
    }

    auto generate_break_blocks_pairs(const unsigned char iv[],
                                     unsigned char cipher[],
                                     size_t cipher_length,
                                     size_t block_size)
    {
        std::vector<std::pair<const unsigned char *, const unsigned char *>> v;

        std::copy_n(cipher, cipher_length, cipher);

        breaks::internal::ecb_block_parser helper(cipher, cipher_length, block_size);

        for (int i = helper.blocks_count() - 1; i >= 1; --i)
        {
            v.emplace_back(helper[i - 1].data(), helper[i].data());
        }

        v.emplace_back(iv, helper[0].data());

        return v;
    }
};

TEST_F(set_3_1, run)
{
    auto base64 = "MDAwMDAwTm93IHRoYXQgdGhlIHBhcnR5IGlzIGp1bXBpbmc=";
    unsigned char *iv = _random_iv;
    unsigned char data[256];
    unsigned char cipher[256];

    const auto data_length = utils::base64::decode(base64, strlen(base64), data, sizeof(data));
    const auto cipher_length = encrypt(data, data_length, cipher, sizeof(cipher));
    const auto block_size = model::aes128_cbc::block_size;
    const auto plain_length = cipher_length;
    auto plain = new unsigned char[plain_length + 1];

    unsigned int write_index = plain_length - 1;

    std::fill_n(plain, plain_length + 1, 0);

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
    //      1 =        I[last] ^ C'-prev[last]           ->
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
    std::array<unsigned char, block_size> base_mask = {0};

    auto blocks_break_pairs = generate_break_blocks_pairs(iv, cipher, cipher_length, block_size);

    for (auto a : blocks_break_pairs)
    {
        for (int byte_index = block_size - 1; byte_index >= 0; byte_index--)
        {
            bool found = false;
            auto padding_length = block_size - byte_index;
            std::array<unsigned char, block_size> iteration_mask = generate_iteration_mask(base_mask, padding_length);

            for (auto j = 0; j < UCHAR_MAX; ++j)
            {
                auto _C = (unsigned char) j;
                auto C = a.first[byte_index];
                auto _P = (unsigned char) padding_length;
                auto I = _C ^_P;
                auto P = I ^C;

                iteration_mask[byte_index] = _C;

                //
                // Create a special cipher containing the two blocks, the 'mask' block and the block we are breaking
                //        block 1  |  block 2
                //     [   mask   ] *[  block   ]
                //
                std::array<std::array<unsigned char, block_size>, 2> cipher_two_blocks = {0};
                std::copy_n(iteration_mask.begin(), block_size, cipher_two_blocks[0].begin());
                std::copy_n(a.second, block_size, cipher_two_blocks[1].begin());

                if (is_padding_valid((unsigned char *) &cipher_two_blocks, block_size * 2, padding_length))
                {
                    base_mask[byte_index] = _C ^ _P;

                    plain[write_index--] = (unsigned char) (P);
                    found = true;

                    break;
                }
            }

            if (!found)
            {
                throw std::runtime_error("Internal error, did not found any matching byte, must be a bug");
            }
        }
    }

    ASSERT_EQ(write_index, -1);

    std::cout << (char *) plain << std::endl;

    delete[] plain;
    // ASSERT_TRUE(is_padding_valid(cipher, cipher_length));
}

