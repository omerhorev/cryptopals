//
// Created by omerh on 22/11/2019.
//

#ifndef CRYPTOPALS_BREAKS_SHA1_H
#define CRYPTOPALS_BREAKS_SHA1_H

#include <cstdio>
#include <stdexcept>
#include <model/internal/utils.h>
#include <model/hash.h>

namespace breaks
{
    class sha1
    {
    public:
        static const size_t sha1_chunk_size = 64;

        /**
         * Given a chunk containing a message to sign, completes the chunk with the padding as if it was padded
         * by the sha1 itself.
         *
         * @param chunk             A 64 bytes chunk with enough space to complete the padding.
         * @param chunk_filled_size The length of the filled part of the last chunk,
         *                          must be smaller than 57 (64 - 1[pad byte] - 8[64bit size])
         * @param message_length    The total length of the message used for the padding
         */
        static void complete_padding(unsigned char chunk[sha1_chunk_size],
                                     size_t chunk_filled_size,
                                     size_t message_length)
        {
            if (chunk_filled_size > 56)
            {
                throw std::length_error("For simplicity, use a smaller message");
            }

            std::fill_n(chunk + chunk_filled_size, sha1_chunk_size - chunk_filled_size, 0);
            chunk[chunk_filled_size] = 0x80;

            model::internal::endianness_encoder<uint64_t, model::internal::endianness::big>::encode(
                    message_length * CHAR_BIT,
                    *(unsigned char (*)[sizeof(uint64_t)]) (chunk + sha1_chunk_size - sizeof(uint64_t)));
        }

        static model::sha1 generate_sha1_from_result(unsigned char result[], size_t result_legnth, size_t length_so_far)
        {
            if (result_legnth < model::sha1::hash_size)
            {
                throw std::length_error("The result buffer provided is too small");
            }

            struct sha1_internal_state
            {
                uint64_t _message_length;

                size_t _buffer_index;
                unsigned char _buffer[64];
                uint32_t _h0;
                uint32_t _h1;
                uint32_t _h2;
                uint32_t _h3;
                uint32_t _h4;
            };

            struct sha1_result
            {
                unsigned char h_raw[5][4];
            };

            using be_encoder = model::internal::endianness_encoder<uint32_t, model::internal::endianness::big>;

            auto r = (sha1_result *) result;

            model::sha1 _;

            auto state = (sha1_internal_state *) &_;

            state->_h0 = be_encoder::decode(r->h_raw[0]);
            state->_h1 = be_encoder::decode(r->h_raw[1]);
            state->_h2 = be_encoder::decode(r->h_raw[2]);
            state->_h3 = be_encoder::decode(r->h_raw[3]);
            state->_h4 = be_encoder::decode(r->h_raw[4]);
            state->_message_length = length_so_far;

            return _;
        }
    };
}

#endif //CRYPTOPALS_BREAKS_SHA1_H
