//
// Created by omerh on 01/08/2019.
//

#ifndef CRYPTOPALS_AES_UTILS_H
#define CRYPTOPALS_AES_UTILS_H

#include <cstddef>
#include "aes-round-keys.h"

namespace model
{
    namespace internal
    {
        class aes_utils
        {
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


        private:

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
             * @param data    4 byte array
             */
            static void circular_rotate(unsigned char data[]);

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
