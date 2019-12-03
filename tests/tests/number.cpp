//
// Created by omerh on 01/12/2019.
//

#include <gtest/gtest.h>
#include <math/number.h>


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
    ASSERT_EQ(a, 0xff + 0x100);
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