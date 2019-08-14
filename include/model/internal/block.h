//
// Created by omerh on 13/08/2019.
//

#ifndef CRYPTOPALS_BLOCK_INTERNAL_H
#define CRYPTOPALS_BLOCK_INTERNAL_H

#include <model/internal/moo/ecb.h>

namespace model
{
    namespace internal
    {
        template<typename Model>
        class default_ecb_block_cipher
        {
        public:
            template<typename ...Args>
            default_ecb_block_cipher(Args... args) : _model(args...), _moo(_model)
            {}

            /**
             * Initializes the model and the moo
             *
             * @param key The key to initialize
             */
            void initialize(const unsigned char (&key)[Model::key_size])
            {
                _model.set_key(key);
            }

            /**
             * Encrypts some data, The data must be a multiplicand of the Model::block_size
             *
             * @param data   The data to encrypt
             * @param length The length of the data to encrypt
             */
            void encrypt(unsigned char data[], size_t length)
            {
                _moo.encrypt(data, length);
            }

            /**
             * Decrypts some data, The data must be a multiplicand of the Model::block_size
             *
             * @param data   The data to decrypt
             * @param length The length of the data to decrypt
             */
            void decrypt(unsigned char data[], size_t length)
            {
                _moo.decrypt(data, length);
            }

        private:
            moo::ecb<Model> _moo;
            Model _model;
        };
    }
}

#endif //CRYPTOPALS_BLOCK_INTERNAL_H
