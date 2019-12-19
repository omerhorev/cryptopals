//
// Created by omerh on 01/12/2019.
//

#include <math/internal/bignum.h>
#include <algorithm>
#include <stdexcept>
#include <utils/debug.h>

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

void bignum::subtract(unsigned char *number, size_t length, const unsigned char *value, size_t value_length)
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
    auto l1 = minimum_required_length(number, length);
    auto l2 = minimum_required_length(value, value_length);

    if (l1 != length || l2 != value_length)
    {
        //
        // This calculation can be simplified if we don't try to subtract the whole number if it's full of zeros
        //
        subtract(number + length - l1, l1, value + value_length - l2, l2);
        return;
    }

    if (compare(number, length, value, value_length) < 0)
    {
        throw std::underflow_error("Result will be a negative number");
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
}

void bignum::set(unsigned char *number, size_t length, const unsigned char *value, size_t value_length)
{
    if (length < value_length)
    { throw std::runtime_error("The left side's max value must be grater or equal to the right side's max value"); }

    std::copy_n(value, value_length, number + length - value_length);
    std::fill_n(number, length - value_length, 0);
}

int bignum::compare(const unsigned char *first, size_t first_length, const unsigned char *second, size_t second_length)
{
    if (first_length < second_length)
    {
        return compare(second, second_length, first, first_length) > 0 ? -1 : 1;
    }

    unsigned diff_length = first_length - second_length;
    int eq = 0;

    for (auto i = 0; i < diff_length; i++)
    {
        if (first[i] != 0)
        {
            if (eq == 0) eq = 1;
        }
    }

    first += diff_length;

    for (auto i = 0; i < second_length; i++)
    {
        if (eq == 0)
        {
            if (first[i] > second[i]) eq = 1;
            else if (first[i] < second[i]) eq = -1;
        }
    }

    return eq;
}

void bignum::mod(unsigned char number[], size_t length, const unsigned char N[], size_t N_length)
{
    while (true)
    {
        size_t l = 0;

        while (compare(number, l, N, N_length) < 0 && l <= length)
        {
            l++;
        }

        if (l > length)
        {
            break;
        }

        while (compare(number, l, N, N_length) >= 0)
        {
            subtract(number, l, N, N_length);
        }
    }
}

void bignum::modpow(unsigned char number[], size_t number_length,
                    const unsigned char exponent[], size_t exponent_length,
                    const unsigned char *N, size_t N_length)
{
    auto result_length = number_length;
    auto result = new unsigned char[result_length];

    auto temp_number_length = number_length;
    auto temp_number = new unsigned char[temp_number_length];

    auto temp_exponent_length = exponent_length;
    auto temp_exponent = new unsigned char[temp_exponent_length];

    set(temp_number, temp_number_length, number, number_length);
    set(temp_exponent, temp_exponent_length, exponent, exponent_length);

    try
    {
        modpow(temp_number, temp_number_length, temp_exponent, temp_exponent_length, N, N_length, result,
               result_length);
    }
    catch (std::exception &e)
    {
        delete[] result;
        throw e;
    }

    std::copy_n(result, number_length, number);

    delete[] result;
}

void bignum::modpow(unsigned char base[], size_t base_length,
                    unsigned char exponent[], size_t exponent_length,
                    const unsigned char N[], size_t N_length,
                    unsigned char result[], size_t result_length)
{
    unsigned char num_0[] = {0};
    unsigned char num_1[] = {1};

    auto _base = new unsigned char[base_length];
    set(_base, base_length, base, base_length);

    auto _exponent = new unsigned char[base_length];
    auto _exponent_length = exponent_length;
    set(_exponent, exponent_length, exponent, exponent_length);

    if (compare(N, N_length, num_1, sizeof(num_1)) == 0)
    {
        set(result, result_length, num_0, sizeof(num_0));
        return;
    }

    utils::print_buffer("base", base, base_length);
    utils::print_buffer("exponent", exponent, exponent_length);
    utils::print_buffer("N", N, N_length);

    mod(base, base_length, N, N_length);
    utils::print_buffer("base %= N", base, base_length);

    set(result, result_length, num_1, sizeof(num_1));
    utils::print_buffer("result", result, result_length);

    while (!is_empty(exponent, exponent_length))
    {
        std::cout << "---------------------" << std::endl;
        utils::print_buffer("exponent", exponent, exponent_length);

        if (is_odd(exponent, exponent_length))
        {
            std::cout << "result is odd, result = (result * base) % N" << std::endl;

            utils::print_buffer("result before", result, result_length);
            utils::print_buffer("base", base, base_length);
            //result = (result * base) % modulus;
            modmul(result, result_length, base, base_length, N, N_length);

            utils::print_buffer("result after", result, result_length);
        }

        utils::print_buffer("base before", base, base_length);
        modsquare(base, base_length, N, N_length);
        utils::print_buffer("base after", base, base_length);

        utils::print_buffer("exponent before", exponent, exponent_length);
        shift_right(exponent, exponent_length, 1);
        utils::print_buffer("exponent after", exponent, exponent_length);
    }

    utils::print_buffer("result", result, result_length);
}


void bignum::modmul(unsigned char *number, size_t length,
                    const unsigned char *value, size_t value_length,
                    const unsigned char *N, size_t N_length)
{
    if (is_empty(N, N_length))
    {
        throw std::logic_error("Can't calculate modulus 0");
    }

    if (is_empty(number, length) || is_empty(value, value_length))
    {
        std::fill_n(number, length, 0);
        return;
    }

    auto v = new unsigned char[value_length];
    std::copy_n(value, value_length, v);

    modmul_internal(number, length, v, value_length, N, N_length);

    delete[] v;
}

void bignum::modmul_internal(unsigned char *a, size_t a_length,
                             unsigned char *b, size_t b_length,
                             const unsigned char *N, size_t N_length)
{
    unsigned char one[] = {1};

    if (compare(a, a_length, one, sizeof(one)) == 0)
    {
        set(a, a_length, b, b_length);
        mod(a, a_length, N, N_length);
        return;
    }

    if (is_odd(a, a_length))
    {
        //unsigned int temp = b;
        auto *temp = new unsigned char[b_length];
        set(temp, b_length, b, b_length);

        decrease(a, a_length);

        modmul_internal(a, a_length, b, b_length, N, N_length);
        mod(a, a_length, N, N_length);
        add(a, a_length, temp, b_length);
        mod(a, a_length, N, N_length);
    }
    else
    {
        shift_right(a, a_length, 1);
        shift_left(b, b_length, 1);
        mod(b, b_length, N, N_length);
        modmul_internal(a, a_length, b, b_length, N, N_length);
        mod(a, a_length, N, N_length);
    }
}


void bignum::shift_left(unsigned char number[], size_t length, unsigned int count)
{
    constexpr const unsigned char mask[] = {0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f};
    auto bits = count % CHAR_BIT;
    auto bytes = count / CHAR_BIT;

    //
    // Shift to left by multiples of 8 is shifting left bytes.
    // For example:
    //
    //     [X0 X1 X2 X3 X4 X5 X6 X7] [X8 X9 Xa Xb Xc Xd Xe Xf] Shift one byte left is equals to:
    //     [X8 X9 XA Xb Xc Xd Xe Xf] [0  0  0  0  0  0  0  0 ]
    //
    //     and...
    //
    //     [X0 X1 X2 X3 X4 X5 X6 X7] [X8 X9 Xa Xb Xc Xd Xe Xf] Shift 8 bits left is equals to:
    //     [X8 X9 XA Xb Xc Xd Xe Xf] [0  0  0  0  0  0  0  0 ]
    //
    //     The same!
    //
    for (auto i = 0; i < length - bytes; i++)
    {
        number[i] = number[i + bytes];
    }

    for (auto i = length - bytes; i < length; i++)
    {
        number[i] = 0;
    }

    //
    // We had to shift N bits and N = 8*bytes_count + bits_count.
    // We already shifted 8*bytes_count when we shifted the bytes, all we have to do is shift the bits now!
    //
    for (auto i = 0; i < length; i++)
    {
        number[i] <<= bits;

        if (i + 1 < length)
        {
            number[i] |= ((number[i + 1] & ~mask[CHAR_BIT - bits - 1]) >> (CHAR_BIT - bits));
        }
    }
}


void bignum::shift_right(unsigned char number[], size_t length, unsigned int count)
{
    constexpr const unsigned char mask[] = {0x00, 0x01, 0x03, 0x07, 0x0f, 0x1f, 0x3f, 0x7f};
    auto bits = count % CHAR_BIT;
    auto bytes = count / CHAR_BIT;

    if (bytes >= length)
    {
        std::fill_n(number, length, 0);
        return;
    }

    for (int i = 0; i < length - bytes; ++i)
    {
        auto l = length - i - 1;
        number[l] = number[l - bytes];
    }

    std::fill_n(number, bytes, 0);

    for (int i = 0; i < length; ++i)
    {
        auto l = length - i - 1;

        number[l] >>= bits;

        if (l > 0)
        {
            number[l] |= (number[l - 1] & mask[bits]) << (CHAR_BIT - bits);
        }
    }
}


bool bignum::is_empty(const unsigned char *number, size_t length)
{
    return compare(number, length, nullptr, 0) == 0;
}

size_t bignum::minimum_required_length(const unsigned char number[], size_t length)
{
    size_t l = 0;
    while (*(number + l) == 0 && l != length) l++;

    return length - l;
}

bool bignum::is_odd(const unsigned char *number, size_t length)
{
    return (number[length - 1] & 0x1) == 1;
}

bool bignum::is_even(const unsigned char *number, size_t length)
{
    return !is_odd(number, length);
}

void bignum::decrease(unsigned char *number, size_t length)
{
    unsigned char one[] = {1};

    subtract(number, length, one, sizeof(one));
}

void bignum::increase(unsigned char *number, size_t length)
{
    unsigned char one[] = {1};

    add(number, length, one, sizeof(one));
}

void bignum::modsquare(unsigned char *number, size_t length, const unsigned char *N, size_t N_length)
{
    auto l = length;
    auto _ = new unsigned char[l];
    set(_, l, number, length);

    modmul(number, length, _, l, N, N_length);

    delete[] _;
}
