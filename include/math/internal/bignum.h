//
// Created by omerh on 01/12/2019.
//

#ifndef CRYPTOPALS_BIGNUM_H
#define CRYPTOPALS_BIGNUM_H

#include <cstddef>

namespace math
{
    namespace internal
    {
        class bignum
        {
        public:
            static void set(unsigned char *number, size_t length, const unsigned char *value, size_t value_length);

            static void add(unsigned char *number, size_t length, const unsigned char *value, size_t value_length);

            static void subtract(unsigned char *number, size_t length, const unsigned char *value, size_t value_length);

            static bool equal(const unsigned char *first, size_t first_length,
                                const unsigned char *second, size_t second_length);
        };
    }
}

#endif //CRYPTOPALS_BIGNUM_H
