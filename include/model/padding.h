//
// Created by omerh on 16/08/2019.
//

#ifndef CRYPTOPALS_PADDING_H
#define CRYPTOPALS_PADDING_H

#include <cstddef>
#include <model/internal/padding/pkcs7.h>
#include "model/internal/padding/pkcs7.h"

namespace model
{
    class padding
    {
    public:
        class pkcs7
        {
        public:
            /**
             * Adds padding according to PKCS#7 encoding.
             *
             * @param data          (input/output) The buffer to work on.
             * @param length        The length of the actual data.
             * @param buffer_length The max length of the buffer.
             * @param block_size    The size of the block to pad accordingly.
             * @return (size_t) The size of the padded buffer.
             */
            static size_t encode(unsigned char data[], size_t buffer_length, size_t data_length, size_t block_size)
            {
                return model::internal::padding::pkcs7::encode(data, buffer_length, data_length, block_size);
            }

            /**
             * Decodes PKCS#7 padding
             *
             * @param data          The buffer of the data.
             * @param length        The length of the buffer
             * * @param block_size    The size of the block to pad accordingly.
             * @return (size_t) The number of pad bytes added, 256 if error
             */
            static size_t decode(const unsigned char data[], size_t buffer_length, size_t block_size)
            {
                return model::internal::padding::pkcs7::decode(data, buffer_length, block_size);
            }
        };
    };
}

#endif //CRYPTOPALS_PADDING_H
