//
// Created by omerh on 01/12/2019.
//

#ifndef CRYPTOPALS_NUMBER_H
#define CRYPTOPALS_NUMBER_H

#include <cstddef>
#include <limits>
#include <math/internal/bignum.h>
#include <model/internal/utils.h>

namespace math
{
    template<unsigned int BitSize>
    class number
    {
        static_assert(BitSize % 8 == 0, "only bits count that are multiplicand of 8 are supported");

    private:
        static constexpr unsigned int BytesCount = BitSize / CHAR_BIT;

    public:

        /**
         * Initialize a new number with value of 0
         */
        number() : number(0)
        {}

        /**
         * Initialize a new number with an integer value
         *
         * @param value The first value
         */
        number(unsigned int value)
        {
            auto v = generate_be_uint_buffer(value);

            math::internal::bignum::set(_raw, bytes_count(), v.data(), v.size());
        }

        /**
         * Copy constructor. Copies the number from a different one
         * @param other The over number to copy from
         */
        number(const number &other)
        {
            math::internal::bignum::set(_raw, bytes_count(), other._raw, other.bytes_count());
        }

        /**
         * Copy constructor from a right value reference.
         * @param other The number to copy from
         */
        number(const number &&other)
        {
            math::internal::bignum::set(_raw, bytes_count(), other._raw, other.bytes_count());
        }

        /**
         * Copy constructor from a left value reference.
         * @param other The number to copy from
         */
        number &operator=(const number &other)
        {
            math::internal::bignum::set(_raw, bytes_count(), other._raw, other.bytes_count());
            return *this;
        }

        /**
         * Copy constructor from a right value reference.
         * @param other The number to copy from
         */
        number &operator=(number &&other) noexcept
        {
            math::internal::bignum::set(_raw, bytes_count(), other._raw, other.bytes_count());
            return *this;
        }

        /**
         * Assigns a integer value to a number
         *
         * @param value The value to assign
         * @return a reference to the object assigned to.
         */
        number &operator=(unsigned int value) noexcept
        {
            unsigned char v_raw[sizeof(value)];


            math::internal::bignum::set(_raw, bytes_count(), v_raw, sizeof(v_raw));
        }

        /**
         * Increment the value of the number by 1
         *
         * @return The number after the incrementation. =
         */
        number &operator++()
        {
            unsigned char num = 1;

            math::internal::bignum::add(_raw, bytes_count(), &num, sizeof(unsigned char));

            return *this;
        }

        /**
         * Increment the value of the number by 1
         *
         * @return The number before the incrementation
         */
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

        /**
         * Adds a number to this number
         *
         * @param rhs The number to add
         * @return This number after the addition
         */
        number &operator+=(const number &rhs)
        {
            math::internal::bignum::add(_raw, bytes_count(), rhs._raw, rhs.bytes_count());
            return *this;
        }

        /**
         * Adds an integer to this number
         *
         * @param rhs The integer to add
         * @return This number after the addition
         */
        number &operator+=(unsigned int rhs)
        {
            auto v = generate_be_uint_buffer(rhs);

            math::internal::bignum::add(_raw, bytes_count(), v.data(), v.size());
            return *this;
        }

        number &operator-=(const number &rhs)
        {
            /* addition of rhs to *this takes place here */
            return *this;
        }

        number &operator-=(unsigned int &rhs)
        {
            /* addition of rhs to *this takes place here */
            return *this;
        }

        /**
         * Returns whether two numbers are equal
         *
         * @param rhs The second number
         * @return Whether the two numbers are equal
         */
        bool operator==(const number<BitSize> &rhs) const
        {
            return math::internal::bignum::equal(_raw, bytes_count(), rhs._raw, rhs.bytes_count());
        }

        /**
         * Returns whether the number is equal to an integer
         * @param value The integer to compare to
         * @return Whether the number is equal to the integer provided
         */
        bool operator==(unsigned int value) const
        {
            auto v = generate_be_uint_buffer(value);
            return math::internal::bignum::equal(_raw, bytes_count(), v.data(), v.size());
        }

    private:
        template<class T>
        static auto generate_be_uint_buffer(T value)
        {
            unsigned char _[sizeof(T)];

            model::internal::endianness_encoder<unsigned int, model::internal::endianness::big>::encode(value, _);

            return *((std::array<unsigned char, sizeof(T)> *) _);
        }

    private:

        size_t bytes_count() const
        { return BytesCount; }

    private:
        unsigned char _raw[BytesCount];
    };

    /**
     * Adds a type (can be anything supported) to the number provided
     *
     * @tparam BitSize The size in bits of the number provided
     * @tparam T       The type of the object (currently only same-sized numbers and ints are supported) to add
     * @param lhs      The number to add to (not a reference for better optimizations)
     * @param rhs      The object to add to the number
     * @return The new number after the addition
     */
    template<unsigned int BitSize, class T>
    number<BitSize> operator+(number<BitSize> lhs, const T &rhs)
    {
        lhs += rhs;
        return lhs; // return the result by value (uses move constructor)
    }

    /**
     * Adds a type (can be anything supported) to the number provided
     *
     * @tparam BitSize The size in bits of the number provided
     * @tparam T       The type of the object (currently only same-sized numbers and ints are supported) to add
     * @param lhs      The number to add to (not a reference for better optimizations)
     * @param rhs      The object to add to the number
     * @return The new number after the addition
     */
    template<unsigned int BitSize, class T>
    number<BitSize> operator+(number<BitSize> lhs, const T &&rhs)
    {
        lhs += rhs; // reuse compound assignment
        return lhs; // return the result by value (uses move constructor)
    }

    template<unsigned int BitSize, class T>
    number<BitSize> operator-(number<BitSize> lhs, const T &rhs)
    {
        lhs -= rhs; // reuse compound assignment
        return lhs; // return the result by value (uses move constructor)
    }

    template<unsigned int BitSize, class T>
    number<BitSize> operator-(number<BitSize> lhs, const T &&rhs)
    {
        lhs -= rhs; // reuse compound assignment
        return lhs; // return the result by value (uses move constructor)
    }

    template<unsigned int BitSize>
    inline bool operator==(unsigned int lhs, const number<BitSize> &rhs)
    {
        return rhs == lhs;
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
