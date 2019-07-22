//
// Created by omerh on 22/07/2019.
//

#ifndef CRYPTOPALS_UTILS_H
#define CRYPTOPALS_UTILS_H

#include <cstddef>
#include <functional>

class utils
{
public:

    /**
     * XOR some buffers and put it in the result
     * The lengths of all the buffers must be equal
     *
     * @param data1         Buffer 1
     * @param length1       The length of the first buffer
     * @param data2         Buffer 2
     * @param length2       The length of the second buffer
     * @param result        The result buffer
     * @param result_length The length of the result
     * @return (size_t) The number of bytes got xor'ed
     */
    static size_t xorBuffers(const unsigned char data1[],
                             size_t length1,
                             const unsigned char data2[],
                             size_t length2,
                             unsigned char result[],
                             size_t result_length);

    /**
     * XOR some buffers and put it in the result
     *
     * @tparam N     The length of all the buffer
     * @param data1  Buffer 1
     * @param data2  Buffer 2
     * @param result The result buffer
     * @return (size_t) The number of bytes got xor'ed
     */
    template<size_t N>
    static size_t xorBuffers(const unsigned char (&data1)[N],
                             const unsigned char (&data2)[N],
                             unsigned char (&result)[N])
    {
        return xorBuffers(data1, N, data2, N, result, N);
    }
};


#endif //CRYPTOPALS_UTILS_H
