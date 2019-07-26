//
// Created by omerh on 26/07/2019.
//

#ifndef CRYPTOPALS_DEBUG_H
#define CRYPTOPALS_DEBUG_H

#include <array>
#include "hex.h"

template <size_t N>
auto constexpr h2b(const char (&text)[N])
{
    const size_t l = (N - N%2);
    std::array<char, l/2> a { ._M_elems = {0}};
    utils::hex::decode(text, l, (unsigned char*)&a, l/2);

    return a;
}

#endif //CRYPTOPALS_DEBUG_H
