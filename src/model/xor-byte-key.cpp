//
// Created by omerh on 26/07/2019.
//

#include <model/xor-byte-key.h>

using namespace model;

void xor_byte_key::decrypt(const unsigned char *cipher, unsigned char *plain, size_t length, unsigned char key)
{
    xor_single_byte_key(cipher, plain, length, key);
}

void xor_byte_key::encrypt(const unsigned char *plain, unsigned char *cipher, size_t length, unsigned char key)
{
    xor_single_byte_key(plain, cipher, length, key);
}

void xor_byte_key::xor_single_byte_key(const unsigned char *src, unsigned char *dst, size_t length, unsigned char key)
{
    for (int i = 0; i < length; ++i)
    {
        dst[i] = src[i] ^ key;
    }
}

