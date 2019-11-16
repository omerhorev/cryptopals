//
// Created by omerh on 16/11/2019.
//

#include <gtest/gtest.h>
#include <random>
#include <model/stream.h>
#include <utils/base64.h>
#include <utils/random.h>

// The fixture for testing class Foo.
class set_4_2 : public ::testing::Test
{
private:
    unsigned char _random_key[16] = {0};
    unsigned int _random_nonce;

public:

    set_4_2() : _random_nonce(0), _random_key()
    {
        utils::random::instance().fill(_random_key);
        _random_nonce = utils::random::instance().get<uint32_t>();
    }

    size_t generate_message(const unsigned char *user_message,
                            size_t user_message_length,
                            unsigned char *buffer,
                            size_t buffer_length)
    {
        const char *pre_msg = "comment1=cooking MCs;userdata=";
        const char *post_msg = ";comment2= like a pound of bacon";

        model::aes128_ctr model;
        model.initialize_encryption(_random_key, sizeof(_random_key), _random_nonce, 0);

        size_t total_length = strlen(pre_msg) + strlen(post_msg) + user_message_length;

        if (buffer_length < strlen(pre_msg) + strlen(post_msg) + user_message_length)
        { throw std::length_error("Invalid length"); }

        std::copy_n(pre_msg, strlen(pre_msg), buffer);
        std::copy_n(user_message, user_message_length, buffer + strlen(pre_msg));
        std::copy_n(post_msg, strlen(post_msg), buffer + strlen(pre_msg) + user_message_length);

        model.encrypt(buffer, total_length);

        return total_length;
    }

    bool validate_message(const unsigned char *msg, size_t length)
    {
        model::aes128_ctr model;
        model.initialize_decryption(_random_key, sizeof(_random_key), _random_nonce, 0);

        auto *data = new unsigned char[length];

        std::copy_n(msg, length, data);

        model.decrypt(data, length);

        std::string s((const char *) data);

        auto a = s.find("admin=1");

        delete[] data;

        return a != std::string::npos;
    }
};


TEST_F(set_4_2, run)
{
    /**
     * A simple bitflipping attack on CTR mode.
     * This attack uses the fact that CTR works like this:
     *
     *     C = enc(counter + nonce, key) ^ P
     *
     * And in decryption like this:
     *
     *     P = enc(counter + nonce, key) ^ C
     *
     * Meaning, that each change to the ciphertext will be directly applied to the same bit in the decrypted plaintext.
     *
     * Like set 4.1, this isn't hard mathematically, but passes the idea that messages needs to be validated and not
     * only decrypted. a simple hash (or even crc!) would do the work here!
     */
    unsigned char buffer[1000];
    auto *msg = (const unsigned char *) "XX0000000000000000admin<1000000000";

    auto l = generate_message(msg, strlen((const char *) msg), buffer, sizeof(buffer));

    ASSERT_FALSE(validate_message(buffer, l));

    //
    // Now, lets flip the correct bit:
    // The crafted message is:
    // [--------------][--------------][--------------][--------------][--------------][--------------]
    // comment1=cooking MCs;userdata=XX0000000000000000admin<1000000000;comment2= like a pound of bacon
    //

    buffer[53] ^= 0x01;

    ASSERT_TRUE(validate_message(buffer, l));
}