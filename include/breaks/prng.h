//
// Created by omerh on 11/11/2019.
//

#ifndef CRYPTOPALS_BREAKS_PRNG_H
#define CRYPTOPALS_BREAKS_PRNG_H

#include <cstdint>
#include <utils/bits.h>

namespace breaks
{
    class mt19973
    {
    public:
        /**
         * Given the first element the mt19973 engine provided, crack the original seed.
         *
         * @param element The first element provided by the engine
         * @return (uint32_t) The seed used to initialize the mt19973 engine
         */
        static uint32_t get_seed_from_first_element(uint32_t element)
        {
            return untamper(element);
        }

        static uint32_t untamper(uint32_t value)
        {
            /**
             * The original tempering method:
             *
             *     v ^= ((v >> ParamU) & ParamD);
             *     v ^= ((v << ParamS) & ParamB);
             *     v ^= ((v << ParamT) & ParamC);
             *     v ^= (v >> ParamL);
             *
             * And the params is:
             *
             *     (w, n, m, r) = (32, 624, 397, 31)
             *     a = 9908B0DF
             *     (u, d) = (11, FFFFFFFF)
             *     (s, b) = (7, 9D2C5680)
             *     (t, c) = (15, EFC60000)
             *     l = 18
             */

            unsigned const U = 11, D = 0xffffffff;
            unsigned const S = 7, B = 0x9d2c5680;
            unsigned const T = 15, C = 0xefc60000;
            unsigned const L = 18;

            value = reverse_shift_right_xor_and(value, L, 0xffffffff);
            value = reverse_shift_left_xor_and(value, T, C);
            value = reverse_shift_left_xor_and(value, S, B);
            value = reverse_shift_right_xor_and(value, U, D);

            return value;
        }

        /**
         * Given the expression 'value = (number << offset) & mask', retrieves 'number' from a known
         * value, offset and mask.
         *
         * @param value  The 'value' in the expression described.
         * @param offset The 'offset' in the expression described
         * @param mask   The 'mask' in the expression described
         * @return (uint32_t) The 'number' in the expression described
         */
        static uint32_t reverse_shift_left_xor_and(uint32_t value, uint32_t offset, uint32_t mask)
        {
            /**
             * Y = X ^ ((X << S) & M)
             *
             * Firstable, lets calculate '(X << S) & M' (and for example assume S=3)
             *
             *     x4         x3         x2         x1         x0         0          0          0
             *     m7         m6         m5         m4         m3         m2         m1         m0
             *     ---AND---------------------------------------------------------------------------
             *     x4&m7      x3&m6      x2&m5      x1&m4      x0&m3      0          0          0
             *
             * Now, lets calculate:
             *
             *     x7         x6         x5         x4         x3         x2         x1         x0
             *     x4&m7      x3&m6      x2&m5      x1&m4      x0&m3      0          0          0
             *     ---XOR---------------------------------------------------------------------------
             *     x7^(x4&m7) x6^(x3&m6) x5^(x2&m5) x4^(x1&m4) x3^(x0&m3) x2         x1         x0
             *
             * If so...
             *
             *     y0 = x0 ^ 0
             *     y1 = x1 ^ 0
             *     y2 = x2 ^ 0
             *     y3 = x3 ^ (x0   & m3)
             *     yi = xi ^ (xi-M & mi)
             *
             * Mix things up:
             *     x0 = y0
             *     x1 = y1
             *     x2 = y2
             *     x3 = y3 ^ (x0   & m3)
             *     xi = yi ^ (xi-M & mi)
             *
             * So the algorithm is:
             *
             *     for i to N:
             *         if i < N:
             *             x[i] = y[i]
             *         else:
             *             x[i] = y[i] ^ (x[i-M] & m[i])
             */
            const auto bits_in_uint32_t = 32;
            uint32_t X = 0;
            uint32_t Y = value;
            unsigned int S = offset;
            typedef utils::bits<uint32_t> bits;

            for (unsigned int i = 0; i < bits_in_uint32_t; i++)
            {
                if (i < S)
                {
                    X = bits::change(X, i, bits::get(Y, i));
                }
                else
                {
                    X = bits::change(X, i, bits::get(Y, i) ^ (bits::get(X, i - S) & bits::get(mask, i)));
                }
            }

            return X;
        }

        /**
         * Given the expression 'value = (number >> offset) & mask', retrieves 'number' from a known
         * value, offset and mask.
         *
         * @param value  The 'value' in the expression described.
         * @param offset The 'offset' in the expression described
         * @param mask   The 'mask' in the expression described
         * @return (uint32_t) The 'number' in the expression described
         */
        static uint32_t reverse_shift_right_xor_and(uint32_t value, uint32_t offset, uint32_t mask)
        {
            /**
             * Y = X ^ ((X >> S) & M)
             *
             * Firstable, lets calculate '(X >> S) & M' (and for example assume S=3)
             *
             *     0          0          0          x7         x6         x5         x4         x3
             *     m7         m6         m5         m4         m3         m2         m1         m0
             *     ---AND---------------------------------------------------------------------------
             *     0          0          0          x7&m4      x6&m3      x5&m2      x4&m1      x3&m0
             *
             * Now, lets calculate the 'X ^ ...' part:
             *
             *     x7         x6         x5         x4         x3         x2         x1         x0
             *     0          0          0          x7&m4      x6&m3      x5&m2      x4&m1      x3&m0
             *     ---XOR---------------------------------------------------------------------------
             *     x7         x6         x5         x4^(x7&m4) x3^(x6&m3) x2^(x5&m2) x1^(x4&m1) x0^(x3&m0)
             *
             * If so... (for i<N-S)
             *
             *     yi = xi^(x(i+S) & mi)
             *     ...
             *     y5 = x5
             *     y6 = x7
             *     y7 = x7
             *
             * Mix things up:
             *
             *     for i<=N-S: xi = yi ^ (x(i+S) & mi)
             *     else:       xi = yi
             *
             * So the algorithm is:
             *
             *     for i from N-1 to 0:
             *         if i > N-S:
             *             x[i] = y[i]
             *         else:
             *             x[i] = y[i] ^ (x[i+S] & m[i])
             *
             */

            const auto bits_in_uint32_t = 32;
            uint32_t X = 0;
            uint32_t Y = value;
            unsigned int S = offset;
            auto N = bits_in_uint32_t;
            auto M = mask;
            typedef utils::bits<uint32_t> bits;

            for (unsigned int _ = 0; _ < N; ++_)
            {
                unsigned int i = N - _ - 1;

                if (i > N - S)
                {
                    X = bits::change(X, i, bits::get(Y, i));
                }
                else
                {
                    X = bits::change(X, i, bits::get(Y, i) ^ (bits::get(X, i + S) & bits::get(M, i)));
                }
            }

            return X;
        }
    };
}

#endif //CRYPTOPALS_BREAKS_PRNG_H
