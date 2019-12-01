//
// Created by omerh on 01/12/2019.
//

#include <math/internal/bignum.h>
#include <algorithm>
#include <stdexcept>

using namespace math;
using namespace math::internal;

void bignum::add(unsigned char *number, size_t length, const unsigned char *value, size_t value_length)
{
    /**
     *     78 a4
     *     01 82
     *     ----- +
     *     7a 26
     *
     * Lets take a deeper look. We do the calculated from the lsb to the msb, and considering each byte to be a digit.
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

    size_t smaller_size = std::min(length, value_length);

    unsigned char carry = 0;

    for (auto i = 0; i < smaller_size; ++i)
    {
        //
        // 'a' for sure can hold both numbers
        //
        unsigned int a = number[i] + value[i] + carry;
        carry = (unsigned char) (a > 0xff ? 1 : 0);
    }

    for (auto i = smaller_size; i < length; ++i)
    {
        //
        // 'a' for sure can hold both numbers
        //
        unsigned int a = number[i] + carry;
        carry = (unsigned char) (a > 0xff ? 1 : 0);
    }

    if (carry)
    {
        throw std::overflow_error("The result is to big to be held in the receivers side");
    }
}

void bignum::set(unsigned char *number, size_t length, const unsigned char *value, size_t value_length)
{
    if (length < value_length)
    { throw std::runtime_error("The left side's max value must be grater or equal to the right side's max value"); }

    std::copy_n(value, value_length, number + length - value_length);
    std::fill_n(number, length - value_length, 0);
}

bool bignum::equal(const unsigned char *first, size_t first_length, const unsigned char *second, size_t second_length)
{
    auto bigger_length = std::max(first_length, second_length);
    auto smaller_length = std::min(first_length, second_length);

    const unsigned char *bigger = (first_length > second_length) ? first : second;
    const unsigned char *smaller = (first_length > second_length) ? second : first;

    bool eq = true;

    for (auto i = 0; i < smaller_length; i++)
    {
        if (bigger[bigger_length - i - 1] != smaller[smaller_length - i - 1])
        {
            eq = false;
            //
            // Do not 'break' or 'return' here! this is done to prevent timing attacks
            //
        }
    }

    for (auto i = smaller_length; i < bigger_length; i++)
    {
        if (bigger[bigger_length - i - 1] != 0)
        {
            eq = false;
            //
            // Do not 'break' or 'return' here! this is done to prevent timing attacks
            //
        }
    }

    return eq;
}
