//
// Created by omerh on 01/12/2019.
//

#include <gtest/gtest.h>
#include <math/numbers.h>


TEST(numbers, comparison)
{
    math::num512_t a = 1;
    math::num512_t b = 2;
    math::num512_t c = 1;
    math::num512_t d = 0xffffff;
    math::num32_t e = 0xffffff0a;

    ASSERT_FALSE(a == b);
    ASSERT_TRUE(c == a);

    ASSERT_TRUE(b == 2);
    ASSERT_TRUE(2 == b);
    ASSERT_FALSE(b == 1);
    ASSERT_FALSE(1 == b);

    ASSERT_TRUE(a == 1);
    ASSERT_TRUE(1 == a);
    ASSERT_FALSE(a == 0xffff);
    ASSERT_FALSE(0xffff == a);

    ASSERT_TRUE(d == 0xffffff);

    ASSERT_TRUE(e == 0xffffff0a);

    ASSERT_FALSE(a != 1);
    ASSERT_FALSE(1 != a);
    ASSERT_TRUE(a != 2);
    ASSERT_TRUE(2 != a);

    ASSERT_FALSE(a == b);
    ASSERT_FALSE(b == a);
    ASSERT_TRUE(a != b);
    ASSERT_TRUE(b != a);
}

TEST(numbers, addition)
{
    math::num512_t a = 0;
    math::num512_t b = 1;
    math::num512_t c = 0xff;
    math::num512_t d = 0x100;

    ASSERT_EQ(a + 1, b);
    ASSERT_EQ(c + 1, 0x100);
    ASSERT_EQ(a + c, c);
    ASSERT_EQ(b + c, d);

    a += b;
    c += d;
    b += 1;

    ASSERT_EQ(a, 1);
    ASSERT_EQ(c, 0xff + 0x100);
    ASSERT_EQ(b, a + 1);
}

TEST(numbers, increment)
{
    math::num512_t a = 0xfd;

    ASSERT_EQ(a++, 0xfd);
    ASSERT_EQ(a, 0xfe);
    ASSERT_EQ(++a, 0xff);
    ASSERT_EQ(a++, 0xff);
    ASSERT_EQ(a, 0x100);
}

TEST(numbers, substraction)
{
    math::num512_t a = 0x2;
    math::num512_t b = 0x3;
    math::num512_t c = 0x100;

    ASSERT_EQ(b - a, 0x1);
    ASSERT_EQ(c - a, 0x100 - 0x2);
}

TEST(numbers, decreasement)
{
    math::num512_t a = 0x100;

    ASSERT_EQ(a--, 0x100);
    ASSERT_EQ(a, 0xff);
    ASSERT_EQ(--a, 0xfe);
}

TEST(numbers, compare)
{
    unsigned big = 0xff11;
    unsigned eq = 0x0123;
    unsigned small = 0x0020;

    math::num512_t a = eq;
    math::num512_t _small = small;
    math::num512_t _eq = eq;
    math::num512_t _big = big;


    ASSERT_TRUE(a > small);
    ASSERT_TRUE(a > _small);
    ASSERT_TRUE(small < a);
    ASSERT_TRUE(_small < a);
    ASSERT_FALSE(a < small);
    ASSERT_FALSE(a < _small);
    ASSERT_FALSE(small > a);
    ASSERT_FALSE(_small > a);

    ASSERT_TRUE(a < big);
    ASSERT_TRUE(a < _big);
    ASSERT_TRUE(big > a);
    ASSERT_TRUE(_big > a);
    ASSERT_FALSE(a > big);
    ASSERT_FALSE(a > _big);
    ASSERT_FALSE(big < a);
    ASSERT_FALSE(_big < a);

    ASSERT_TRUE(a >= small);
    ASSERT_TRUE(a >= _small);
    ASSERT_TRUE(small <= a);
    ASSERT_TRUE(_small <= a);
    ASSERT_FALSE(a <= small);
    ASSERT_FALSE(a <= _small);
    ASSERT_FALSE(small >= a);
    ASSERT_FALSE(_small >= a);
    ASSERT_TRUE(a <= eq);
    ASSERT_TRUE(a <= _eq);
    ASSERT_TRUE(eq >= a);
    ASSERT_TRUE(_eq >= a);

    ASSERT_TRUE(a <= big);
    ASSERT_TRUE(a <= _big);
    ASSERT_TRUE(big >= a);
    ASSERT_TRUE(_big >= a);
    ASSERT_FALSE(a >= big);
    ASSERT_FALSE(a >= _big);
    ASSERT_FALSE(big <= a);
    ASSERT_FALSE(_big <= a);
    ASSERT_TRUE(a >= eq);
    ASSERT_TRUE(a >= _eq);
    ASSERT_TRUE(eq <= a);
    ASSERT_TRUE(_eq <= a);
}

TEST(numbers, modulus)
{
    unsigned int int_000001 = 0x000001;
    unsigned int int_000002 = 0x000002;
    unsigned int int_000003 = 0x000003;
    unsigned int int_000005 = 0x000005;
    unsigned int int_0000ff = 0x0000ff;
    unsigned int int_0010ff = 0x0010ff;
    unsigned int int_010000 = 0x010000;
    unsigned int int_001000 = 0x001000;
    unsigned int int_1fe891 = 0x1fe891;
    unsigned int int_098765 = 0x098765;

    math::num512_t num_000001 = int_000001;
    math::num512_t num_000002 = int_000002;
    math::num512_t num_000003 = int_000003;
    math::num512_t num_000005 = int_000005;
    math::num512_t num_0000ff = int_0000ff;
    math::num512_t num_0010ff = int_0010ff;
    math::num512_t num_010000 = int_010000;
    math::num512_t num_001000 = int_001000;
    math::num512_t num_1fe891 = int_1fe891;
    math::num512_t num_098765 = int_098765;

    ASSERT_EQ(num_0010ff % num_000003, int_0010ff % int_000003);

    ASSERT_EQ(num_0000ff % int_000001, int_0000ff % int_000001);
    ASSERT_EQ(num_0000ff % num_000001, int_0000ff % int_000001);

    ASSERT_EQ(num_0000ff % int_000002, int_0000ff % int_000002);
    ASSERT_EQ(num_0000ff % num_000002, int_0000ff % int_000002);

    ASSERT_EQ(num_0000ff % int_000003, int_0000ff % int_000003);
    ASSERT_EQ(num_0000ff % num_000003, int_0000ff % int_000003);

    ASSERT_EQ(num_0000ff % int_000005, int_0000ff % int_000005);
    ASSERT_EQ(num_0000ff % num_000005, int_0000ff % int_000005);

    ASSERT_EQ(num_000003 % int_000003, int_000003 % int_000003);
    ASSERT_EQ(num_000003 % num_000003, int_000003 % int_000003);

    ASSERT_EQ(num_000002 % int_000003, int_000002 % int_000003);
    ASSERT_EQ(num_000002 % num_000003, int_000002 % int_000003);

    ASSERT_EQ(num_0010ff % int_000003, int_0010ff % int_000003);
    ASSERT_EQ(num_0010ff % num_000003, int_0010ff % int_000003);

    ASSERT_EQ(num_010000 % int_001000, int_010000 % int_001000);
    ASSERT_EQ(num_010000 % num_001000, int_010000 % int_001000);

    ASSERT_EQ(num_010000 % int_000003, int_010000 % int_000003);
    ASSERT_EQ(num_010000 % num_000003, int_010000 % int_000003);

    ASSERT_EQ(num_1fe891 % int_098765, int_1fe891 % int_098765);
    ASSERT_EQ(num_1fe891 % num_098765, int_1fe891 % int_098765);

    num_0010ff %= num_000005;
    ASSERT_EQ(num_0010ff, int_0010ff % int_000005);

    num_0000ff %= num_000005;
    ASSERT_EQ(num_0000ff, int_0000ff % int_000005);

    math::num32_t a = 0x01234567;
    math::num32_t b = 201;

    ASSERT_EQ(a % b, 0x01234567 % 201);
}