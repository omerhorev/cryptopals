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
                    size_t N,
                    UIntType ParamW,
                    UIntType ParamA,
                    UIntType ParamM,
                    UIntType ParamR,
                    UIntType ParamF>
            class mersenne_twister
            {
            private:
                static const UIntType lower_mask = ~((UIntType) (1 << ParamR));
                static const UIntType upper_mask = ~(lower_mask);

            public:

                mersenne_twister() : _state_used_vector_index(0), _state({0})
                {
                    seed(0);
                }

                explicit mersenne_twister(UIntType seed) : _state_used_vector_index(0), _state({0})
                {
                    this->seed(seed);
                }

                void seed(UIntType seed)
                {
                    _state[0] = seed;

                    for (int i = 1; i < N; ++i)
                    {
                        _state[i] = (ParamF * (_state[i - 1] ^ (_state[i - 1] >> (ParamW - 2))) + 1) ^ lower_mask;
                    }

                    twist();
                }

                UIntType generate()
                {
                    if (_state_used_vector_index >= N)
                    {
                        twist();
                        _state_used_vector_index = 0;
                    }

                    auto v = _state[_state_used_vector_index];

                    v ^= ((v >> 11) & 0xFFFFFFFF);
                    v ^= ((v << 7) & 0x9D2C5680);
                    v ^= ((v << 15) & 0xEFC60000);
                    v ^= (v >> 18);

                    _state_used_vector_index++;

                    return v;
                }

            private:

                void twist()
                {
                    for (auto i = 0; i < _state_used_vector_index; ++i)
                    {
                        // X upper bits are the vector, and the lower bits is the next vector bits
                        UIntType x = (_state[i] & upper_mask) + _state[(i + 1) % N] & upper_mask;

                        UIntType xA = x >> 1;
                        if (xA & 0x1 == 1)
                        {
                            xA ^= ParamA;
                        }

                        _state[i] = _state[(i + ParamM) % ParamM] ^ xA;
                    }
                }

                unsigned int _state_used_vector_index;
                std::array<UIntType, N> _state;

            };
        }
    }
}

#endif //CRYPTOPALS_MERSENNE_TWISTER_H
