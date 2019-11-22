//
// Created by omerh on 22/11/2019.
//

#include <gtest/gtest.h>
#include <utils/random.h>
#include <model/hash.h>

class set_4_5 : public testing::Test
{
public:

    struct protocol_message
    {
        static const size_t signature_length = model::sha1::hash_size;

        unsigned char signature[signature_length];
        unsigned char message[];
    };

    set_4_5() : _secret_key(std::move(utils::random::instance().buffer<16>()))
    {}

    size_t sign(const unsigned char message[], size_t message_length, unsigned char buffer[], size_t buffer_length)
    {
        if (buffer_length < model::sha1::hash_size)
        {
            throw std::length_error("Not enough space in the buffer provided");
        }

        auto msg = (protocol_message *) buffer;

        model::sha1 hash;

        hash.update(_secret_key.data(), _secret_key.size());
        hash.update(message, message_length);

        hash.digest(msg->signature, sizeof(msg->signature));
        std::copy_n(message, message_length, msg->message);

        return protocol_message::signature_length + message_length;
    }

    bool validate(const unsigned char signed_message[], size_t length)
    {
        if (length < model::sha1::hash_size)
        {
            throw std::length_error("Not enough space in the buffer provided");
        }

        auto msg = (protocol_message *) signed_message;

        unsigned char result[model::sha1::hash_size];

        model::sha1 hash;

        hash.update(_secret_key.data(), _secret_key.size());
        hash.update(msg->message, length - protocol_message::signature_length);

        hash.digest(result, sizeof(result));

        return std::equal(result, result + sizeof(result), msg->signature);
    }

    std::array<unsigned char, 16> _secret_key;
};

TEST_F(set_4_5, run)
{
    unsigned char buffer[60];
    const char *msg = "this is a message with 31 chars";

    auto l = sign((const unsigned char *) msg, strlen(msg), buffer, sizeof(buffer));

    ASSERT_EQ(l, model::sha1::hash_size + strlen(msg));

    ASSERT_TRUE(validate(buffer, l));
}