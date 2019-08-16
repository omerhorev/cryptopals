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
        namespace aes
        {
            template<typename Flavor>
            class aes_model
            {
            public:

                static constexpr size_t block_size = aes_utils::block_size;
                static constexpr size_t key_size = Flavor::key_size;

                aes_model() : _round_keys({0})
                {}

                /**
                 * Sets the key for the aes encryption
                 *
                 * @param key    The key yo set
                 * @param length The length of the key, must be Flavor::key_size
                 */
                void set_key(const unsigned char key[], size_t length)
                {
                    if (length != key_size)
                    {
                        return;
                    }

                    aes_utils::generate_round_keys(key, _round_keys);
                }

                /**
                 * Encrypts a block of data
                 *
                 * @param block  The data to encrypt
                 * @param length The length of the data
                 */
                void encrypt(unsigned char block[], size_t length)
                {
                    if (length != block_size)
                    {
                        return;
                    }

                    aes_utils::encrypt(block, aes_utils::block_size, _round_keys);
                }

                /**
                 * Encrypts a block of data
                 *
                 * @param block The block to encrypt
                 */
                void encrypt(unsigned char (&block)[block_size])
                {
                    aes_utils::encrypt(block, aes_utils::block_size, _round_keys);
                }

                /**
                 * Encrypts a block of data
                 *
                 * @param block  The data to encrypt
                 * @param length The length of the data
                 */
                void decrypt(unsigned char block[], size_t length)
                {
                    if (length != block_size)
                    {
                        return;
                    }

                    aes_utils::decrypt(block, aes_utils::block_size, _round_keys);
                }

                /**
                 * Encrypts a block of data
                 *
                 * @param block The block to encrypt
                 */
                void decrypt(unsigned char (&block)[block_size])
                {
                    aes_utils::decrypt(block, aes_utils::block_size, _round_keys);
                }

            private:

                aes_round_keys<Flavor> _round_keys;
            };
        }
    }
}

#endif //CRYPTOPALS_AES_H
