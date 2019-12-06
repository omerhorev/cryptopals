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
    unsigned int int_0001 = 0x0001;
    unsigned int int_0002 = 0x0002;
    unsigned int int_0003 = 0x0003;
    unsigned int int_0005 = 0x0005;
    unsigned int int_00ff = 0x00ff;
    unsigned int int_10ff = 0x10ff;

    math::num512_t num_0001 = int_0001;
    math::num512_t num_0002 = int_0002;
    math::num512_t num_0003 = int_0003;
    math::num512_t num_0005 = int_0005;
    math::num512_t num_00ff = int_00ff;
    math::num512_t num_10ff = int_10ff;

    ASSERT_EQ(num_00ff % int_0001, int_00ff % int_0001);
    ASSERT_EQ(num_00ff % num_0001, int_00ff % int_0001);

    ASSERT_EQ(num_00ff % int_0002, int_00ff % int_0002);
    ASSERT_EQ(num_00ff % num_0002, int_00ff % int_0002);

    ASSERT_EQ(num_00ff % int_0003, int_00ff % int_0003);
    ASSERT_EQ(num_00ff % num_0003, int_00ff % int_0003);

    ASSERT_EQ(num_00ff % int_0005, int_00ff % int_0005);
    ASSERT_EQ(num_00ff % num_0005, int_00ff % int_0005);

    ASSERT_EQ(num_0003 % int_0003, int_0003 % int_0003);
    ASSERT_EQ(num_0003 % num_0003, int_0003 % int_0003);

    ASSERT_EQ(num_0002 % int_0003, int_0002 % int_0003);
    ASSERT_EQ(num_0002 % num_0003, int_0002 % int_0003);

    ASSERT_EQ(num_10ff % int_0003, int_10ff % int_0003);
    ASSERT_EQ(num_10ff % num_0003, int_10ff % int_0003);

    num_10ff %= num_0005;
    ASSERT_EQ(num_10ff, int_10ff % int_0005);

    num_00ff %= num_0005;
    ASSERT_EQ(num_00ff, int_00ff % int_0005);
}