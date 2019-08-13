//
// Created by omerh on 01/08/2019.
//

#ifndef CRYPTOPALS_AES_UTILS_H
#define CRYPTOPALS_AES_UTILS_H

#include <cstddef>
#include <utils/debug.h>
#include "utils/general.h"
#include "math/galois-field.h"
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
                math::galois256 gmatrix[state_rows_count][state_cols_count];
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
                generate_key_schedule(key, Flavor::key_size, (unsigned char *) o_round_keys.keys,
                                      sizeof(o_round_keys.keys));
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
            static void encrypt(unsigned char block[], size_t length, const aes_round_keys<Flavor> &round_keys)
            {
                auto &s = *((state *) block);

                add_round_key(s, round_keys.keys[0]);

                for (auto i = 0; i < Flavor::rounds_count - 1; i++)
                {
                    sub_bytes(s);
                    shift_rows(s);
                    mix_columns(s);
                    add_round_key(s, round_keys.keys[i + 1]);
                }

                sub_bytes(s);
                shift_rows(s);
                add_round_key(s, round_keys.keys[Flavor::rounds_count]);
            }

            /**
             * Decrypts a block of data using aes
             * @tparam Flavor    the AES flavor to use
             * @param block      The block to decrypt
             * @param length     The length of the block to decrypt
             * @param round_keys The round keys to use
             */
            template<typename Flavor>
            static void decrypt(unsigned char block[], size_t length, const aes_round_keys<Flavor> &round_keys)
            {
                auto &s = *((state *) block);

                add_round_key(s, round_keys.keys[Flavor::rounds_count]);
                shift_rows_inv(s);
                sub_bytes_inv(s);

                for (auto i = Flavor::rounds_count - 1; i > 0; i--)
                {
                    add_round_key(s, round_keys.keys[i]);
                    mix_columns_inv(s);
                    shift_rows_inv(s);
                    sub_bytes_inv(s);
                }

                add_round_key(s, round_keys.keys[0]);
            }

        private:

            static void add_round_key(state &s, const unsigned char (&round_key)[aes_utils::block_size])
            {
                utils::general::xor_buffers(s.raw, round_key, s.raw);
            }

            static void shift_rows(state &s);

            static void shift_rows_inv(state &state);

            static void sub_bytes(state &s);

            static void sub_bytes_inv(state &state);

            static void mix_columns(state &s);

            static void mix_columns_inv(state &state);

            static void generate_key_schedule(const unsigned char *root_key,
                                              size_t root_key_length,
                                              unsigned char *expanded_key,
                                              size_t expanded_key_length);

            static void circular_rotate_left(unsigned char *buffer, size_t length, unsigned int offset);

            static const unsigned char sbox_table[256];
            static const unsigned char sbox_inv_table[256];
        };
    }
}

#endif //CRYPTOPALS_AES_UTILS_H
