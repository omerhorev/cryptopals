//
// Created by omerh on 26/07/2019.
//

#include <model/xor-byte-key.h>

using namespace model;

void xor_byte_key::decrypt(const unsigned char *cipher, unsigned char *plain, size_t length, unsigned char key)
{
    xor_repeating_key(cipher, plain, length, &key, 1);
}

void xor_byte_key::decrypt(const unsigned char *cipher,
                           unsigned char *plain,
                           size_t length,
                           const unsigned char *key,
                           size_t key_length)
{
    xor_repeating_key(cipher, plain, length, key, key_length);
}

void xor_byte_key::encrypt(const unsigned char *plain, unsigned char *cipher, size_t length, unsigned char key)
{
    xor_repeating_key(plain, cipher, length, &key, 1);
}

void xor_byte_key::encrypt(const unsigned char *plain,
                           unsigned char *cipher,
                           size_t length,
                           const unsigned char *key,
                           size_t key_length)
{
    xor_repeating_key(plain, cipher, length, key, key_length);
}

void xor_byte_key::xor_repeating_key(const unsigned char *src,
                                     unsigned char *dst,
                                     size_t length,
                                     const unsigned char *key,
                                     size_t key_length)
{
    for (auto i = 0; i < length; i++)
    {
        auto j = (i % key_length);

        dst[i] = src[i] ^ key[j];
    }
}
