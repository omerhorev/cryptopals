//
// Created by omerh on 24/10/2019.
//

#ifndef CRYPTOPALS_MODEL_UTILS_H
#define CRYPTOPALS_MODEL_UTILS_H

#include <cstddef>
#include <array>

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
        struct encoder
        {
            static void encode(T t, unsigned char (&buffer)[sizeof(T)]);
        };

        template<typename T>
        struct encoder<T, endianness::little>
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
        struct encoder<T, endianness::big>
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
                    t >>= 8;
                }
            }
        };
    }
}

#endif //CRYPTOPALS_MODEL_UTILS_H
