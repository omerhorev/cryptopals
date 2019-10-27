//
// Created by omerh on 24/10/2019.
//

#ifndef CRYPTOPALS_MODEL_UTILS_H
#define CRYPTOPALS_MODEL_UTILS_H

#include <cstddef>

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
            static void encode(T t, unsigned char (&buffer)[sizeof(T)])
            {
                for (unsigned char &i : buffer)
                {
                    auto first_byte = t & 0xff;
                    i = static_cast<unsigned char>(first_byte);
                    t <<= 8;
                }
            }
        };
    }
}

#endif //CRYPTOPALS_MODEL_UTILS_H
