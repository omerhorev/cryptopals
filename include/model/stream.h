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
            : public internal::default_ctr_stream_cipher<internal::aes::aes_model<internal::aes::aes_flavor_aes128>, internal::moo::ctr_format::nounce_first, internal::endianness::little>
    {
    };
}

#endif //CRYPTOPALS_STREAM_H
