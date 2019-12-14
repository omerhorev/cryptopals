//
// Created by omerh on 26/07/2019.
//

#ifndef CRYPTOPALS_DEBUG_H
#define CRYPTOPALS_DEBUG_H

#include <array>
#include "hex.h"
#include <iostream>

namespace utils
{
    namespace internal
    {
        struct hex_string
        {
            unsigned char *data;
            size_t length;
        };
    }

    template<size_t N>
    auto constexpr h2b(const char (&text)[N])
    {
        const size_t l = (N - N % 2);
        std::array<char, l / 2> a{._M_elems = {0}};
        utils::hex::decode(text, l, (unsigned char *) &a, l / 2);

        return a;
    }


    void print_divider();

    void print_buffer(const unsigned char data[], size_t length);

    void print_buffer_bits(const unsigned char data[], size_t length);

    void print_buffer(const char* name, const unsigned char data[], size_t length);

    void print_buffer_bits(const char* name, const unsigned char data[], size_t length);

    template<size_t N>
    void print_buffer(const unsigned char (&data)[N])
    {
        print_buffer(data, N);
    }

    template<size_t N>
    void print_buffer(const char* name, const unsigned char (&data)[N])
    {
        std::cout << name << ": ";

        print_buffer(data);
    }

    void print_blocked_buffer(const unsigned char data[], size_t length, size_t block_size);
    void print_blocked_buffer(const char* name, const unsigned char data[], size_t length, size_t block_size);
}

#endif //CRYPTOPALS_DEBUG_H
