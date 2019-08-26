//
// Created by omerh on 24/08/2019.
//

#include <gtest/gtest.h>
#include <cstring>
#include <model/block.h>
#include <model/padding.h>
#include "utils/cookie_encoding.h"

static unsigned char g_key[16] = {0, 1, 2, 3, 4, 5, 6, 7, 0, 1, 2, 3, 4, 5, 6, 7,};

struct user_profile
{
    static const size_t max_email_length = 32;
    static const size_t max_role_length = 32;

    char email[max_email_length];
    unsigned int uid;
    char role[max_role_length];
};

namespace utils
{
    template<>
    class cookie_encoder<user_profile> : internal::encode_cookie_base
    {
    public:
        std::string encode(const user_profile &val)
        {
            std::string s;
            s += encode_field("email", val.email);
            s += encode_field("uid", val.uid);
            s += encode_field("role", val.role);

            return s;
        }

        void decode(const std::string &string, user_profile &obj)
        {
            decode_field(string, "email", obj.email);
            decode_field(string, "uid", obj.uid);
            decode_field(string, "role", obj.role);
        }
    };
}

user_profile profile_for(const char email[])
{
    user_profile u{};

    std::strcpy(u.email, email);
    std::strcpy(u.role, "user");
    u.uid = 10;

    return u;
}

size_t encryption_oracle(const unsigned char plain[],
                         size_t plain_length,
                         unsigned char cipher[],
                         size_t max_cipher_length)
{
    model::aes128_ecb model;
    model.initialize(g_key, sizeof(g_key));

    if (plain_length > max_cipher_length)
    {
        return 0;
    }

    std::copy_n(plain, plain_length, cipher);

    auto encoded_length = model::padding::pkcs7::encode(cipher,
                                                        max_cipher_length,
                                                        plain_length,
                                                        model::aes128_ecb::block_size);

    if (encoded_length == 0)
    {
        return 0;
    }

    model.encrypt(cipher, encoded_length);

    return encoded_length;
}


TEST(sets, set_2_5)
{
    const size_t max_cipher_length = 256;

    unsigned char profile_cipher[max_cipher_length];
    size_t profile_cipher_length;

    unsigned char admin_cipher[max_cipher_length];
    size_t admin_cipher_length;

    //
    // The cookie-encoded data will be in the form of email=foo@bar.com&uid=10&role=user
    // so... lets craft one that we can split into blocks. we will need:
    //  1) Some block containing a "admin\11\11\11\11\11\11\11\11\11\11\11" ("admin" + PKCS7 padding)
    //  2) Some block containing all the data until the '=' of the role=user (including the '=')
    //
    // That means we need to encrypt twice (the ? is padding...):
    //  1) For the admin and padding block we will need this data:
    //
    //     [              ][       *      ][              ][              ]
    //     email=__________admin???????????@domain.com&uid=10&role=user????
    //
    //
    //  2) For the user profile block we will need:
    //
    //     [       *      ][        *     ][        *     ][              ]
    //     email=the_user_was_hacked@hacker.io&uid=10&role=user????????????
    //
    // Then, we will combine the blocks 2 and 1,2,3 of the data and we will get
    //
    //     [       *      ][        *     ][        *     ][        *     ]
    //     email=the_user_was_hacked@hacker.io&uid=10&role=admin???????????
    //
    //

    //
    // Lets start with the blocks for the username...
    //
    {
        auto u = profile_for("the_user_was_hacked@hacker.io");
        auto encoded_profile = utils::encode_as_cookie(u);
        profile_cipher_length = encryption_oracle((unsigned char *) encoded_profile.c_str(),
                                                  encoded_profile.length(),
                                                  profile_cipher,
                                                  sizeof(profile_cipher));
    }

    //
    // Lets generate the user so we can steal the 'admin' block
    //
    {
        char malicious_email[27] = {0};
        std::strcpy((char *) malicious_email, "__________admin");

        model::padding::pkcs7::encode((unsigned char *) malicious_email + 10, // Skip all the ___...__ at the start
                                      model::aes128_ecb::block_size,          // 16 byte
                                      strlen("admin"),                        // Encode the block starting with 'admin'
                                      model::aes128_ecb::block_size);         // 16 byte

        auto u = profile_for(malicious_email);
        auto encoded_profile = utils::encode_as_cookie(u);
        admin_cipher_length = encryption_oracle((unsigned char *) encoded_profile.c_str(),
                                                encoded_profile.length(),
                                                admin_cipher,
                                                sizeof(admin_cipher));
    }

    //
    // Now... for the final step, lets combine the two generated profiles specific blocks to generate a valid
    // admin user profile!!
    //
    unsigned char cipher[max_cipher_length] = {0};
    std::copy_n(profile_cipher,
                model::aes128_ecb::block_size * 3,
                cipher);
    std::copy_n(admin_cipher + model::aes128_ecb::block_size * 1,
                model::aes128_ecb::block_size,
                cipher + model::aes128_ecb::block_size * 3);


    //
    // ------------------------------------------------------------------------------------------------------------
    //
    // Only for checking ....
    // Decrypt profile
    //
    model::aes128_ecb model;
    model.initialize(g_key, sizeof(g_key));

    model.decrypt(cipher, model::aes128_ecb::block_size * 4);
    auto data_length = model::padding::pkcs7::decode(cipher,
                                                     model::aes128_ecb::block_size * 4,
                                                     model::aes128_ecb::block_size);

    cipher[data_length] = 0;

    std::string maliciously_crafted_encoded_profile((const char*)cipher);

    user_profile maliciously_crafted_profile = {};
    utils::decode_as_cookie(maliciously_crafted_encoded_profile, maliciously_crafted_profile);

    ASSERT_STREQ(maliciously_crafted_profile.email, "the_user_was_hacked@hacker.io");
    ASSERT_STREQ(maliciously_crafted_profile.role, "admin");
    ASSERT_EQ   (maliciously_crafted_profile.uid, 10);
}