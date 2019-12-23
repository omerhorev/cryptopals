//
// Created by omerh on 20/12/2019.
//

#include <gtest/gtest.h>
#include <math/numbers.h>
#include <model/diffie-hellman.h>
#include <utils/random.h>
#include <utils/debug.h>

using namespace math;

static uint32_t modpow(uint32_t a, uint32_t b, uint32_t N)
{
    uint64_t _ = 1;
    for (int i = 0; i < b; ++i)
    {
        _ *= a;
        _ %= N;
    }

    return (uint32_t) (_ % N);
}


TEST(set_5_1, math_small)
{
    uint32_t g = 5;
    uint32_t p = 37;

    uint32_t a = 123 % p;
    auto A = modpow(g, a, p);

    uint32_t b = 321 % p;
    auto B = modpow(g, b, p);

    uint32_t alice_s = modpow(B, a, p);
    uint32_t bob_s = modpow(A, b, p);

    ASSERT_EQ(alice_s, bob_s);
}

TEST(set_5_1, math_big1)
{
    using num = math::num32_t;
    num g = 5;
    num p = 0x7fffffff;

    // -----  Alice -----
    num a = 123;
    a %= p;
    num A = g;
    A.apply_modular_power(a, p);

    // ----- Bob --------
    num b = 321;
    b %= p;
    num B = g;
    B.apply_modular_power(b, p);


    // ----- Both -------
    num s1 = A;
    num s2 = B;
    s1.apply_modular_power(b, p);
    s2.apply_modular_power(a, p);

    ASSERT_EQ(s1, s2);
}

TEST(set_5_1, math_big2)
{
    using num = math::num512_t;
    num g = 2;
    num p;

    const char *str_p = "7ca682ce8e12caba26efccf7110e526db078b05edecbcd1eb4a208f3ae1617ae01f35b91a47e6df63413c5e12ed0899bcd132acd50d99151bdc43ee737592e17";
    auto str_p_len = strlen(str_p);
    utils::hex::decode(str_p, str_p_len, (unsigned char *) &p, sizeof(p));

    // -----  Alice -----
    num a = 123;
    a %= p;
    num A = g;
    A.apply_modular_power(a, p);

    // ----- Bob --------
    num b = 321;
    b %= p;
    num B = g;
    B.apply_modular_power(b, p);


    // ----- Both -------
    num s1 = A;
    num s2 = B;
    s1.apply_modular_power(b, p);
    s2.apply_modular_power(a, p);

    ASSERT_EQ(s1, s2);
}

TEST(set_5_1, internal)
{
    using dh_t = model::internal::diffie_hellman<512>;

    dh_t alice_model(7, 8);
    dh_t bob_model(7, 8);

    auto a = 90;
    auto b = 91;

    alice_model.seed(a);
    bob_model.seed(b);

    auto A = alice_model.generate_public_key();
    auto B = bob_model.generate_public_key();

    auto s1 = alice_model.generate_session_key(B);
    auto s2 = bob_model.generate_session_key(A);

    ASSERT_EQ(s1, s2);
}

TEST(set_5_1, run)
{
    using dh_t = model::diffie_hellman<512>;
    std::array<unsigned char, 64> a{};
    std::array<unsigned char, 64> b{};
    std::array<unsigned char, 64> A{};
    std::array<unsigned char, 64> B{};
    std::array<unsigned char, 64> S1{};
    std::array<unsigned char, 64> S2{};

    // ----------------------|Alice|-------------------------
    dh_t alice_model;
    a = utils::random::instance().buffer<64>();
    alice_model.seed(a.data(), a.size());
    alice_model.generate_public_key(A.data(), A.size());

    // -----------------------|Bob|--------------------------
    dh_t bob_model;
    b = utils::random::instance().buffer<64>();
    bob_model.seed(b.data(), b.size());
    bob_model.generate_public_key(B.data(), B.size());

    // ----------------------|Alice|-------------------------
    alice_model.generate_session_key(B.data(), B.size(), S1.data(), S1.size());

    // -----------------------|Bob|--------------------------
    bob_model.generate_session_key(A.data(), A.size(), S2.data(), S2.size());

    ASSERT_EQ(S1, S2);
}