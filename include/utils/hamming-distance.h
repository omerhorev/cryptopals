//
// Created by omerh on 26/07/2019.
//

#include <cstddef>

namespace utils
{
    class hamming_distance
    {
    public:
        /**
         * Calculate the hamming distance of two buffers
         *
         * @param buffer1 The first buffer
         * @param buffer2 The second buffer
         * @param length  The length of the buffers
         * @return (unsigned int) The hamming distance
         */
        static unsigned int calculate(const unsigned char* buffer1, const unsigned char* buffer2, size_t length);

    private:

        static const unsigned int ones_in_nible[];
    };
}