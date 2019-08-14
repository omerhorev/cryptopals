//
// Created by omerh on 13/08/2019.
//

#ifndef CRYPTOPALS_BLOCK_H
#define CRYPTOPALS_BLOCK_H

#include <model/internal/aes/aes-falvor.h>
#include <model/internal/block.h>
#include "model/internal/moo/ecb.h"
#include "model/internal/aes/aes.h"

namespace model
{
    class aes128_ecb
            : public internal::default_ecb_block_cipher<internal::aes::aes_model<internal::aes::aes_flavor_aes128>>
    {
    };

    class aes192_ecb
            : public internal::default_ecb_block_cipher<internal::aes::aes_model<internal::aes::aes_flavor_aes192>>
    {
    };

    class aes256_ecb
            : public internal::default_ecb_block_cipher<internal::aes::aes_model<internal::aes::aes_flavor_aes256>>
    {
    };
}


#endif //CRYPTOPALS_BLOCK_H
