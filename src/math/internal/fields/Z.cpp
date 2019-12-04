//
// Created by omerh on 01/12/2019.
//

#include <math/internal/fields/Z.h>
#include <algorithm>
#include <stdexcept>

using namespace math;
using namespace math::internal;
using namespace math::internal::fields;

void Z::add(unsigned char *number, size_t length, const unsigned char *value, size_t value_length)
{
    /**
     *     78 a4
     *     01 82
     *     ----- +
     *     7a 26
     *
     * Lets take a deeper look. We calculate from the lsb to the msb, and considering each byte to be a digit.
     * lets calculate a4+82:
     *     a4 + 82 = 126 = 100 + 26 = 26 and carry for the next round
     *
     * Because the result exceeds the digit size, we need to set carry for the next calculation:
     *     78 + 1 + carry = 78 + 1 + 1 = 7a.
     *
     * and the result is 7a26.
     *
     * Another example, for the biggest digits (with carry) we get:
     *     ff + ff + carry = ff + ff + 1 = 1ff = 100 + ff -> ff and carry for the next round
     *
     */

    if (length < value_length)
    {
        throw std::runtime_error("The left side's max value must be grater or equal to the right side's max value");
    }

    unsigned char carry = 0;
    number += length;
    value += value_length;

    for (auto i = 0; i < value_length; ++i)
    {
        number--;
        value--;

        //
        // 'a' for sure can hold both numbers
        //
        unsigned int a = *number + *value + carry;
        carry = (unsigned char) (a > 0xff ? 1 : 0);
        *number = (unsigned char) (carry ? (a - 0x100) : a);
    }

    for (auto i = value_length; i < length; ++i)
    {
        number--;
        value--;

        //
        // 'a' for sure can hold both numbers
        //
        unsigned int a = *number + carry;
        carry = (unsigned char) (a > 0xff ? 1 : 0);
        *number = (unsigned char) (carry ? (a - 0x100) : a);
    }

    if (carry)
    {
        throw std::overflow_error("The result is to big to be held in the receivers side");
    }
}

void Z::subtract(unsigned char *number, size_t length, const unsigned char *value, size_t value_length)
{
    /**
     *     87 33
     *     00 46
     *     ----- -
     *     86 ED
     *
     * Lets take a deeper look. We calculate from the lsb to the msb, and considering each byte to be a digit.
     * lets calculate 33-46:
     *     33 - 46 ~= 100 + (33 - 46) = ED and carry for the next round
     *
     * Because the result exceeds the digit size, we need to set carry for the next calculation:
     *     87 - 0 - carry = 87 - 0 - 1 = 7a.
     *
     * and the result is 86ED.
     *
     * Another example, for the biggest digits (with carry) we get:
     *     0100 - 1 = 00 - 01 = 100 - 00 - 1 = ff, 1-1 = 0 -> 00FF
     *
     */

    if (length < value_length)
    {
        throw std::runtime_error("The left side's max value must be grater or equal to the right side's max value");
    }

    unsigned char carry = 0;
    number += length;
    value += value_length;

    for (auto i = 0; i < value_length; ++i)
    {
        number--;
        value--;

        //
        // 'a' for sure can hold both numbers
        //
        unsigned int a = (0x100ul + *number) - *value - carry;
        carry = (unsigned char) (a < 0x100 ? 1 : 0);
        *number = (unsigned char) (carry ? a : a - 0x100);
    }

    for (auto i = value_length; i < length; ++i)
    {
        number--;

        //
        // 'a' for sure can hold both numbers
        //
        unsigned int a = (0x100ul + *number) - carry;
        carry = (unsigned char) (a < 0x100 ? 1 : 0);
        *number = (unsigned char) (carry ? a : a - 0x100);
    }

    if (carry)
    {
        throw std::overflow_error("The result is to big to be held in the receivers side");
    }
}

void Z::set(unsigned char *number, size_t length, const unsigned char *value, size_t value_length)
{
    if (length < value_length)
    { throw std::runtime_error("The left side's max value must be grater or equal to the right side's max value"); }

    std::copy_n(value, value_length, number + length - value_length);
    std::fill_n(number, length - value_length, 0);
}

int Z::compare(const unsigned char *first, size_t first_length, const unsigned char *second, size_t second_length)
{
    auto bigger_length = std::max(first_length, second_length);
    auto smaller_length = std::min(first_length, second_length);

    const unsigned char *bigger = (first_length > second_length) ? first : second;
    const unsigned char *smaller = (first_length > second_length) ? second : first;

    int eq = 0;

    for (auto i = 0; i < smaller_length; i++)
    {
        if (bigger[bigger_length - i - 1] != smaller[smaller_length - i - 1])
        {
            if (eq == 0)
            {
                eq = bigger[bigger_length - i - 1] > smaller[smaller_length - i - 1] ? 1 : -1;
            }
            //
            // Do not 'break' or 'return' here! this is done to prevent timing attacks
            //
        }
    }

    for (auto i = smaller_length; i < bigger_length; i++)
    {
        if (bigger[bigger_length - i - 1] != 0)
        {
            eq = 1;
            //
            // Do not 'break' or 'return' here! this is done to prevent timing attacks
            //
        }
    }

    if (eq == 1)
    {
        return (first_length > second_length) ? 1 : -1;
    }
    else if (eq == -1)
    {
        return (first_length > second_length) ? -1 : 1;
    }
    else
    {
        return 0;
    }
}
