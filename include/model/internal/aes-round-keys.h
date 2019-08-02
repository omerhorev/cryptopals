//
// Created by omerh on 01/08/2019.
//

#ifndef CRYPTOPALS_AES_ROUND_KEYS_H
#define CRYPTOPALS_AES_ROUND_KEYS_H

#include <cstddef>

namespace model
{
    namespace internal
    {
        template<typename Flavor>
        struct aes_round_keys
        {
            static constexpr size_t round_keys_count = Flavor::rounds_count + 1;

            unsigned char round_keys[16][round_keys_count];
        };
    }
}

#endif //CRYPTOPALS_AES_ROUND_KEYS_H
