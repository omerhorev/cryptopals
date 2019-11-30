#include <gtest/gtest.h>
#include <random>
#include <model/block.h>
#include <model/padding.h>
#include <utils/base64.h>
#include <breaks/byte-at-a-time-ecb-decryption.h>
#include <breaks/bitflipping.h>
#include <utils/random.h>
#include <utils/debug.h>

// The fixture for testing class Foo.
class set_2_8 : public ::testing::Test
{
private:
    unsigned char _random_key[16] = {0};
    unsigned char _random_iv[16] = {0};

public:

    void SetUp() override
    {
        utils::random::instance().fill(_random_key);
        utils::random::instance().fill(_random_iv);
    }

    void TearDown() override
    {
    }

    size_t generate_message(const unsigned char *user_message,
                            size_t user_message_length,
                            unsigned char *buffer,
                            size_t buffer_length)
    {
        const char *pre_msg = "comment1=cooking MCs;userdata=";
        const char *post_msg = ";comment2= like a pound of bacon";

        model::aes128_cbc model;
        model.initialize(_random_key, sizeof(_random_key), _random_iv, sizeof(_random_iv));

        size_t total_length = strlen(pre_msg) + strlen(post_msg) + user_message_length;

        if (buffer_length < strlen(pre_msg) + strlen(post_msg) + user_message_length)
        { throw std::length_error("Invalid length"); }

        std::copy_n(pre_msg, strlen(pre_msg), buffer);
        std::copy_n(user_message, user_message_length, buffer + strlen(pre_msg));
        std::copy_n(post_msg, strlen(post_msg), buffer + strlen(pre_msg) + user_message_length);

        auto enc_length = model::padding::pkcs7::encode(buffer,
                                                        buffer_length,
                                                        total_length,
                                                        model::aes128_ecb::block_size);

        if (enc_length == 0)
        {
            return 0;
        }

        model.encrypt(buffer, enc_length);

        return enc_length;
    }

    bool validate_message(const unsigned char *msg, size_t length)
    {
        model::aes128_cbc model;
        model.initialize(_random_key, sizeof(_random_key), _random_iv, sizeof(_random_iv));

        auto *data = new unsigned char[length];

        std::copy_n(msg, length, data);

        model.decrypt(data, length);

        // Debug prints
        utils::print_blocked_buffer(data, length, model::aes128_cbc::block_size);

        auto enc_length = model::padding::pkcs7::decode(data,
                                                        length,
                                                        model::aes128_cbc::block_size);

        if (enc_length == 0)
        {
            throw std::runtime_error("The buffer is not aligned to block size or invalid padding");
        }

        std::string s((const char *) data);

        auto a = s.find("admin=1");

        delete[] data;

        return a != std::string::npos;
    }
};


TEST_F(set_2_8, run)
{
    unsigned char buffer[1000];
    auto *msg = (const unsigned char *) "XX0000000000000000admin<1000000000";

    auto l = generate_message(msg, strlen((const char*)msg), buffer, sizeof(buffer));

    ASSERT_FALSE(validate_message(buffer, l));

    breaks::bitflippig::do_flip(buffer, model::aes128_cbc::block_size * 2, model::aes128_cbc::block_size);
    ASSERT_TRUE(validate_message(buffer, l));
}