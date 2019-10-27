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
        template<typename Model, moo::ctr_format Format, model::internal::endianness Endianness>
        class default_ctr_stream_cipher_base
        {
        public:

            static const size_t block_size = Model::block_size;

            typedef typename moo::ctr<Model, Model::block_size, Format, Endianness>::counter_t counter_t;

            template<class ...Args>
            explicit default_ctr_stream_cipher_base(Args... args)
                    : _model(args...),
                      _index(block_size),
                      _ctr(_model),
                      _counter(0),
                      _nounce(0)
            {}

            void initialize(const unsigned char key[], size_t key_length, counter_t nounce, counter_t counter)
            {
                _model.set_key(key, key_length);
                _counter = counter;
                _nounce = nounce;
                _index = block_size;
                std::fill(_block_mask, _block_mask + block_size, 0);
            }

            void encrypt(unsigned char buffer[], size_t length)
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
                    // Feed some more data
                    _ctr.get_block_mask(_nounce, _counter++, _block_mask);
                    _index = 0;
                }

                return _block_mask[_index++];
            }

        private:

            counter_t _nounce;
            counter_t _counter;
            unsigned int _index;
            unsigned char _block_mask[block_size];

            Model _model;
            moo::ctr<Model, block_size, Format, Endianness> _ctr;
        };

        template<typename Model, moo::ctr_format Format, model::internal::endianness Endianness>
        class default_ctr_stream_cipher
        {
        private:
            typedef default_ctr_stream_cipher_base<Model, Format, Endianness> base_stream_cipher;
            typedef typename default_ctr_stream_cipher_base<Model, Format, Endianness>::counter_t counter_t;

        public:

            template<class ...Args>
            explicit default_ctr_stream_cipher(Args... args) : _enc_cipher(args...), _dec_cipher(args...)
            {}

            void
            initialize_encryption(const unsigned char key[], size_t key_length, counter_t nounce, counter_t counter)
            {
                _enc_cipher.initialize(key, key_length, nounce, counter);
            }

            void
            initialize_decryption(const unsigned char key[], size_t key_length, counter_t nounce, counter_t counter)
            {
                _dec_cipher.initialize(key, key_length, nounce, counter);
            }

            void encrypt(unsigned char buffer[], size_t length)
            {
                _enc_cipher.encrypt(buffer, length);
            }

            void decrypt(unsigned char buffer[], size_t length)
            {
                _dec_cipher.encrypt(buffer, length);
            }

        private:
            base_stream_cipher _enc_cipher;
            base_stream_cipher _dec_cipher;
        };
    }
}


#endif //CRYPTOPALS_INTERNAL_STREAM_H
