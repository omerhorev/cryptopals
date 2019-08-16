//
// Created by omerh on 16/08/2019.
//

#include "utils/debug.h"

void utils::print_buffer(const unsigned char *data, size_t length)
{
    char *string = new char[length * 2 + 1];

    string[length * 2] = 0;

    hex::encode(data, length, string, length * 2);

    std::cout << string << " (" << length << "b)" << std::endl;

    delete[] string;
}

void utils::print_buffer(const char *name, const unsigned char *data, size_t length)
{
    std::cout << name << ": ";

    print_buffer(data, length);
}
