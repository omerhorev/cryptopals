//
// Created by omerh on 27/10/2019.
//

#ifndef CRYPTOPALS_INTERNAL_STREAM_H
#define CRYPTOPALS_INTERNAL_STREAM_H

#include <utils/general.h>
#include <model/internal/utils.h>
#include <model/internal/moo/ctr.h>

namespace model
{
    namespace internal
    {
        /**
         * Represents a base class for all types of ctr moo's. Provide a stream wrapping to convert the CTR block
         * encryption to a stream api using counters.
         *
         * @tparam Model      The type of the model to use
         * @tparam Format     The format of the CTR to use
         * @tparam Endianness The endianess to encode with (for the nonce and the counter)
         */
        template<typename Model, moo::ctr_format Format, model::internal::endianness Endianness>
        class default_ctr_stream_cipher_base
        {
        public:

            static const size_t block_size = Model::block_size;

            typedef typename moo::ctr<Model, Model::block_size, Format, Endianness>::counter_t counter_t;

            /**
             * Initializes a new instance of the CTR base class.
             *
             * @tparam Args The types of the argument to use to initialize the model
             * @param args The arguments to use to initialize the model
             */
            template<class ...Args>
            explicit default_ctr_stream_cipher_base(Args... args)
                    : _model(args...),
                      _index(block_size),
                      _ctr(_model),
                      _counter(0),
                      _nonce(0)
            {}

            /**
             * Initializes the base class with specific counter and nonce that will be used. The counter will advance
             * as more and more blocks-sized buffers will be encrypted.
             *
             * @param key        The key to initialize the model with
             * @param key_length The length of the key
             * @param nonce      The nonce to use
             * @param counter    The counter initialize with (default is 0)
             */
            void initialize(const unsigned char key[], size_t key_length, counter_t nonce, counter_t counter = 0)
            {
                _model.set_key(key, key_length);
                _counter = counter;
                _nonce = nonce;
                _index = block_size;
                std::fill(_block_mask, _block_mask + block_size, 0);
            }

            /**
             * Performs the action (encryption/decryption - the same in CTR) over the buffer provided
             *
             * @param buffer The buffer to perform upon
             * @param length The length of the buffer provided
             */
            void perform(unsigned char *buffer, size_t length)
            {
                for (int i = 0; i < length; ++i)
                {
                    buffer[i] ^= get_byte();
                }
            }

        private:
            unsigned char get_byte()
            {
                if (_index == block_size)
                {
                    //
                    // No more usable data in the mask, lets generate the mask for the next block-sized data
                    //
                    _ctr.get_block_mask(_nonce, _counter++, _block_mask);
                    _index = 0;
                }

                return _block_mask[_index++];
            }

        private:

            counter_t _nonce;
            counter_t _counter;
            unsigned int _index;
            unsigned char _block_mask[block_size];

            Model _model;
            moo::ctr<Model, block_size, Format, Endianness> _ctr;
        };

        /**
         * The default class for CTR what will work with any block cipher and provide both encryption and decryption
         * capabilities. This is the final class that wrap all the actions and consists of two base classes for
         * encryption and decryption.
         *
         * @tparam Model      The type of the model to use
         * @tparam Format     The format of the CTR to use
         * @tparam Endianness The endianess to encode with (for the nonce and the counter)
         */
        template<typename Model, moo::ctr_format Format, model::internal::endianness Endianness>
        class default_ctr_stream_cipher
        {
        private:
            typedef default_ctr_stream_cipher_base<Model, Format, Endianness> base_stream_cipher;

        public:
            typedef typename default_ctr_stream_cipher_base<Model, Format, Endianness>::counter_t counter_t;

        public:

            /**
             * Initializes a new instance of the CTR class.
             *
             * @tparam Args The types of the argument to use to initialize the model
             * @param args The arguments to use to initialize the model
             */
            template<class ...Args>
            explicit default_ctr_stream_cipher(Args... args) : _enc_cipher(args...), _dec_cipher(args...)
            {}

            /**
             * Initializes the encryption system with specific counter and nonce that will be used.
             * The counter will advance as more and more blocks-sized buffers will be encrypted.
             *
             * The encryption system and the decryption systems can be used simultaneously and can be initialized with
             * different keys and params.
             *
             * @param key        The key to initialize the model with
             * @param key_length The length of the key
             * @param nonce      The nonce to use
             * @param counter    The counter initialize with (default is 0)
             */
            void
            initialize_encryption(const unsigned char key[], size_t key_length, counter_t nonce, counter_t counter)
            {
                _enc_cipher.initialize(key, key_length, nonce, counter);
            }

            /**
             * Initializes the decryption system with specific counter and nonce that will be used.
             * The counter will advance as more and more blocks-sized buffers will be encrypted.
             *
             * The encryption system and the decryption systems can be used simultaneously and can be initialized with
             * different keys and params.
             *
             * @param key        The key to initialize the model with
             * @param key_length The length of the key
             * @param nonce      The nonce to use
             * @param counter    The counter initialize with (default is 0)
             */
            void
            initialize_decryption(const unsigned char key[], size_t key_length, counter_t nonce, counter_t counter)
            {
                _dec_cipher.initialize(key, key_length, nonce, counter);
            }

            /**
             * Encrypts a buffer with the key and params initialized in the encryption system.
             *
             * @param buffer The buffer to encrypt.
             * @param length The length of the buffer to encrypt
             */
            void encrypt(unsigned char buffer[], size_t length)
            {
                _enc_cipher.perform(buffer, length);
            }

            /**
             * Decrypts a buffer with the key and params initialized in the decryption system.
             *
             * @param buffer The buffer to encrypt.
             * @param length The length of the buffer to encrypt
             */
            void decrypt(unsigned char buffer[], size_t length)
            {
                _dec_cipher.perform(buffer, length);
            }

        private:
            base_stream_cipher _enc_cipher;
            base_stream_cipher _dec_cipher;
        };
    }
}


#endif //CRYPTOPALS_INTERNAL_STREAM_H
