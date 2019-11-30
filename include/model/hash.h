//
// Created by omerh on 22/11/2019.
//

#ifndef CRYPTOPALS_HASH_H
#define CRYPTOPALS_HASH_H

#include <model/internal/hash/sha1.h>
#include <model/internal/hash/md4.h>

namespace model
{
    using sha1 = model::internal::hash::sha1<uint32_t, uint64_t, 0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476, 0xC3D2E1F0>;
    using md4 = model::internal::hash::md4<uint32_t, uint64_t, 0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476>;
}

#endif //CRYPTOPALS_HASH_H
