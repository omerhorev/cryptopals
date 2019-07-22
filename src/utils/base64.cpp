//
// Created by omerh on 16/07/2019.
//

#include "utils/base64.h"

using namespace utils;

const char base64::padd_char = '=';
const char base64::encoded_char_table[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N',
                                           'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z', 'a', 'b',
                                           'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p',
                                           'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3',
                                           '4', '5', '6', '7', '8', '9', '+', '/',};

unsigned char base64::decode_char(char encoded_char)
{
    if (false);
    else if (encoded_char >= 'A' && encoded_char <= 'Z') return (unsigned char) (encoded_char - 'A' + 0);
    else if (encoded_char >= 'a' && encoded_char <= 'z') return (unsigned char) (encoded_char - 'a' + 26);
    else if (encoded_char >= '0' && encoded_char <= '9') return (unsigned char) (encoded_char - '0' + 52);
    else if (encoded_char == '+') return 62;
    else if (encoded_char == '/') return 63;
    else if (encoded_char == '=') return 0;

    return 0xFF;
}

size_t base64::decode(const char *base64, size_t length, unsigned char *buffer, size_t buffer_length)
{
    if (length == 0)
    {
        return 0;
    }

    size_t decoded_buffer_length = 0;
    auto number_of_encoding_groups = (length / chars_in_encoding_group);

    for (auto i = 0; i < number_of_encoding_groups; i++)
    {
        auto read_index = i * chars_in_encoding_group;
        auto write_index = i * bytes_in_encoding_group;
        auto group_buffer = &buffer[write_index];

        auto decoded1 = decode_char(base64[read_index + 0]);
        auto decoded2 = decode_char(base64[read_index + 1]);
        auto decoded3 = decode_char(base64[read_index + 2]);
        auto decoded4 = decode_char(base64[read_index + 3]);

        if (decoded1 > 0x3F || decoded2 > 0x3F || decoded3 > 0x3F || decoded4 > 0x3F)
        {
            return 0;
        }

        decoded_buffer_length++;
        group_buffer[0] = (unsigned char) (((decoded1 & 0x3f) << 2) | ((decoded2 & 0x30) >> 4));

        if (base64[read_index + 2] == padd_char)
        {
            break;
        }

        decoded_buffer_length++;
        group_buffer[1] = (unsigned char) (((decoded2 & 0x0f) << 4) | ((decoded3 & 0x3c) >> 2));

        if (base64[read_index + 3] == padd_char)
        {
            break;
        }


        group_buffer[2] = (unsigned char) (((decoded3 & 0x03) << 6) | ((decoded4 & 0x3f)));
        decoded_buffer_length++;
    }

    return decoded_buffer_length;
}

size_t base64::encode(const unsigned char *buffer, size_t length, char *base64, size_t base64_length)
{
    if (length == 0)
    {
        return 0;
    }

    auto number_of_encoding_groups = ((length - 1) / bytes_in_encoding_group) + 1;

    if (base64_length < number_of_encoding_groups * chars_in_encoding_group)
    {
        return 0;
    }

    for (auto i = 0; i < number_of_encoding_groups; ++i)
    {
        auto read_index = i * bytes_in_encoding_group;
        auto write_index = i * chars_in_encoding_group;
        auto group_buffer = &buffer[read_index];
        auto group_base64_text = &base64[write_index];
        unsigned int byte1 = read_index + 0 < length ? group_buffer[0] : 0;
        unsigned int byte2 = read_index + 1 < length ? group_buffer[1] : 0;
        unsigned int byte3 = read_index + 2 < length ? group_buffer[2] : 0;
        unsigned int six_bits = 0;

        six_bits = (byte1 & 0xfc) >> 2;
        group_base64_text[0] = encoded_char_table[six_bits];

        six_bits = ((byte1 & 0x3) << 4) | ((byte2 & 0xf0) >> 4);
        group_base64_text[1] = encoded_char_table[six_bits];

        if (read_index + 1 == length)
        {
            group_base64_text[2] = padd_char;
            group_base64_text[3] = padd_char;
            break;
        }

        six_bits = ((byte2 & 0x0f) << 2) | ((byte3 & 0xc0) >> 6);
        group_base64_text[2] = encoded_char_table[six_bits];

        if (read_index + 2 == length)
        {
            group_base64_text[3] = padd_char;
            break;
        }

        six_bits = (byte3 & 0x3f);
        group_base64_text[3] = encoded_char_table[six_bits];
    }

    return number_of_encoding_groups * chars_in_encoding_group;
}
