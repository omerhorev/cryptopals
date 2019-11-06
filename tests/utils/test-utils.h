//
// Created by omerh on 29/10/2019.
//

#ifndef CRYPTOPALS_TEST_UTILS_H
#define CRYPTOPALS_TEST_UTILS_H

#include <cstdio>
#include <utils/base64.h>
#include <cstring>

namespace test_utils
{
    template <size_t MaxBufferSize = 256>
    struct base64_test_vector
    {
        base64_test_vector(const char* base64)
        {
            length = utils::base64::decode(base64, strlen(base64), data, MaxBufferSize);
        }

        size_t length;
        unsigned char data[MaxBufferSize];
    };
}

#endif //CRYPTOPALS_TEST_UTILS_H
