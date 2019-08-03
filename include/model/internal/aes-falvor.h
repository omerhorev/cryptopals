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

        class aes_flavor_aes192
        {
        public:
            static constexpr size_t rounds_count = 12;
            static constexpr size_t key_size = 24;
        };

        class aes_flavor_aes256
        {
        public:
            static constexpr size_t rounds_count = 14;
            static constexpr size_t key_size = 32;
        };
    }
}

#endif //CRYPTOPALS_AES_FALVOR_H
