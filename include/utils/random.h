//
// Created by omerh on 29/09/2019.
//

#ifndef CRYPTOPALS_RANDOM_H
#define CRYPTOPALS_RANDOM_H

#include <cstddef>
#include <random>
#include <algorithm>

namespace utils
{
    class random
    {
    public:
        static random &instance()
        {
            static random _;
            return _;
        }

        template<typename T>
        T get()
        {
            return get<T>(std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        }

        template<typename T>
        T get(T min, T max)
        {
            std::uniform_int_distribution<T> _(min, max);
            return _(_rd);
        }

        template<typename T, size_t N>
        void fill(T (&arr)[N])
        {
            return fill(arr, N);
        }

        template<typename T>
        void fill(T arr[], size_t length)
        {
            return fill<T>(arr, length, std::numeric_limits<T>::min(), std::numeric_limits<T>::max());
        }

        template<typename T, size_t N>
        void fill(T (&arr)[N], T min, T max)
        {
            return fill(arr, N, min, max);
        }

        template<typename T>
        void fill(T arr[], size_t length, T min, T max)
        {
            std::uniform_int_distribution<T> _(min, max);

            std::generate_n(arr, length, [&]() {
                return _(_rd);
            });
        }

    private:

        std::random_device _rd;
    };
}

#endif //CRYPTOPALS_RANDOM_H
