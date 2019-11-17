//
// Created by omerh on 17/11/2019.
//

#include <gtest/gtest.h>
#include <random>
#include <model/block.h>
#include <model/padding.h>
#include <utils/base64.h>
#include <breaks/byte-at-a-time-ecb-decryption.h>
#include <breaks/bitflipping.h>
#include <utils/random.h>
#include <breaks/internal/ecb-block-parser.h>

// The fixture for testing class Foo.
class set_4_3 : public ::testing::Test
{
public:
    struct bad_data : public std::exception
    {
        /**
         * Initializes an invalid key exception
         *
         * @param key    The key buffer provided.
         * @param length The length of the key buffer provided.
         */
        bad_data(const unsigned char data[], size_t length) : data(data), length(length)
        {}

        const char *what() const noexcept override
        {
            return "Bad data received";
        }

        const unsigned char *data;
        size_t length;
    };

public:
    unsigned char _random_key[16] = {0};

public:

    void SetUp() override
    {
        utils::random::instance().fill(_random_key);
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

        // Use key as the IV (this is bad!)
        model.initialize(_random_key, sizeof(_random_key), _random_key, sizeof(_random_key));

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
        model.initialize(_random_key, sizeof(_random_key), _random_key, sizeof(_random_key));

        auto *data = new unsigned char[length];

        std::copy_n(msg, length, data);

        model.decrypt(data, length);

        auto enc_length = model::padding::pkcs7::decode(data,
                                                        length,
                                                        model::aes128_cbc::block_size);

        if (enc_length == 0)
        {
            throw std::runtime_error("The buffer is not aligned to block size or invalid padding");
        }

        data[enc_length] = 0;
        std::string s((const char *) data);

        if (!std::all_of(s.cbegin(), s.cend(), [](const auto &c) { return std::isprint(c); }))
        {
            throw bad_data(data, enc_length);
        }

        auto a = s.find("admin=1");

        delete[] data;

        return a != std::string::npos;
    }
};


TEST_F(set_4_3, run)
{
    /**
     *
     * CBC decryption looks like this:
     * (https://en.wikipedia.org/wiki/Block_cipher_mode_of_operation#/media/File:CBC_decryption.svg)
     *
     *        K   C1            K   C2            K   C2
     *        |   |________     |   |________     |   |
     *        |   |        |    |   |        |    |   |
     *      Decryption     |  Decryption     |  Decryption      ...
     *           |         |       |         |       |
     *     IV---XOR        \------XOR        \------XOR
     *           |                 |                 |
     *           P1                P2                P2
     *
     *     P1  = Dec(K, C1) ^ Iv
     *     P2  = Dec(K, C2) ^ C1
     *     P3  = Dec(K, C3) ^ C2
     *
     * So, if we use Key=IV we get:
     *
     *     P1  = Dec(K, C1) ^ K
     *     P2  = Dec(K, C2) ^ C1
     *     P3  = Dec(K, C3) ^ C2
     *
     * Lets change after the encryption so C2=0, C3=C1. This will effect P2 and P3. Lets mark:
     *
     *     P1  = Dec(K, C1) ^ K
     *     P'2 = Dec(K, 0) ^ C1
     *     P'3 = Dec(K, C1) ^ 0
     *
     * If the server raises an error (because the message is invalid after changing P2 to 0) and reveals P'2 and P'3 we
     * get:
     *     (We know: P1,P2,P3,C1,C2,C3,P'2,P'3 and we want to know K)
     *
     *     P1 ^ P'3 = Dec(K, C1) ^ K * Dec(K, C1) ^ 0 = K ^ 0 = K
     *
     *  ____________________________________________________________
     * |                                                            |
     * | Conclusion: Don't reveal anything in the returning errors! |
     * |____________________________________________________________|
     */
    const auto block_size = model::aes128_cbc::block_size;
    unsigned char buffer[1000];
    unsigned char P_tag[1000];
    size_t P_tag_length = 0;
    auto *msg = (const unsigned char *) "XX";

    auto l = generate_message(msg, strlen((const char *) msg), buffer, sizeof(buffer));

    //
    // The crafted message is:
    //        P1              P2              P3            ...
    // [--------------][--------------][--------------][--------------]
    // comment1=cooking MCs;userdata=XX;comment2= like a pound of bacon
    //
    //
    // Like in the break description, we mark:
    //     C'1 = C1
    //     C'2 = 0
    //     C'3 = C1
    //
    std::fill_n(buffer + block_size * 1, block_size, 0);
    std::copy_n(buffer + block_size * 0, block_size, buffer + block_size * 2);

    try
    {
        validate_message(buffer, l);
    }
    catch (set_4_3_run_Test::bad_data const &e)
    {
        std::copy_n(e.data, e.length, P_tag);
        P_tag_length = e.length;
    }
    catch (...)
    {
        FAIL();
    }

    ASSERT_GT(P_tag_length, 0);

    breaks::internal::ecb_block_parser plain_parser(P_tag, P_tag_length, model::aes128_cbc::block_size);
    unsigned char IV[model::aes128_cbc::block_size];

    //
    // Like in the break description, we mark:
    //     IV = P'1 ^ P'3
    //
    utils::general::xor_buffers(plain_parser[0].data(), plain_parser[2].data(), IV, model::aes128_cbc::block_size);

    ASSERT_TRUE(memcmp(IV, _random_key, model::aes128_cbc::block_size) == 0);
}