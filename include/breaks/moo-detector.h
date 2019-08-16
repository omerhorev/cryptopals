//
// Created by omerh on 14/08/2019.
//

#ifndef CRYPTOPALS_MOO_DETECTOR_H
#define CRYPTOPALS_MOO_DETECTOR_H

#include <cstddef>

namespace breaks
{
    class moo_detector
    {
    public:

        /**
         * Returns whether the encrypted data is in ECB mode given we know only the ciphertext.
         * Will work only if the plaintext has the same blocks in it.
         *
         * @param cipher The ciphertext
         * @param length The length of the ciphertext
         * @param block_size The length of the block
         * @return (bool) True if is in ECB mode, False if not certain (could be, not definitive answer).
         */
        static bool is_ecb(const unsigned char cipher[], size_t length, size_t block_size)
        {
            if(length % 16 != 0) return false;

            auto blocks_count = length / 16;

            // First, lets search for blocks that are equal.
            for (auto i = 0; i < blocks_count; ++i)
            {
                for (auto j = 0; j < blocks_count; ++j)
                {
                    if (i == j) continue;

                    auto a = i * 16;
                    auto b = j * 16;

                    if (memcmp(cipher + a, cipher + b, block_size) == 0)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

    };
}

#endif //CRYPTOPALS_MOO_DETECTOR_H
