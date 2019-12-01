//
// Created by omerh on 01/12/2019.
//

#ifndef CRYPTOPALS_NUMBER_H
#define CRYPTOPALS_NUMBER_H

#include <limits>

namespace math
{
    template<unsigned int BitSize>
    class number
    {
        static_assert(BitSize % 8 == 0, "only bits count that are multiplicand of 8 are supported");

    private:
        static constexpr unsigned int BytesCount = BitSize / CHAR_BIT;


    public:


        number()
        {

        }

        number(unsigned int)
        {

        }

        number(const number& other)
        {

        }

        number &operator=(const number other)
        {
            return *this;
        }

        number &operator=(number &&other) noexcept // move assignment
        {
            return *this;
        }

        number &operator=(unsigned int other) noexcept // move assignment
        {
            return *this;
        }

        number &operator++()
        {
            return *this;
        }

        const number operator++(int)
        {
            number tmp(*this); // copy
            operator++(); // pre-increment
            return tmp;   // return old value
        }

        number &operator--()
        {
            return *this;
        }

        const number operator--(int)
        {
            number tmp(*this); // copy
            operator--(); // pre-increment
            return tmp;   // return old value
        }

        number &operator+=(const number &rhs)
        {
            /* addition of rhs to *this takes place here */
            return *this;
        }

        number &operator-=(const number &rhs)
        {
            /* addition of rhs to *this takes place here */
            return *this;
        }

    private:
        unsigned char raw[BytesCount];
    };

    template<unsigned int BitSize>
    number<BitSize> operator+(number<BitSize> lhs, const number<BitSize> &rhs)
    {
        lhs += rhs; // reuse compound assignment
        return lhs; // return the result by value (uses move constructor)
    }

    template<unsigned int BitSize>
    number<BitSize> operator-(number<BitSize> lhs, const number<BitSize> &rhs)
    {
        lhs -= rhs; // reuse compound assignment
        return lhs; // return the result by value (uses move constructor)
    }

    template<unsigned int BitSize>
    inline bool operator==(const number<BitSize> &lhs, const number<BitSize> &rhs)
    {
        return false;
    }

    using num32_t = number<32>;
    using num64_t = number<64>;
    using num128_t = number<128>;
    using num256_t = number<256>;
    using num512_t = number<512>;
    using num1024_t = number<1024>;
    using num2048_t = number<2048>;
    using num4096_t = number<4096>;
}

#endif //CRYPTOPALS_NUMBER_H
