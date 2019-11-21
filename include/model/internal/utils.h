//
// Created by omerh on 24/10/2019.
//

#ifndef CRYPTOPALS_MODEL_UTILS_H
#define CRYPTOPALS_MODEL_UTILS_H

#include <cstddef>
#include <array>
#include <type_traits>
#include <limits>

namespace model
{
    namespace internal
    {
        enum class endianness
        {
            little,
            big,
        };

        template<typename T, model::internal::endianness Endianness>
        struct endianness_encoder
        {
            static void encode(T t, unsigned char (&buffer)[sizeof(T)]);
        };

        template<typename T>
        struct endianness_encoder<T, endianness::little>
        {
            static_assert(std::is_integral<T>::value, "T must be an integral type");

            typedef unsigned char(&T_sized_buffer_reference_t)[sizeof(T)];

            /**
             * Encode the the object provided to a byte array using little endian.
             * @param t      The object to encode
             * @param buffer The buffer to encode into, must be in the size of the object.
             */
            static void encode(T t, T_sized_buffer_reference_t buffer)
            {
                for (unsigned char &i : buffer)
                {
                    auto first_byte = t & 0xff;
                    i = static_cast<unsigned char>(first_byte);
                    t >>= 8;
                }
            }
        };

        template<typename T>
        struct endianness_encoder<T, endianness::big>
        {
            static_assert(std::is_integral<T>::value, "T must be an integral type");

            typedef unsigned char(&T_sized_buffer_reference_t)[sizeof(T)];

            /**
             * Encode the the object provided to a byte array using big endian.
             * @param t      The object to encode
             * @param buffer The buffer to encode into, must be in the size of the object.
             */
            static void encode(T t, unsigned char (&buffer)[sizeof(T)])
            {
                typedef std::array<unsigned char, sizeof(buffer)> array_t;
                array_t &arr = *((array_t *) buffer);

                for (auto i = arr.rbegin(); i != arr.rend(); ++i)
                {
                    auto first_byte = t & 0xff;
                    *i = static_cast<unsigned char>(first_byte);
                    t >>= CHAR_BIT;
                }
            }

            static T decode(unsigned char (&buffer)[sizeof(T)])
            {
                T val = 0;

                for (auto i = 0; i < sizeof(T); i++)
                {
                    val <<= CHAR_BIT;
                    val |= buffer[i];
                }

                return val;
            }
        };

        template<class T>
        static T circular_left_shift(T value, unsigned int offset)
        {
            const size_t size_in_bits = sizeof(T) * CHAR_BIT;

            return (value << offset) | (value >> (size_in_bits - offset));
        }
    }
}

#endif //CRYPTOPALS_MODEL_UTILS_H
