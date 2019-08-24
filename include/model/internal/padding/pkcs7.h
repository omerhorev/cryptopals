//
// Created by omerh on 14/08/2019.
//

#ifndef CRYPTOPALS_INTERNAL_PKCS7_H
#define CRYPTOPALS_INTERNAL_PKCS7_H

#include <cstddef>
#include <algorithm>

namespace model
{
    namespace internal
    {
        namespace padding
        {
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
                    if (block_size > 0xff)
                    {
                        return 0;
                    }

                    size_t bytes_to_add = block_size - (data_length % block_size);

                    if (buffer_length < data_length + bytes_to_add)
                    {
                        return 0;
                    }

                    for (auto i = 0; i < bytes_to_add; ++i)
                    {
                        data[data_length + i] = (unsigned char) bytes_to_add;
                    }

                    return data_length + bytes_to_add;
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
                    if (buffer_length % block_size != 0)
                    {
                        return 0;
                    }

                    auto expected_pad_length = size_t(data[buffer_length - 1]);
                    auto expected_pad_byte = (unsigned char) expected_pad_length;

                    for (size_t i = 0; i < expected_pad_length; i++)
                    {
                        if (data[buffer_length - i - 1] != expected_pad_byte)
                        {
                            return 0;
                        }
                    }

                    return buffer_length - expected_pad_length;
                }
            };
        };
    }
}

#endif //CRYPTOPALS_INTERNAL_PKCS7_H
