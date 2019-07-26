//
// Created by omerh on 26/07/2019.
//
#include <utils/hamming-distance.h>

using namespace utils;

unsigned int hamming_distance::calculate(const unsigned char buffer1[],
                                                const unsigned char buffer2[],
                                                size_t length)
{
    unsigned int dist = 0;

    for (int i = 0; i < length; ++i)
    {
        unsigned char d = buffer1[i] ^ buffer2[i];

        dist += ones_in_nible[(d & 0xf0) >> 4];
        dist += ones_in_nible[(d & 0x0f) >> 0];
    }

    return dist;
}

const unsigned int hamming_distance::ones_in_nible[] = {0, 1, 1, 2, 1, 2, 2, 3, 1, 2, 2, 3, 2, 3, 3, 4};