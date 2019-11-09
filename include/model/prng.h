//
// Created by omerh on 08/11/2019.
//

#ifndef CRYPTOPALS_PRNG_H
#define CRYPTOPALS_PRNG_H

#include <model/internal/prng/mersenne-twister.h>

namespace model
{
    /**
     * An mt19935 Mersenne-Twister according to the standard as published by Makoto Matsumoto and Takuji Nishimura.
     */
    using mt19937 = model::internal::prng::mersenne_twister<
            uint32_t,
            624,
            32,
            0x9908B0DF,
            397,
            31,
            1812433253,
            11, 0xFFFFFFFF,
            7, 0x9D2C5680,
            15, 0xEFC60000,
            18>;
}

#endif //CRYPTOPALS_PRNG_H
