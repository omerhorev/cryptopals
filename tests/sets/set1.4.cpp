//
// Created by omerh on 25/07/2019.
//

#include <utils/hex.h>
#include <breaks/xor-break.h>
#include <fstream>
#include <utils/files.h>
#include <model/xor-byte-key.h>
#include "gtest/gtest.h"

using namespace utils;

TEST(sets, set_1_4)
{
    char key = 0;
    float max_accuracy = 0;
    breaks::xor_break b;
    const size_t max_buffer_length = 256;
    unsigned char read_buffer[max_buffer_length] = {0};
    unsigned char xor_encrypted_cipher_buffer[max_buffer_length] = {0};
    ssize_t read_length;
    size_t xor_encrypted_cipher_buffer_length = 0;

    std::ifstream f;
    f.open("data/sets/4/4.txt", std::fstream::in);

    ASSERT_TRUE(f.is_open());

    read_length = files::read_hex_line(f, read_buffer, sizeof(read_buffer));

    while(read_length > 0)
    {
        float a;
        char k;

        b.single_xor_cipher_break(read_buffer, (size_t)(read_length), k, a);

        if (a > max_accuracy)
        {
            key = k;
            xor_encrypted_cipher_buffer_length = (size_t)read_length;
            max_accuracy = a;
            memcpy(xor_encrypted_cipher_buffer, read_buffer, xor_encrypted_cipher_buffer_length);
        }

        read_length = files::read_hex_line(f, read_buffer, sizeof(read_buffer));
    }

    char plain[max_buffer_length] = {0};
    model::xor_byte_key::decrypt(xor_encrypted_cipher_buffer, (unsigned char*)plain, xor_encrypted_cipher_buffer_length, key);

    ASSERT_EQ(key, 0x35);
    ASSERT_STREQ(plain, "Now that the party is jumping\n");
}