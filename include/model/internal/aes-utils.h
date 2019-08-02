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

            template<typename Flavor>
            static void generate_round_keys(unsigned char key[Flavor::key_size], aes_round_keys<Flavor> &keys)
            {
                expand_key(key, Flavor::key_size, (unsigned char *) keys.round_keys, sizeof(keys.round_keys));
            }


        private:
            static void expand_key(unsigned char root[],
                                   size_t root_length,
                                   unsigned char expanded_key[],
                                   size_t expanded_key_length);

            /**
             * Rotates a 4 byte array (32 bit) one byte (8 bit) to the left
             * @param data    4 byte array
             */
            static void circular_rotate(unsigned char data[]);

            /**
             * The core operation of the generation. This will be called according to the Rijndael's key generation
             * scheme.
             *
             * @param input  4 byte buffer
             * @param output (output) 4 byte buffer
             * @param index  The index of the operation
             */
            static void expantion_core(unsigned char data[], unsigned int index);

            static unsigned char sbox(unsigned char v);

            static const unsigned char sbox_table[256];
        };
    }
}

#endif //CRYPTOPALS_AES_UTILS_H
