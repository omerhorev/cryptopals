//
// Created by omerh on 22/10/2019.
//

#ifndef CRYPTOPALS_CTR_H
#define CRYPTOPALS_CTR_H

#include <cstdint>
#include <algorithm>
#include <array>
#include <model/internal/utils.h>

namespace model
{
    namespace internal
    {
        namespace moo
        {
            enum class ctr_format
            {
                nonce_first,
                counter_first
            };


            template<typename Model, unsigned int BlockSize, ctr_format Format, model::internal::endianness Endianness>
            class ctr;

            /**
             * a CTR mode of operation for 16 byte block models (meaning, that each blocks is 16 bytes, 128 bits).
             *
             * @tparam Model      The type of the model used
             * @tparam Format     The Format of the CTR used
             * @tparam Endianness The endianness to use to encode the nonce and the counter
             */
            template<typename Model, ctr_format Format, model::internal::endianness Endianness>
            class ctr<Model, 16, Format, Endianness>
            {
                static_assert(Model::block_size == 16, "The size of the model must be 16");
                static const size_t block_size = Model::block_size;

            public:
                typedef uint64_t counter_t;

                /**
                 * Initializes a new ctr mode of operation object
                 *
                 * @param model Reference to the pre-initialized model used
                 */
                explicit ctr(Model &model) : _model(model)
                {}

            public:

                /**
                 * Get a block-sized mask for a specific counter and nonce
                 * @param nonce   The nonce used
                 * @param counter The counter of the block
                 * @param mask    The mask to fill, must be a block-sized buffer
                 */
                void get_block_mask(counter_t nonce, counter_t counter, unsigned char mask[block_size])
                {
                    if (Format == ctr_format::nonce_first)
                    {
                        model::internal::encoder<counter_t, Endianness>::encode(nonce,
                                                                                *((unsigned char (*)[sizeof(counter_t)]) &mask[0]));
                        model::internal::encoder<counter_t, Endianness>::encode(counter,
                                                                                *((unsigned char (*)[sizeof(counter_t)]) &mask[8]));
                    }
                    else if (Format == ctr_format::counter_first)
                    {
                        model::internal::encoder<counter_t, Endianness>::encode(counter,
                                                                                *((unsigned char (*)[sizeof(counter_t)]) &mask[0]));
                        model::internal::encoder<counter_t, Endianness>::encode(nonce,
                                                                                *((unsigned char (*)[sizeof(counter_t)]) &mask[8]));
                    }

                    _model.encrypt(mask, block_size);
                }

            private:

                Model &_model;
            };
        }
    }
}

#endif //CRYPTOPALS_CTR_H
