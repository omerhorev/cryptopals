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
         * @tparam N     The length of the buffers
         * @param cipher The ciphertext
         * @return (bool) True if is in ECB mode, False if not certain (could be, not definitive answer).
         */
        template<size_t N>
        static bool is_ecb(const unsigned char (&cipher)[N])
        {
            static_assert(N % 16 == 0, "Must be multiplicand of block size (16)");

            auto blocks_count = N / 16;

            // First, lets search for blocks that are equal.
            for (auto i = 0; i < blocks_count; ++i)
            {
                for (auto j = 0; j < blocks_count; ++j)
                {
                    if (i == j) continue;

                    auto a = i * 16;
                    auto b = j * 16;

                    if (memcmp(cipher + a, cipher + b, 16) == 0)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        /**
         * Returns whether the encrypted data is in ECB mode given we know the plain but not the key
         *
         * @tparam N          The length of the buffers
         * @param known_plain The known plaintext
         * @param cipher      The cipher
         * @return (bool) True if is in ECB mode, False if not certain (could be, not definitive answer).
         */
        template<size_t N>
        static bool is_ecb(const unsigned char (&known_plain)[N], const unsigned char (&cipher)[N])
        {
            static_assert(N % 16 == 0, "Must be multiplicand of block size (16)");

            auto blocks_count = N / 16;

            int equal_blocks_found_offset[2];
            bool found_same_blocks = false;

            // First, lets search for blocks that are equal.
            for (auto i = 0; i < blocks_count; ++i)
            {
                for (auto j = 0; j < blocks_count; ++j)
                {
                    if (i == j) continue;

                    auto a = i * 16;
                    auto b = j * 16;

                    if (memcmp(known_plain + a, known_plain + b, 16) == 0)
                    {
                        equal_blocks_found_offset[0] = a;
                        equal_blocks_found_offset[1] = b;
                        found_same_blocks = true;
                    }
                }
            }

            if (!found_same_blocks)
            {
                return false;
            }

            // We found two matching blocks, lets check if they were encrypted in the same way

            return memcmp(cipher + equal_blocks_found_offset[0], cipher + equal_blocks_found_offset[1], 16) == 0;
        }
    };
}

#endif //CRYPTOPALS_MOO_DETECTOR_H
