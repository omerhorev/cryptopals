//
// Created by omerh on 01/08/2019.
//

#ifndef CRYPTOPALS_AES_FALVOR_H
#define CRYPTOPALS_AES_FALVOR_H

#include <cstddef>

namespace model
{
    namespace internal
    {
        class aes_flavor_aes128
        {
        public:
            static constexpr size_t rounds_count = 10;
            static constexpr size_t key_size = 16;
        };
    }
}

#endif //CRYPTOPALS_AES_FALVOR_H
