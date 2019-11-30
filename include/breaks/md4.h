//
// Created by omerh on 30/11/2019.
//

#ifndef CRYPTOPALS_BREAKS_MD4_H
#define CRYPTOPALS_BREAKS_MD4_H

#include <cstdio>
#include <stdexcept>
#include <model/internal/utils.h>
#include <model/hash.h>

namespace breaks
{
    class md4
    {
    public:
        static const size_t md4_chunk_size = 64;

        /**
         * Given a chunk containing a message to sign, completes the chunk with the padding as if it was padded
         * by the md4 itself.
         *
         * @param chunk             A 64 bytes chunk with enough space to complete the padding.
         * @param chunk_filled_size The length of the filled part of the last chunk,
         *                          must be smaller than 57 (64 - 1[pad byte] - 8[64bit size])
         * @param message_length    The total length of the message used for the padding
         */
        static void complete_padding(unsigned char chunk[md4_chunk_size],
                                     size_t chunk_filled_size,
                                     size_t message_length)
        {
            if (chunk_filled_size > 56)
            {
                throw std::length_error("For simplicity, use a smaller message");
            }

            std::fill_n(chunk + chunk_filled_size, md4_chunk_size - chunk_filled_size, 0);
            chunk[chunk_filled_size] = 0x80;

            model::internal::endianness_encoder<uint64_t, model::internal::endianness::little>::encode(
                    message_length * CHAR_BIT,
                    *(unsigned char (*)[sizeof(uint64_t)]) (chunk + md4_chunk_size - sizeof(uint64_t)));
        }

        static model::md4 generate_md4_from_result(unsigned char result[], size_t result_legnth, size_t length_so_far)
        {
            if (result_legnth < model::md4::hash_size)
            {
                throw std::length_error("The result buffer provided is too small");
            }

            struct md_internal_state
            {
                uint64_t _message_length;

                size_t _buffer_index;
                unsigned char _buffer[64];
                uint32_t _a;
                uint32_t _b;
                uint32_t _c;
                uint32_t _d;
            };

            struct md4_result
            {
                unsigned char raw[4][4];
            };

            using le_encoder = model::internal::endianness_encoder<uint32_t, model::internal::endianness::little>;

            auto r = (md4_result *) result;

            model::md4 _;

            auto state = (md_internal_state *) &_;

            state->_a = le_encoder::decode(r->raw[0]);
            state->_b = le_encoder::decode(r->raw[1]);
            state->_c = le_encoder::decode(r->raw[2]);
            state->_d = le_encoder::decode(r->raw[3]);
            state->_message_length = length_so_far;

            return _;
        }
    };
}

#endif //CRYPTOPALS_BREAKS_MD4_H
