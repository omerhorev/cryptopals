//
// Created by omerh on 25/07/2019.
//

#ifndef CRYPTOPALS_FILES_H
#define CRYPTOPALS_FILES_H

#include <cstddef>
#include <istream>
#include <fstream>
#include <cstring>
#include <algorithm>
#include "hex.h"

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
         * Reads from a file multiple lines of hex encoded data in the same size.
         *
         * @tparam MaxLines      The maximum amount of lines to read
         * @tparam LineHexSize   The exact length of the the DECODED DATA to read (not the string itself)
         * @param filename       The name of the file to read from
         * @param o_buffer       (output) The buffer of decoded data
         * @return (size_t) The number of lines read
         */
        template <size_t MaxLines, size_t LineHexSize>
        static size_t read_hex_lines(const char filename[], unsigned char (&o_buffer)[MaxLines][LineHexSize])
        {
            std::ifstream f;
            f.open(filename, std::fstream::in);
            std::string line;
            size_t write_index = 0;

            while (std::getline(f, line))
            {
                line.erase(std::remove(line.begin(), line.end(), '\n'), line.end());
                line.erase(std::remove(line.begin(), line.end(), '\r'), line.end());

                if (line.length() != (LineHexSize * 2))
                {
                    break;
                }

                hex::decode(line.c_str(), size_t(line.length()), (unsigned char*)&o_buffer[write_index], LineHexSize);
                write_index += 1;
            }

            f.close();

            return write_index;
        }

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
