//
// Created by omerh on 01/08/2019.
//

#ifndef CRYPTOPALS_AES_UTILS_H
#define CRYPTOPALS_AES_UTILS_H

#include <cstddef>
#include "utils/general.h"
#include "galois-field.h"
#include "aes-round-keys.h"

namespace model
{
    namespace internal
    {
        class aes_utils
        {
        public:

            static const size_t block_size = 16;
            static const size_t state_rows_count = 4;
            static const size_t state_cols_count = 4;

        private:
            union state
            {
                unsigned char raw[block_size];
                unsigned char matrix[state_rows_count][state_cols_count];
            };
        public:

            /**\
             * Generates set of round keys according to the aes flavor specified
             * @tparam Flavor The AES flavor (can be aes_flavor_aes128/aes_flavor_aes192/aes_flavor_aes256)
             * @param key   The root key to expand into round keys
             * @param o_round_keys  (output) The round keys
             */
            template<typename Flavor>
            static void generate_round_keys(const unsigned char *key, aes_round_keys<Flavor> &o_round_keys)
            {
                generate_key_schedule(key, Flavor::key_size, (unsigned char *) o_round_keys.round_keys,
                                      sizeof(o_round_keys.round_keys));
            }

            /**
             * Encrypts a block of data using aes
             *
             * @tparam Flavor    The AES flavor to use
             * @param block      The block to encrypt
             * @param length     The length of the block to encrypt (must be 16)
             * @param round_keys The round keys to use
             */
            template<typename Flavor>
            void encrypt(const unsigned char block[], size_t length, aes_round_keys<Flavor> round_keys)
            {
                state s = {0};

                add_round_key(s, round_keys[0]);

                for (auto i = 0; i < Flavor::rounds_count - 1; i++)
                {
                    sub_bytes(s);
                    shift_rows(s);
                    // mix_culumns();
                    add_round_key(s, round_keys[i + 1]);
                }

                sub_bytes(s);
                shift_rows(s);
                add_round_key(s, round_keys[Flavor::rounds_count - 1]);
            }

        public:

            void add_round_key(state &s, const unsigned char (&round_key)[aes_utils::block_size])
            {
                utils::general::xor_buffers(s.raw, round_key, s.raw);
            }

            void sub_bytes(state &s)
            {
                for (auto &c : s.raw) c = sbox(c);
            }

            void shift_rows(state &s)
            {
                for (unsigned int r = 0; r < state_rows_count; ++r)
                {
                    auto &row = s.matrix[r];

                    circular_rotate_left(row, 0, r);
                }
            }

            void mix_columns(state &s)
            {
                for (int c = 0; c < state_cols_count; c++)
                {
                    unsigned char temp_col[state_cols_count];

                    /*temp_col[0] = gmul(a[0], 2) ^ gmul(a[3], 1) ^ gmul(a[2], 1) ^ gmul(a[1], 3);
                    temp_col[1] = gmul(a[1], 2) ^ gmul(a[0], 1) ^ gmul(a[3], 1) ^ gmul(a[2], 3);
                    temp_col[2] = gmul(a[2], 2) ^ gmul(a[1], 1) ^ gmul(a[0], 1) ^ gmul(a[3], 3);
                    temp_col[3] = gmul(a[3], 2) ^ gmul(a[2], 1) ^ gmul(a[1], 1) ^ gmul(a[0], 3);*/
                }
            }

            /**
             * Generates a key schedule
             *
             * @param root_key                The root key
             * @param root_key_length         The root key length (can be 16/24/32)
             * @param expanded_key        (output) The expanded key (can be 176/208/240)
             * @param expanded_key_length The length of the expanded key buffer
             */
            static void generate_key_schedule(const unsigned char *root_key,
                                              size_t root_key_length,
                                              unsigned char *expanded_key,
                                              size_t expanded_key_length);

            /**
             * Rotates a 4 byte array (32 bit) one byte (8 bit) to the left
             * @param buffer    4 byte array
             */
            static void circular_rotate_left(unsigned char *buffer, size_t length, unsigned int offset);

            /**
             * Performs an sbox operation in a given char using the sbox table
             * @param v  The char to perform sbox on
             * @return   (unsigned char) The result
             */
            static unsigned char sbox(unsigned char v);

            static const unsigned char sbox_table[256];
        };
    }
}

#endif //CRYPTOPALS_AES_UTILS_H
