//
// Created by omerh on 28/12/2019.
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

TEST(set_5_3, g_is_1)
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
    //     set g'=1
    //
    // When changing those params for bob, the math will be:
    //     B = (g'^b)%p = 1
    //     Sb = (A^b) <------------ unknown
    //
    // For alice:
    //     A = (g^a)%p
    //     Sa = (B^a)%p = ((g'^b)6a)%p = (1^ab)%p = 1
    //
    // That means, bob got a actual unknown key but we know that the key alice got is 1
    //
    decltype(msg1.g) g_tag = {0};
    g_tag[63] = 1;

    msg1.g = g_tag;

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
    auto bob_decrypted_data = msg3.msg;
    bob_enc.decrypt(bob_decrypted_data.data(), bob_decrypted_data.size());
    //
    // Because we changed the params, bob will be unable to decrypt the message, but we can!
    //

    // -----------------------|Eve|--------------------------
    // We know that the session key was 1, now we can decrypt the communication
    //
    aes_t eve_enc;
    std::array<unsigned char, 64> eve_session_key = {0};
    eve_session_key[63] = 1;
    auto eve_aes_key = derive_key<aes_t::key_size>(eve_session_key.data(), eve_session_key.size());
    auto eve_decrypted_data = msg3.msg;
    eve_enc.initialize(eve_aes_key.data(), eve_aes_key.size(), IV.data(), IV.size());
    eve_enc.decrypt(eve_decrypted_data.data(), eve_decrypted_data.size());

    // ----------------------|TEST|--------------------------
    ASSERT_NE(bob_decrypted_data, message_to_send);
    ASSERT_EQ(eve_decrypted_data, message_to_send);
}

TEST(set_5_3, g_is_p)
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
    //     set g'=p
    //
    // When changing those params for bob, the math will be:
    //     B = (g'^b)%p = (p^b) % p = 0
    //     Sb = (A^b) <------------ unknown
    //
    // For alice:
    //     A = (g^a)%p
    //     Sa = (B^a)%p = ((g'^b)6a)%p = (p^ab)%p = 0
    //
    // That means, bob got a actual unknown key but we know that the key alice got is 0
    //
    decltype(msg1.g) g_tag = {0};

    msg1.g = g_tag;

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
    auto bob_decrypted_data = msg3.msg;
    bob_enc.decrypt(bob_decrypted_data.data(), bob_decrypted_data.size());
    //
    // Because we changed the params, bob will be unable to decrypt the message, but we can!
    //

    // -----------------------|Eve|--------------------------
    // We know that the session key was 0, now we can decrypt the communication
    //
    aes_t eve_enc;
    std::array<unsigned char, 64> eve_session_key = {0};

    auto eve_aes_key = derive_key<aes_t::key_size>(eve_session_key.data(), eve_session_key.size());
    auto eve_decrypted_data = msg3.msg;
    eve_enc.initialize(eve_aes_key.data(), eve_aes_key.size(), IV.data(), IV.size());
    eve_enc.decrypt(eve_decrypted_data.data(), eve_decrypted_data.size());

    // ----------------------|TEST|--------------------------
    ASSERT_NE(bob_decrypted_data, message_to_send);
    ASSERT_EQ(eve_decrypted_data, message_to_send);
}

TEST(set_5_3, g_is_p_minus_1)
{
    /*
     * Lets see the math:
     *
     *            n
     * (a+b)^n =  E C(N,K)*a^(n-k)*b^k
     *           k=0
     *
     * When a=p, b=-1 and the whole expression is with modulo p, we get:
     *                n
     * ((p-1)^n)%P = (E C(n,k)*p^(n-k)*(-1)^k)%p
     *               k=0
     *
     * Lets take a look of each summand:
     *
     *     (C(n,k)*p^(n-k)*(-1)^k)%p
     *
     * for n-k != 0:
     *
     * 	(C(n,k)*p^(n-k)*(-1)^k)%p = 0
     * 	                |               |
     *      Multiplication with p        Modulus p
     *
     * for n=k:
     *     C(n,k) = C(n,n) = 1
     *     (C(n,k)*p^(n-k)*(-1)^k)%p = (C(n,n)*(p^0)*(-1)^k)%p =
     *
     *     (1*1*(-1)^k)%p = ((-1)^k)%p = { p-1 ,k is odd
     *                                     1   ,k is even
     *
     * so, the expression is:
     *
     *                n
     * ((p-1)^n)%P = (E C(n,k)*p^(n-k)*(-1)^k)%p = 0+0+...+((-1)^k)%p = { p-1 ,k is odd
     *               k=0                                                  1   ,k is even
     *
     */

    //
    // Lets skip the actual implementation, it's too boring...
    //
}