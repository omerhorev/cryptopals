//
// Created by omerh on 15/11/2019.
//

#include <gtest/gtest.h>
#include <model/prng.h>
#include <chrono>
#include <utils/random.h>

class set_3_8 : public ::testing::Test
{
public:
    static const auto token_byte_count = 16;

    using token_t = std::array<unsigned char, token_byte_count>;

    size_t encrypt(const unsigned char plain[], size_t plain_length, unsigned char cipher[], size_t cipher_length)
    {
        auto random_key = utils::random::instance().get<uint16_t>();
        auto postfix_length = utils::random::instance().get<size_t>(10, 20);
        auto length = plain_length + postfix_length;

        if (cipher_length < length)
        {
            return 0;
        }

        std::copy_n(plain, plain_length, cipher);

        utils::random::instance().fill(cipher + plain_length, postfix_length);

        encrypt(random_key, cipher, length);

        return length;
    }

    token_t generate_token()
    {
        return generate_token_by_unix_timestamp(uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count()));
    }

    token_t generate_token_by_unix_timestamp(unsigned int unix_timestamp)
    {
        token_t _ = {0};
        encrypt(uint32_t(unix_timestamp), _.data(), _.size());

        return _;
    }

private:

    void encrypt(uint32_t key, unsigned char buffer[], size_t length)
    {
        auto _ = model::mt19937(key);

        for (auto i = 0; i < length; i++)
        {
            buffer[i] ^= _.generate();
        }
    }

};

TEST_F(set_3_8, part1)
{
    //
    // Crack the encryption seed from the mt19937 based stream cipher using known plain
    //
    unsigned char plain[] = {0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,
                             0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A, 0x0A,};

    unsigned char cipher[256];

    auto length = encrypt(plain, sizeof(plain), cipher, sizeof(cipher));

    ASSERT_GT(length, 0);

    // Because its 16 bit, we can actually iterate over it!

    bool found = false;

    for (auto key = 0; key < 0xFFFF; ++key)
    {
        auto _ = model::mt19937(uint32_t(key));
        found = true;

        for (auto i = 0; i < 16; ++i)
        {
            auto g = _.generate();

            if (cipher[i] != (unsigned char) (g ^ plain[i]))
            {
                found = false;
                break;
            }
        }

        if (found)
        {
            break;
        }
    }

    ASSERT_TRUE(found);
}

TEST_F(set_3_8, part2)
{
    auto token = generate_token();

    auto timestamp = uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now().time_since_epoch()).count());

    bool found = false;
    
    for (int i = 0; i < 10000; ++i)
    {
        if (generate_token_by_unix_timestamp(timestamp--) == token)
        {
            found = true;
        }
    }

    ASSERT_TRUE(found);
}