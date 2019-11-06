//
// Created by omerh on 06/11/2019.
//

#ifndef CRYPTOPALS_MT19937_H
#define CRYPTOPALS_MT19937_H

#include <cstdint>
#include <array>

namespace model
{
    namespace internal
    {
        namespace random
        {
            /**
             * Represents a Mersenne Twister algorith for generating pseudo-random numbers
             */
            class mt19937
            {
            private:
                const static size_t state_vectors_count = 624;

            public:

                /**
                 * Initializes thr random numbers generator with a default seed.
                 */
                mt19937() : _state_used_vector_index(0), _state({0})
                {
                    unsigned char _[4] = {0};
                    seed(_, sizeof(_));
                }

                /**
                 * Initialize the random numbers generated with a given seed
                 * @param seed   A 4 bytes (32-bit) random seed to initialize with
                 * @param length The length of the seed (must be 4 bytes)
                 */
                mt19937(unsigned char seed[], size_t length) : _state_used_vector_index(0), _state({0})
                {
                    this->seed(seed, length);
                }


                /**
                 * Seeds the Mersenne-Twister.
                 *
                 * @param seed   A 4 bytes (32-bit) random seed
                 * @param length The length of the seed (must be 4 bytes)
                 */
                void seed(const unsigned char seed[], size_t length);

                /**
                 * Generates 4 bytes (32-bit) of random data from the twister
                 *
                 * @param o_data      (output) The buffer to place the random data in
                 * @param data_length The length of the buffer provided (must be at least 4 bytes)
                 */
                void generate(unsigned char o_data[], size_t data_length);

            private:

                void twist();

                unsigned int _state_used_vector_index;
                std::array<std::array<unsigned char, 4>, state_vectors_count> _state;

            };
        }
    }
}

#endif //CRYPTOPALS_MT19937_H
