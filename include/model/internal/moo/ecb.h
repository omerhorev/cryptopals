//
// Created by omerh on 13/08/2019.
//

#ifndef CRYPTOPALS_ECB_H
#define CRYPTOPALS_ECB_H

#include <cstddef>

namespace model
{
    namespace internal
    {
        namespace moo
        {
            template<typename Model>
            class ecb
            {
                static constexpr size_t block_size = Model::block_size;

                Model &_model;
            public:
                explicit ecb(Model &model) : _model(model)
                {}

                void encrypt(unsigned char *data, size_t length)
                {
                    if (length % 16 != 0) return;

                    for (int i = 0; i < length; i += 16)
                    {
                        _model.encrypt(*(unsigned char(*)[block_size])(data + i));
                    }
                }

                void decrypt(unsigned char *data, size_t length)
                {
                    if (length % 16 != 0) return;

                    for (int i = 0; i < length; i += 16)
                    {
                        _model.decrypt(*(unsigned char(*)[block_size])(data + i));
                    }
                }

            };
        }
    }
}

#endif //CRYPTOPALS_ECB_H
