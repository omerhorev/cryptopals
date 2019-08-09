//
// Created by omerh on 03/08/2019.
//

#ifndef CRYPTOPALS_AES_H
#define CRYPTOPALS_AES_H

#include <cstddef>
#include "aes-round-keys.h"
#include "aes-utils.h"

namespace model
{
    namespace internal
    {
        template<typename Flavor>
        class aes
        {
        public:

            aes() : _round_keys(0)
            {}

            /**
             * Sets the key for the aes encryption
             *
             * @param key The key yo set
             */
            void set_key(const unsigned char (&key)[Flavor::key_size])
            {
                set_key(&key, sizeof(key));
            }

            /**
             * Sets the key for the aes encryption
             *
             * @param key    The key yo set
             * @param length The length of the key to set
             */
            void set_key(const unsigned char key[], size_t length)
            {
                aes_utils::generate_round_keys(key, length, _round_keys);
            }

            /**
             * Encrypts a block of data
             *
             * @param block The block to encrypt
             */
            void encrypt(const unsigned char (&block)[aes_utils::block_size])
            {
                encrypt(block, aes_utils::block_size);
            }

        private:

            aes_round_keys<Flavor> _round_keys;
        };
    }
}

#endif //CRYPTOPALS_AES_H
