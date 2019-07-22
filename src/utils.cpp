//
// Created by omerh on 22/07/2019.
//

#include <cstddef>
#include "utils.h"

size_t utils::xorBuffers(const unsigned char *data1, size_t length1, const unsigned char *data2, size_t length2,
                         unsigned char *result, size_t result_length)
{
    if (length1 != length2 || length2 != result_length || result_length != length1)
    {
        return 0;
    }

    for (int i = 0; i < length1; ++i)
    {
        result[i] = data1[i] ^ data2[i];
    }

    return length1;
}
