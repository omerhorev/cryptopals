//
// Created by omerh on 22/10/2019.
//

#ifndef CRYPTOPALS_CTR_H
#define CRYPTOPALS_CTR_H


#include <cstdint>
#include <algorithm>
#include <array>
#include <model/internal/utils.h>
#include <utils/debug.h>

namespace model
{
    namespace internal
    {
        namespace moo
        {
            enum class ctr_format
            {
                nounce_first,
                counter_first
            };

            template<typename Model, unsigned int BlockSize, ctr_format Format, model::internal::endianness Endianness>
            class ctr;

            template<typename Model, ctr_format Format, model::internal::endianness Endianness>
            class ctr<Model, 16, Format, Endianness>
            {
                static_assert(Model::block_size == 16, "The size of the model must be 16");
                static const size_t block_size = Model::block_size;

            public:
                typedef uint64_t counter_t;


                ctr(Model &model) : _model(model)
                {}

            public:

                void get_block_mask(counter_t nounce, counter_t counter, unsigned char mask[block_size])
                {
                    if (Format == ctr_format::nounce_first)
                    {
                        model::internal::encoder<counter_t, Endianness>::encode(nounce,
                                                                               *((unsigned char (*)[sizeof(counter_t)]) &mask[0]));
                        model::internal::encoder<counter_t, Endianness>::encode(counter,
                                                                                *((unsigned char (*)[sizeof(counter_t)]) &mask[8]));
                    }
                    else if (Format == ctr_format::counter_first)
                    {
                        model::internal::encoder<counter_t, Endianness>::encode(counter,
                                                                                *((unsigned char (*)[sizeof(counter_t)]) &mask[0]));
                        model::internal::encoder<counter_t, Endianness>::encode(nounce,
                                                                                *((unsigned char (*)[sizeof(counter_t)]) &mask[8]));
                    }

                    utils::print_blocked_buffer(mask, block_size, block_size);
                    _model.encrypt(mask, block_size);
                }

            private:

                Model &_model;
            };
        }
    }
}

#endif //CRYPTOPALS_CTR_H
