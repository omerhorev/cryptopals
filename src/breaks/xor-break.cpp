//
// Created by omerh on 22/07/2019.
//

#include <breaks/xor-break.h>
#include <cstdio>
#include <utils/string-score.h>
#include <cstring>

void breaks::xor_break::single_xor_cipher_break(const unsigned char *cipher, size_t cipher_length, char &o_key)
{
    auto plain = new unsigned char[cipher_length + 1];
    float highest_score = 0;

    // Make sure there's always null terminator because we are dilling with strings
    plain[cipher_length] = 0;

    for (int i = 0; i <= 0xff; ++i)
    {
        auto k = (unsigned char) i;

        xor_with_key(cipher, cipher_length, plain, k);

        float score = utils::string_score::score_by_frequency((const char *) plain, cipher_length);

        if (score > highest_score)
        {
            highest_score = score;
            o_key = k;
        }
    }

    delete[]plain;
}

void breaks::xor_break::xor_with_key(const unsigned char *cipher, size_t length, unsigned char *plain, unsigned char k)
{
    for (int i = 0; i < length; ++i)
    {
        plain[i] = cipher[i] ^ k;
    }
}

