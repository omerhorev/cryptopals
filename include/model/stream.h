//
// Created by omerh on 22/10/2019.
//

#ifndef CRYPTOPALS_STREAM_H
#define CRYPTOPALS_STREAM_H

#include <model/internal/stream.h>
#include <model/internal/aes/aes.h>
#include <model/internal/aes/aes-falvor.h>

namespace model
{
    class aes128_ctr
            : public internal::default_ctr_stream_cipher<internal::aes::aes_model<internal::aes::aes_flavor_aes128>, internal::moo::ctr_format::nonce_first, internal::endianness::little>
    {
    };

    class aes128_ctr_be
            : public internal::default_ctr_stream_cipher<internal::aes::aes_model<internal::aes::aes_flavor_aes128>, internal::moo::ctr_format::nonce_first, internal::endianness::big>
    {
    };

    class aes196_ctr
            : public internal::default_ctr_stream_cipher<internal::aes::aes_model<internal::aes::aes_flavor_aes192>, internal::moo::ctr_format::nonce_first, internal::endianness::little>
    {
    };

    class aes196_ctr_be
            : public internal::default_ctr_stream_cipher<internal::aes::aes_model<internal::aes::aes_flavor_aes192>, internal::moo::ctr_format::nonce_first, internal::endianness::big>
    {
    };

    class aes256_ctr
            : public internal::default_ctr_stream_cipher<internal::aes::aes_model<internal::aes::aes_flavor_aes256>, internal::moo::ctr_format::nonce_first, internal::endianness::little>
    {
    };

    class aes256_ctr_be
            : public internal::default_ctr_stream_cipher<internal::aes::aes_model<internal::aes::aes_flavor_aes256>, internal::moo::ctr_format::nonce_first, internal::endianness::big>
    {
    };
}

#endif //CRYPTOPALS_STREAM_H
