//
// Created by omerh on 09/08/2019.
//

#include <math/galois-field.h>

#include "math/galois-field.h"

using namespace math;

galois256 galois256::operator+(galois256 v) const
{
    return (galois256) (v._value ^ _value);
}

galois256 galois256::operator*(galois256 v) const
{
    unsigned char p = 0;
    unsigned char a = _value;
    unsigned char b = v._value;

    for (auto i = 0; i < 8; ++i)
    {
        if ((b & 0x01) != 0) p ^= a;

        b >>= 1;

        auto carry = (a & 0x80) != 0;

        a <<= 1;

        if (carry) a ^= 0x1b;

    }

    return (galois256) p;
}

galois256 galois256::operator^(galois256 v) const
{
    galois256 p = (galois256) 1;

    for (auto i = 0; i < v._value; ++i)
    {
        p = p * ((galois256) 2);
    }

    return p;
}

galois256 &galois256::operator+=(galois256 v)
{
    *this = *this + v;
    return *this;
}

galois256 &galois256::operator=(galois256 v)
{
    _value = v._value;
    return *this;
}

galois256::operator unsigned char()
{
    return _value;
}

bool galois256::operator==(galois256 v) const
{
    return _value == v._value;
}

galois256 galois256::operator++()
{
    _value++;
    return *this;
}

const galois256 galois256::operator++(int)
{
    galois256 temp = *this;
    _value++;

    return temp;
}

galois256 galois256::operator*(const unsigned char v) const
{
    return *this * galois256(v);
}

galois256 &galois256::operator=(unsigned char v)
{
    _value = v;

    return *this;
}
