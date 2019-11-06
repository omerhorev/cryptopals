//
// Created by omerh on 22/07/2019.
//

#ifndef CRYPTOPALS_XOR_BREAK_H
#define CRYPTOPALS_XOR_BREAK_H

#include <cstddef>
#include <algorithm>

namespace breaks
{
    class xor_break
    {
    public:
        /**
         * Breaks the single xor cipher given the cipher is in english alphabet
         *
         * @param cipher        The cipher data
         * @param cipher_length The length of the encrypted data
         * @param o_key         The key extracted
         */
        void single_xor_cipher_break(const unsigned char cipher[],
                                     size_t cipher_length,
                                     unsigned char &o_key);

        /**
         * Breaks the single xor cipher given the cipher is in english alphabet
         *
         * @param cipher        The cipher data
         * @param cipher_length The length of the encrypted data
         * @param o_key         The key extracted
         * @param o_accuracy    The accuracy of the decryption. The higher the number the more certain we are that we
         *                      got the actual key.
         */
        void single_xor_cipher_break(const unsigned char cipher[],
                                     size_t cipher_length,
                                     unsigned char &o_key,
                                     float &o_accuracy);

        /**
         * Break a buffer with repeating xor key.
         *
         * @param cipher The encrypted buffer
         * @param length The length of the encrypted buffer
         * @param key_length The length of the encrypted keky
         * @param o_key The repeated key generated
         * @return The accuracy of the solution, near 0 if not encrypted in the repeating key xor algorithm
         */
        static float cyclic_xor_encryption_by_key_length(const unsigned char cipher[], size_t length, size_t key_length,
                                                  unsigned char o_key[])
        {
            const size_t number_of_split_buffers = key_length;
            const size_t split_buffer_max_size = (length / number_of_split_buffers) + 1;

            size_t split_buffer_write_index[number_of_split_buffers];
            unsigned char split_buffer[number_of_split_buffers][split_buffer_max_size];

            std::fill_n((unsigned char *) split_buffer_write_index, sizeof(split_buffer_write_index), 0);
            std::fill_n((unsigned char *) split_buffer, sizeof(split_buffer), 0);

            for (auto i = 0; i < length; ++i)
            {
                auto k = (i % number_of_split_buffers);

                split_buffer[k][split_buffer_write_index[k]] = cipher[i];
                split_buffer_write_index[k]++;
            }

            breaks::xor_break b;

            float sum_score = 0;

            for (int j = 0; j < number_of_split_buffers; ++j)
            {
                unsigned char key_char;
                float score;

                b.single_xor_cipher_break(split_buffer[j], split_buffer_write_index[j], key_char, score);

                o_key[j] = key_char;

                sum_score += score;
            }

            return (sum_score / number_of_split_buffers);
        }

    };
}

#endif //CRYPTOPALS_XOR_BREAK_H
