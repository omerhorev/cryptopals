//
// Created by omerh on 22/07/2019.
//

#include <breaks/xor-break.h>
#include <cstdio>
#include <utils/string-score.h>
#include <cstring>
#include <model/xor-byte-key.h>

void breaks::xor_break::single_xor_cipher_break(const unsigned char *cipher, size_t cipher_length, unsigned char &o_key)
{
    float _;
    single_xor_cipher_break(cipher, cipher_length, o_key, _);
}

void breaks::xor_break::single_xor_cipher_break(const unsigned char *cipher, size_t cipher_length, unsigned char &o_key,
                                                float &o_accuracy)
{
    auto plain = new unsigned char[cipher_length + 1];
    float highest_score = 0;

    // Make sure there's always null terminator because we are dilling with strings
    plain[cipher_length] = 0;

    for (int i = 0; i <= 0xff; ++i)
    {
        auto k = (unsigned char) i;

        model::xor_byte_key::decrypt(cipher, plain, cipher_length, k);

        float score = utils::string_score::score_by_frequency((const char *) plain, cipher_length);

        if (score > highest_score)
        {
            highest_score = score;
            o_key = k;
            o_accuracy = score;
        }
    }

    delete[]plain;
}

