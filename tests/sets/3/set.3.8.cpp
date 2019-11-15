//
// Created by omerh on 15/11/2019.
//

#include <gtest/gtest.h>
#include <model/prng.h>
#include <chrono>

class set_3_8 : ::testing::Test
{
public:
    static const auto token_byte_count = 16;

    using token_t = std::array<unsigned char, token_byte_count>;

    void encrypt(uint32_t key, unsigned char buffer[], size_t length)
    {
        auto _ = model::mt19937(key);

        for (auto i = 0; i < length; i++)
        {
            buffer[i] ^= _.generate();
        }
    }

    token_t generate_token()
    {
        return generate_token_by_unix_timestamp(uint32_t(std::chrono::duration_cast<std::chrono::milliseconds>(
                std::chrono::system_clock::now().time_since_epoch()).count()));
    }

    token_t generate_token_by_unix_timestamp(unsigned int unix_timestamp)
    {
        token_t _ = {0};
        encrypt(unix_timestamp, _, _.size());

        return _;
    }
};


TEST_F(set_3_8, part1)
{

}

TEST_F(set_3_8, part2)
{

}