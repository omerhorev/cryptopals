//
// Created by omerh on 20/07/2019.
//

#include <hex.h>

size_t hex::decode(const char hex[], size_t length, unsigned char buffer[], size_t buffer_length)
{
    if (length % 2 != 0)
    {
        return 0;
    }

    if (buffer_length < length / 2)
    {
        return 0;
    }

    for (auto i = 0; i < length; i += 2)
    {
        auto j = (i / 2);
        buffer[j] = (unsigned char) ((decode_char(hex[i]) << 4) | (decode_char(hex[i + 1])));
    }

    return (length / 2);
}

size_t hex::encode(const unsigned char buffer[], size_t length, char hex[], size_t hex_length)
{
    if (hex_length < length * 2)
    {
        return 0;
    }

    for (auto i = 0; i < length; i++)
    {
        hex[i * 2] = encode_nible((unsigned int) (buffer[i] & 0xf0) >> 4);
        hex[i * 2 + 1] = encode_nible((unsigned int) buffer[i] & 0x0f);
    }

    return length * 2;
}
