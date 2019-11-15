//
// Created by omerh on 11/11/2019.
//

#ifndef CRYPTOPALS_UTILS_BITS_H
#define CRYPTOPALS_UTILS_BITS_H

#include <cstddef>
#include <limits>
#include <climits>

namespace utils
{
    template<class T>
    class bits
    {
    public:
        static constexpr T set(T number, unsigned int index)
        {
            return number | 1 << index;
        }

        static constexpr T clear(T number, unsigned int index)
        {
            return number & ~(1UL << index);
        }

        static constexpr T change(T number, unsigned int index, bool value)
        {
            if (value)
            {
                return set(number, index);
            }
            else
            {
                return clear(number, index);
            }
        }

        static constexpr bool get(T number, unsigned int index)
        {
            return (number & (1UL << index)) != 0;
        }

        static constexpr T fill_from_msb(unsigned int index)
        {
            return ~((1 << (bit_size() - index)) - 1);
        }

    private:
        static constexpr std::size_t bit_size() noexcept
        {
            return sizeof(T) * CHAR_BIT;
        }
    };
}

#endif //CRYPTOPALS_UTILS_BITS_H
