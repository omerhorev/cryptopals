//
// Created by omerh on 16/08/2019.
//

#include <gtest/gtest.h>
#include <random>
#include <climits>
#include <model/padding.h>
#include <breaks/moo-detector.h>
#include "model/block.h"
#include "utils/debug.h"

size_t encryption_oracle(unsigned char input[],
                         size_t length,
                         unsigned char output[],
                         size_t output_buffer_length,
                         bool &was_cbc_selected)
{
    std::random_device rd;
    std::uniform_int_distribution<size_t> length_dist(5, 10);
    std::uniform_int_distribution<unsigned char> byte_dist(0, 0xff);
    std::uniform_int_distribution<unsigned char> model_dist(0, 1);

    unsigned char iv[model::aes128_cbc::block_size];
    unsigned char key[model::aes128_cbc::block_size];

    std::generate_n(iv, sizeof(iv), [&]() {
        return byte_dist(rd);
    });

    std::generate_n(key, sizeof(key), [&]() {
        return byte_dist(rd);
    });

    size_t rand_length1 = length_dist(rd);
    size_t rand_length2 = length_dist(rd);
    size_t total_length = length + rand_length1 + rand_length2;
    size_t length_after_padding =
            total_length + model::aes128_cbc::block_size - (total_length % model::aes128_cbc::block_size);

    if (output_buffer_length < length_after_padding)
    {
        return 0;
    }

    std::generate_n(output, rand_length1, [&]() {
        return byte_dist(rd);
    });

    std::copy_n(input, length, output + rand_length1);

    std::generate_n(output + rand_length1 + length, rand_length2, [&]() {
        return byte_dist(rd);
    });

    auto padded_length = model::padding::pkcs7::encode(output,
                                                       output_buffer_length,
                                                       total_length,
                                                       model::aes128_cbc::block_size);

    if (model_dist(rd) == 1)
    {
        was_cbc_selected = false;
        model::aes128_ecb m;
        m.initialize(key, sizeof(key));
        m.encrypt(output, padded_length);
    }
    else
    {
        was_cbc_selected = true;
        model::aes128_cbc m;
        m.initialize(key, sizeof(key), iv, sizeof(iv));
        m.encrypt(output, padded_length);
    }

    return padded_length;
}

TEST(set_2_3, run)
{
    const size_t block_size = model::aes128_cbc::block_size;

    unsigned char input[block_size * 3] = {0};
    unsigned char ciphertext[block_size * 5] = {0};
    bool was_cbc_selected;

    for (int i = 0; i < 100; ++i)
    {
        auto length = encryption_oracle(input, sizeof(input), ciphertext, sizeof(ciphertext), was_cbc_selected);
        bool is_ecb_selected = !was_cbc_selected;

        ASSERT_EQ(breaks::moo_detector::is_ecb(ciphertext, length, model::aes128_cbc::block_size), is_ecb_selected);
    }


}
