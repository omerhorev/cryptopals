//
// Created by omerh on 19/12/2019.
//

#ifndef CRYPTOPALS_TEMPBUF_H
#define CRYPTOPALS_TEMPBUF_H

#include <algorithm>

namespace utils
{
    /**
     * tempbuf is a class designed to hold temporary buffers on the stack. The memory is allocated on the ctor
     * and free'd on the dtor to prevent memory leakage.
     *
     */
    struct tempbuf
    {
        /**
         * Creates a new temporary buffer that is a copy of an existing buffer
         */
        static tempbuf copy(const unsigned char buffer[], size_t length)
        {
            tempbuf _(length);
            std::copy_n(buffer, length, _.data);

            return _;
        }

        /**
         * Creates a new temporary buffer that is a copy of an existing buffer
         */
        template<size_t N>
        static tempbuf copy(const unsigned char (&buffer)[N])
        {
            tempbuf _(N);
            std::copy_n(buffer, N, _.data);

            return _;
        }

        static tempbuf create(size_t length)
        {
            tempbuf _(length);
            std::fill_n(_.data, _.length, 0);

            return _;
        }

    private:
        tempbuf(size_t l) : length(l), data(nullptr)
        {
            data = new unsigned char[l];
        }

    public:
        ~tempbuf()
        {
            delete[] data;
        }

        size_t length;
        unsigned char *data;
    };
}

#endif //CRYPTOPALS_TEMPBUF_H
