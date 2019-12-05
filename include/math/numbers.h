//
// Created by omerh on 04/12/2019.
//

#ifndef CRYPTOPALS_NUMBERS_H
#define CRYPTOPALS_NUMBERS_H

#include "internal/number.h"
#include "math/internal/bignum.h"

namespace math
{
    using num32_t = internal::number<32>;
    using num64_t = internal::number<64>;
    using num128_t = internal::number<128>;
    using num256_t = internal::number<256>;
    using num512_t = internal::number<512>;
    using num1024_t = internal::number<1024>;
    using num2048_t = internal::number<2048>;
    using num4096_t = internal::number<4096>;
}

#endif //CRYPTOPALS_NUMBERS_H
