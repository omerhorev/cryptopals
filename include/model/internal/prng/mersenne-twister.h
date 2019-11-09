//
// Created by omerh on 06/11/2019.
//

#ifndef CRYPTOPALS_MERSENNE_TWISTER_H
#define CRYPTOPALS_MERSENNE_TWISTER_H

#include <cstdint>
#include <array>

namespace model
{
    namespace internal
    {
        namespace prng
        {
            /**
             * Represents a Mersenne Twister algorith for generating pseudo-random numbers
             */
            template<typename UIntType,
                    UIntType ParamN,
                    UIntType ParamW,
                    UIntType ParamA,
                    UIntType ParamM,
                    UIntType ParamR,
                    UIntType ParamF,
                    UIntType ParamU, UIntType ParamD,
                    UIntType ParamS, UIntType ParamB,
                    UIntType ParamT, UIntType ParamC,
                    UIntType ParamL>

            class mersenne_twister
            {
            private:
                static const UIntType lower_mask = ~((UIntType) (1 << ParamR));
                static const UIntType upper_mask = ~(lower_mask);
                static const UIntType default_seed = 88726;

            public:

                /**
                 * Initializes the mersenne-twister with a provided seed
                 * @param seed The seed to initialize with, the default value is 88726
                 */
                explicit mersenne_twister(UIntType seed = default_seed) : _state_used_vector_index(ParamN), _state({0})
                {
                    this->seed(seed);
                }

                /**
                 * Reseed the mersenne-twister with a different seed and reset the internal state
                 *
                 * @param seed The seed to initialize with
                 */
                void seed(UIntType seed)
                {
                    _state[0] = seed;

                    for (int i = 1; i < ParamN; ++i)
                    {
                        //_state[i] = (ParamF * (_state[i - 1] ^ (_state[i - 1] >> (ParamW - 2))) + 1) ^ lower_mask;
                        UIntType x = _state[i - 1];
                        x ^= x >> (ParamW - 2);
                        x *= ParamF;
                        x += i;

                        const size_t excess_bits = (sizeof(UIntType) * 8) - ParamW;
                        _state[i] = x & (-1 >> excess_bits);
                    }

                    _state_used_vector_index = ParamN;
                }

                /**
                 * Generates a pseudo-random number.
                 * @return The number generated
                 */
                UIntType generate()
                {
                    if (_state_used_vector_index >= ParamN)
                    {
                        twist();
                        _state_used_vector_index = 0;
                    }

                    auto v = _state[_state_used_vector_index];

                    v ^= ((v >> ParamU) & ParamD);
                    v ^= ((v << ParamS) & ParamB);
                    v ^= ((v << ParamT) & ParamC);
                    v ^= (v >> ParamL);

                    _state_used_vector_index++;

                    return v;
                }

            private:

                void twist()
                {
                    for (auto i = 0; i < ParamN; ++i)
                    {
                        // X upper bits are the vector, and the lower bits is the next vector bits
                        UIntType x = (_state[i] & upper_mask) | _state[(i + 1) % ParamN] & lower_mask;

                        UIntType xA = x >> 1;
                        if (x & 1)
                        {
                            xA ^= ParamA;
                        }

                        _state[i] = _state[(i + ParamM) % ParamN] ^ xA;
                    }
                }

                unsigned int _state_used_vector_index;
                std::array<UIntType, ParamN> _state;

            };
        }
    }
}

#endif //CRYPTOPALS_MERSENNE_TWISTER_H
