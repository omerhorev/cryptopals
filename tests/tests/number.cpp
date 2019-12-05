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
    math::num512_t a = 10;
    math::num512_t _1 = 1;
    math::num512_t _10 = 10;
    math::num512_t _20 = 20;

    ASSERT_TRUE(a > 1);
    ASSERT_TRUE(a > _1);
    ASSERT_TRUE(1 < a);
    ASSERT_TRUE(_1 < a);
    ASSERT_FALSE(a < 1);
    ASSERT_FALSE(a < _1);
    ASSERT_FALSE(1 > a);
    ASSERT_FALSE(_1 > a);

    ASSERT_TRUE(a < 20);
    ASSERT_TRUE(a < _20);
    ASSERT_TRUE(20 > a);
    ASSERT_TRUE(_20 > a);
    ASSERT_FALSE(a > 20);
    ASSERT_FALSE(a > _20);
    ASSERT_FALSE(20 < a);
    ASSERT_FALSE(_20 < a);

    ASSERT_TRUE(a >= 1);
    ASSERT_TRUE(a >= _1);
    ASSERT_TRUE(1 <= a);
    ASSERT_TRUE(_1 <= a);
    ASSERT_FALSE(a <= 1);
    ASSERT_FALSE(a <= _1);
    ASSERT_FALSE(1 >= a);
    ASSERT_FALSE(_1 >= a);
    ASSERT_TRUE(a <= 10);
    ASSERT_TRUE(a <= _10);
    ASSERT_TRUE(10 >= a);
    ASSERT_TRUE(_10 >= a);

    ASSERT_TRUE(a <= 20);
    ASSERT_TRUE(a <= _20);
    ASSERT_TRUE(20 >= a);
    ASSERT_TRUE(_20 >= a);
    ASSERT_FALSE(a >= 20);
    ASSERT_FALSE(a >= _20);
    ASSERT_FALSE(20 <= a);
    ASSERT_FALSE(_20 <= a);
    ASSERT_TRUE(a >= 10);
    ASSERT_TRUE(a >= _10);
    ASSERT_TRUE(10 <= a);
    ASSERT_TRUE(_10 <= a);
}