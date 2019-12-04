//
// Created by omerh on 04/12/2019.
//

#ifndef CRYPTOPALS_NUMBERS_H
#define CRYPTOPALS_NUMBERS_H

#include "internal/number.h"
#include "internal/fields/Z.h"

namespace math
{
    namespace internal
    {
        template<size_t BitSize>
        class Z_number : private internal::fields::Z, public internal::number<BitSize>
        {
        public:
            Z_number() : field(), internal::number<BitSize>(this)
            {}

            Z_number(unsigned int number) : internal::number<BitSize>(this, number)
            {}
        };
    }
    using num32_t = internal::Z_number<32>;
    using num64_t = internal::Z_number<64>;
    using num128_t = internal::Z_number<128>;
    using num256_t = internal::Z_number<256>;
    using num512_t = internal::Z_number<512>;
    using num1024_t = internal::Z_number<1024>;
    using num2048_t = internal::Z_number<2048>;
    using num4096_t = internal::Z_number<4096>;
}

#endif //CRYPTOPALS_NUMBERS_H
