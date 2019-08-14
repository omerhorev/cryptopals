//
// Created by omerh on 25/07/2019.
//

#ifndef CRYPTOPALS_FILES_H
#define CRYPTOPALS_FILES_H

#include <cstddef>
#include <istream>

namespace utils
{
    class files
    {
    public:
        /**
         * Reads from a file an hex encoded buffer until end of line.
         *
         * @param f             The fd to read from
         * @param o_buffer      The buffer to fill
         * @param buffer_length The length of the buffer to fill
         * @return (size_t) The actual length of the buffer read
         */
        static size_t read_hex_line(std::istream& f, unsigned char o_buffer[], size_t buffer_length);

        /**
         * Reads from a file an hex encoded buffer until end of line.
         *
         * @param filename      The filename to read from
         * @param buffer_length The length of the buffer to fill
         * @return (size_t) The actual length of the buffer read
         */
        static size_t read_base64(const char filename[], unsigned char o_buffer[], size_t buffer_length);
    };
}

#endif //CRYPTOPALS_FILES_H
