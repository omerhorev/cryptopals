//
// Created by omerh on 13/08/2019.
//


#include <gtest/gtest.h>
#include "utils/debug.h"
#include "model/internal/moo/ecb.h"

using namespace model;
using namespace model::internal;
using namespace model::internal::moo;

class dummy_enc
{
public:
    static constexpr size_t block_size = 16;

    void encrypt(unsigned char (&block)[block_size])
    {
        for (auto &c : block) c ^= 3;
    }

    void decrypt(unsigned char (&block)[block_size])
    {
        for (auto &c : block) c ^= 3;
    }
};

TEST(moo, ecb)
{
    dummy_enc enc;
    ecb<dummy_enc> moo(enc);

    unsigned char plain[] = {0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee,
                             0xff, 0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd,
                             0xee, 0xff,};
    unsigned char crypt[32];

    std::copy(plain, plain + 32, crypt);

    for (auto &c : crypt)
    {
        c ^= 3;
    }

    moo.encrypt(plain, sizeof(plain));

    ASSERT_TRUE(memcmp(plain, crypt, sizeof(plain)) == 0);

}