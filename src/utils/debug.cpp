//
// Created by omerh on 16/08/2019.
//

#include <utils/general.h>
#include "utils/debug.h"

void utils::print_buffer(const unsigned char *data, size_t length)
{
    char *string = new char[length * 2 + 1];

    hex::encode(data, length, string, length * 2);

    string[length * 2] = 0;

    std::cout << string << " (" << length << " bytes)" << std::endl;

    delete[] string;
}

void utils::print_buffer_bits(const unsigned char *data, size_t length)
{
    const char *nible[] = {
            "0000", "0001", "0010", "0011",
            "0100", "0101", "0110", "0111",
            "1000", "1001", "1010", "1011",
            "1100", "1101", "1110", "1111"
    };

    for (auto i = 0; i < length; ++i)
    {
        std::cout << nible[(data[i] & 0xf0) >> 4];
        std::cout << nible[(data[i] & 0x0f) >> 0];
        std::cout << " ";
    }

    std::cout << "(" << length << " bytes)" << std::endl;
}


void utils::print_buffer(const char *name, const unsigned char *data, size_t length)
{
    std::cout << name << ": ";

    print_buffer(data, length);
}

void utils::print_buffer_bits(const char *name, const unsigned char *data, size_t length)
{
    std::cout << name << ": ";

    print_buffer_bits(data, length);
}


void utils::print_blocked_buffer(const char *name, const unsigned char data[], size_t length, size_t block_size)
{
    std::cout << name << ": ";
    print_blocked_buffer(data, length, block_size);
}

void utils::print_blocked_buffer(const unsigned char data[], size_t length, size_t block_size)
{
    char *string = new char[length * 2 + 1];

    string[length * 2] = 0;

    hex::encode(data, length, string, length * 2);

    for (int i = 0; i < (length); ++i)
    {
        std::cout << string[i * 2] << string[i * 2 + 1];

        if (i % block_size == block_size - 1)
        {
            std::cout << " ";
        }
    }

    std::cout << " (" << length << " bytes)" << std::endl;
    delete[] string;
}