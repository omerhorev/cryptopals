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

    std::cout << string << " (" << length << "b)" << std::endl;

    delete[] string;
}

void utils::print_buffer(const char *name, const unsigned char *data, size_t length)
{
    std::cout << name << ": ";

    print_buffer(data, length);
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

    std::cout << " (" << length << "b)" << std::endl;
    delete[] string;
}