//
// Created by omerh on 01/12/2019.
//

#ifndef CRYPTOPALS_NUMBER_H
#define CRYPTOPALS_NUMBER_H

#include <cstddef>
#include <limits>
#include <math/internal/fields/field.h>
#include <model/internal/utils.h>

namespace math
{
    namespace internal
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
            number(fields::field *field) : number(_field, 0)
            {}

            /**
             * Initialize a new number with an integer value
             *
             * @param value The first value
             */
            number(fields::field *field, unsigned int value) : _field(field)
            {
                auto v = generate_be_uint_buffer(value);

                _field->set(_raw, bytes_count(), v.data(), v.size());
            }

            /**
             * Increment the value of the number by 1
             *
             * @return The number after the incrementation. =
             */
            number &operator++()
            {
                operator+=(1);
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
                operator-=(1);

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
                _field->add(_raw, bytes_count(), rhs._raw, rhs.bytes_count());
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

                _field->add(_raw, bytes_count(), v.data(), v.size());
                return *this;
            }

            /**
             * Subtract from this number another number
             * @param rhs The number to substract
             * @return The result
             */
            number &operator-=(const number &rhs)
            {
                _field->subtract(_raw, bytes_count(), rhs._raw, rhs.bytes_count());
                return *this;
            }

            /**
             * Subtract from this number another integer
             * @param rhs The integer to substract
             * @return The result
             */
            number &operator-=(unsigned int rhs)
            {
                auto v = generate_be_uint_buffer(rhs);

                _field->subtract(_raw, bytes_count(), v.data(), v.size());

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
                return _field->compare(_raw, bytes_count(), rhs._raw, rhs.bytes_count()) == 0;
            }

            /**
             * Returns whether the number is equal to an integer
             * @param value The integer to compare to
             * @return Whether the number is equal to the integer provided
             */
            bool operator==(unsigned int value) const
            {
                auto v = generate_be_uint_buffer(value);
                return _field->compare(_raw, bytes_count(), v.data(), v.size()) == 0;
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

            fields::field *_field;
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
    }
}

#endif //CRYPTOPALS_NUMBER_H
