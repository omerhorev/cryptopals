//
// Created by omerh on 22/11/2019.
//

#include <gtest/gtest.h>
#include <utils/random.h>
#include <model/hash.h>
#include <breaks/sha1.h>

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

    union sha1_result
    {
        unsigned char raw[model::sha1::hash_size];
        uint32_t h[model::sha1::hash_size / sizeof(uint32_t)];
    };

    struct sha1_intenral_state
    {
        uint64_t _message_length;
        size_t _buffer_index;
        unsigned char _buffer[64];
        sha1_result h;
    };

    std::array<unsigned char, 16> _secret_key;
};

TEST_F(set_4_5, run)
{
    std::cout << "-----------" << std::endl;

    unsigned char buffer[100];
    const char *msg1 = "yyyy";
    const size_t msg1_length = strlen(msg1);

    auto l = sign((const unsigned char *) msg1, msg1_length, buffer, sizeof(buffer));

    ASSERT_EQ(l, model::sha1::hash_size + msg1_length);

    ASSERT_TRUE(validate(buffer, l));

    /**
     * Now the break will generate a new message and without knowing the key used to calculate the signature with
     * we will create a message that will be validated correctly:
     *
     *    S1 = sha1(key, m1, (padding1))
     *    M1 = (S1, m1)
     *
     * Now lets create a malicious message M2:
     *
     *    S2 = sha1[initialized with S1](m1, padding1, m2, (padding2))
     *    M2 = (S2, m1, padding1, m2)
     *
     * because when calculating sha we eventually just return the internal state, the following principle is valid:
     *
     *     (ignoring padding for simplicity)
     *     S1 = sha1(x)
     *     S2 = sha1[initialized with S1](y) = sha1(x, y)
     */
    const char *msg2 = "xxxx";
    const size_t msg2_length = strlen(msg2);
    const size_t key_size = 16;

    unsigned char new_buffer[100] = {0};
    unsigned char *data = new_buffer + 20;

    //
    // The hash object extracted contains the hash of [(secret, msg1, pad1), len: secret+msg]
    // That means, in order to sign a new message, we need to
    //     1) create the message msg1, pad1, msg2
    //     2) update with the value msg2
    //
    // The hash object of the hash already digested the key, the msg1 and the padding; the exact calculation
    // will be provided in the next comments, but eventually we get that it has digested 64 bytes.
    //
    auto hash = breaks::sha1::generate_sha1_from_result(buffer, model::sha1::hash_size, 64);

    hash.update((unsigned char*)msg2, msg2_length);
    //
    // After the update and the digest, the hash digested:
    //            PREVIOUSLY             ATTACK
    //     [key, msg1, padding1] + [msg2, padding2]
    //
    // With:
    //     padding1 for [key, msg1](len: 64 - (16 + 4) bytes)
    //     padding2 for [key, msg1, padding1, msg2](len: 64 - (4) bytes, total length of 4 + 16 + (64-4-16) + 4 = 64 + 4 = 68)
    //
    hash.digest(new_buffer, 20);

    //
    // For building the message, lets first calculate some lengths:
    //     pad_length + len(key) + len(msg1) = 64 ->
    //     pad_length + 16 + 4 = 64 ->
    //     pad_length = 48
    //
    // So, if the hash was calculated like this:
    //     hash(key | msg1 | pad)
    //
    // And the new hash will be calculated like this
    //     hash(key | M | msg2)
    //
    // That means that for this attack to work we will need that:
    //     M = msg1 | pad    ->
    //     len(M) = len(msg1) + pad_length ->
    //     len(M) = 4 + 48
    //

    std::copy_n(msg1, msg1_length, data);

    //
    // Add the padding as if the has was updated with (key, msg1)
    //
    breaks::sha1::complete_padding(data - key_size, msg1_length + key_size, msg1_length + key_size);

    //
    // The padding must have added 44 bytes (chunk - key - msg1 -> 64-16-4=44). msg2 need to be written in +(44+4)
    //
    std::copy_n(msg2, msg2_length, data + 48);

    ASSERT_TRUE(validate(new_buffer, 20 + 48 + 4));
}