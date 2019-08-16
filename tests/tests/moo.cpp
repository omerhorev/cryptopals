//
// Created by omerh on 13/08/2019.
//


#include <gtest/gtest.h>
#include <model/xor-byte-key.h>
#include "utils/general.h"
#include "model/block.h"
#include <model/internal/moo/cbc.h>
#include "model/internal/moo/ecb.h"
#include "model/internal/aes/aes-utils.h"
#include "model/internal/aes/aes-falvor.h"
#include "model/internal/aes/aes.h"

using namespace model;
using namespace model::internal;
using namespace model::internal::moo;
using namespace model::internal::aes;

class dummy_enc
{
public:
    static constexpr size_t block_size = 16;

    void encrypt(unsigned char block[], size_t length)
    {
        model::xor_byte_key::encrypt(block, block, block_size, 3);
    }

    void decrypt(unsigned char block[], size_t length)
    {
        model::xor_byte_key::encrypt(block, block, block_size, 3);
    }

    void encrypt(unsigned char (&block)[block_size])
    {
        model::xor_byte_key::encrypt(block, block, block_size, 3);
    }

    void decrypt(unsigned char (&block)[block_size])
    {
        model::xor_byte_key::encrypt(block, block, block_size, 3);
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

TEST(moo, cbc)
{
    const size_t block_size = dummy_enc::block_size;

    dummy_enc enc;
    cbc<dummy_enc> moo(enc);

    unsigned char iv[] =
            {
                    0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff
            };

    unsigned char plain[] =
            {
                    // Block 1
                    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                    // Block 2
                    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77, 0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff,
                    // Block 3
            };

    unsigned char crypt[32] = {0};
    unsigned char crypt_expected[32] = {0};
    memcpy(crypt_expected, plain, sizeof(plain));

    utils::general::xor_buffers(plain, iv, crypt, block_size);
    enc.encrypt(crypt, block_size);

    utils::general::xor_buffers(crypt, plain + block_size, crypt + block_size, block_size);
    enc.encrypt(crypt + block_size, block_size);

    moo.initialize(iv, block_size);
    moo.encrypt(crypt_expected, sizeof(crypt_expected));

    ASSERT_TRUE(memcmp(crypt_expected, crypt, sizeof(crypt_expected)) == 0);
}

TEST(moo, cbc_aes128)
{
    struct
    {
        unsigned char key[aes_flavor_aes128::key_size];
        unsigned char iv[aes_utils::block_size];
        unsigned char plain[aes_utils::block_size * 4];
        unsigned char crypt[aes_utils::block_size * 4];
    } test_vector = {
            {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c,},
            {0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0a, 0x0b, 0x0c, 0x0d, 0x0e, 0x0f,},
            {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a, 0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51, 0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef, 0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10,},
            {0x76, 0x49, 0xab, 0xac, 0x81, 0x19, 0xb2, 0x46, 0xce, 0xe9, 0x8e, 0x9b, 0x12, 0xe9, 0x19, 0x7d, 0x50, 0x86, 0xcb, 0x9b, 0x50, 0x72, 0x19, 0xee, 0x95, 0xdb, 0x11, 0x3a, 0x91, 0x76, 0x78, 0xb2, 0x73, 0xbe, 0xd6, 0xb8, 0xe3, 0xc1, 0x74, 0x3b, 0x71, 0x16, 0xe6, 0x9e, 0x22, 0x22, 0x95, 0x16, 0x3f, 0xf1, 0xca, 0xa1, 0x68, 0x1f, 0xac, 0x09, 0x12, 0x0e, 0xca, 0x30, 0x75, 0x86, 0xe1, 0xa7,}
    };


    aes128_cbc model;
    model.initialize(test_vector.key, sizeof(test_vector.key), test_vector.iv, sizeof(test_vector.iv));
    model.encrypt(test_vector.plain + aes_utils::block_size * 0, aes_utils::block_size * 3);
    model.encrypt(test_vector.plain + aes_utils::block_size * 3, aes_utils::block_size * 1);

    ASSERT_TRUE(memcmp(test_vector.plain, test_vector.crypt, sizeof(test_vector.crypt)) == 0);

}