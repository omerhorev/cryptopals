//
// Created by omerh on 16/08/2019.
//

#ifndef CRYPTOPALS_CBC_H
#define CRYPTOPALS_CBC_H

#include <cstddef>
#include <algorithm>
#include <utils/general.h>
#include <utils/debug.h>

namespace model
{
    namespace internal
    {
        namespace moo
        {
            template<typename Model>
            class cbc
            {
                static constexpr size_t block_size = Model::block_size;

                Model &_model;
                unsigned char _state[block_size];

            public:
                explicit cbc(Model &model) : _model(model)
                {}

                void initialize(const unsigned char iv[], size_t length)
                {
                    if (length != block_size)
                    {
                        return;
                    }

                    for (int i = 0; i < block_size; ++i)
                    {
                        _state[i] = iv[i];
                    }

                    utils::print_buffer("iv", iv, length);
                }

                void encrypt(unsigned char *data, size_t length)
                {
                    if (length % block_size != 0) return;

                    utils::general::xor_buffers(data, _state, data, block_size);
                    _model.encrypt(data, block_size);

                    for (int i = 16; i < length; i += 16)
                    {
                        utils::general::xor_buffers(data + i - block_size, data + i, data + i, block_size);
                        _model.encrypt(data + i, block_size);
                    }

                    std::copy(data + length - block_size, data + length, _state);
                }

                void decrypt(unsigned char *data, size_t length)
                {
                    if (length % 16 != 0) return;

                    unsigned char temp_state[16];

                    for (int i = 0; i < length; i += 16)
                    {
                        std::copy(data + i, data + i + 16, temp_state);
                        _model.decrypt(data + i, block_size);
                        utils::general::xor_buffers(data + i, _state, data + i, block_size);
                        std::copy(temp_state, temp_state + 16, _state);
                    }
                }

            };
        }
    }
}

#endif //CRYPTOPALS_CBC_H
