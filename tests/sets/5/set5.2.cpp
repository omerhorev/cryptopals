//
// Created by omerh on 20/12/2019.
//

#include <model/diffie-hellman.h>
#include <model/block.h>
#include <utils/random.h>
#include <breaks/sha1.h>
#include "gtest/gtest.h"

using namespace model;

using dh_t = model::diffie_hellman<512>;
using aes_t = model::aes128_cbc;

template<size_t KeySize>
static auto derive_key(const unsigned char source[], size_t length)
{
    static_assert(KeySize <= model::sha1::hash_size, "The key can't be bigger than 20 bytes (sha1 result size)");
    unsigned char result[model::sha1::hash_size];

    std::array<unsigned char, KeySize> key = {0};
    model::sha1 hash;
    hash.update(source, length);
    hash.digest(result, sizeof(result));

    std::copy_n(result, KeySize, key.begin());

    return key;
}

TEST(set_5_2, run)
{
    std::array<unsigned char, aes_t::block_size> message_to_send = utils::random::instance().buffer<aes_t::block_size>();
    std::array<unsigned char, aes_t::block_size> IV = utils::random::instance().buffer<aes_t::block_size>();
    std::array<unsigned char, 64> a{};
    std::array<unsigned char, 64> b{};
    std::array<unsigned char, 64> S1{};
    std::array<unsigned char, 64> S2{};

    struct
    {
        std::array<unsigned char, 64> p;
        std::array<unsigned char, 64> g;
        std::array<unsigned char, 64> A;
    } msg1 = {0};

    struct
    {
        std::array<unsigned char, 64> B;
    } msg2 = {0};

    struct
    {
        std::array<unsigned char, 16> msg;
        std::array<unsigned char, 16> iv;
    } msg3 = {0};

    // ----------------------|Alice|-------------------------
    dh_t alice_model;
    a = utils::random::instance().buffer<64>();
    alice_model.seed(a.data(), a.size());
    alice_model.generate_public_key(msg1.A.data(), msg1.A.size());
    alice_model.get_p(msg1.p.data(), msg1.p.size());
    alice_model.get_g(msg1.g.data(), msg1.g.size());

    // -----------------------|Eve|--------------------------
    // Change some params:
    //     set A = p
    //
    // When changing this the math will be:
    //
    // S
    //

    // -----------------------|Bob|--------------------------
    dh_t bob_model(msg1.p._M_elems, msg1.g._M_elems);
    b = utils::random::instance().buffer<64>();
    bob_model.seed(b.data(), b.size());
    bob_model.generate_session_key(msg1.A.data(), msg1.A.size(), S2.data(), S2.size());
    bob_model.generate_public_key(msg2.B.data(), msg2.B.size());
    aes_t bob_enc;
    auto bob_aes_key = derive_key<aes_t::key_size>(S2.data(), S2.size());
    bob_enc.initialize(bob_aes_key.data(), bob_aes_key.size(), IV.data(), IV.size());

    // ----------------------|Alice|-------------------------
    alice_model.generate_session_key(msg2.B.data(), msg2.B.size(), S1.data(), S1.size());
    aes_t alice_enc;
    auto alice_aes_key = derive_key<aes_t::key_size>(S1.data(), S1.size());
    alice_enc.initialize(alice_aes_key.data(), alice_aes_key.size(), IV.data(), IV.size());
    msg3.iv = IV;
    msg3.msg = message_to_send;
    alice_enc.encrypt(msg3.msg.data(), msg3.msg.size());

    // -----------------------|Bob|--------------------------
    bob_enc.decrypt(msg3.msg.data(), msg3.msg.size());

    // ----------------------|TEST|--------------------------
    ASSERT_EQ(msg3.msg, message_to_send);
}