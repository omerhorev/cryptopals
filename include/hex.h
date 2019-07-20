//
// Created by omerh on 20/07/2019.
//

#ifndef CRYPTOPALS_HEX_H
#define CRYPTOPALS_HEX_H

#include <cstddef>

class hex
{
public:

    /**
     * Decode an hex encoded string buffer
     *
     * @param hex            (input)  The hex encoded string
     * @param length         (input)  The length of the hex string
     * @param buffer         (output) The decoded data buffer
     * @param buffer_length  (input)  The decoded data buffer length
     * @return (size_t) The actual length of the decoded data
     */
    static size_t decode(const char hex[], size_t length, unsigned char buffer[], size_t buffer_length);

    /**
     * Encodes data into hex encoding
     *
     * @param buffer        (input)  The buffer of data to encode into hex string
     * @param length        (input)  The length of the data to encode
     * @param hex        (output) The hex encoded string of the data
     * @param hex_length (input)  The hex string buffer size
     * @return (size_t) The length of the hex encoded string
     */
    static size_t encode(unsigned const char buffer[], size_t length, char hex[], size_t hex_length);

private:

    static char encode_nible(unsigned int nible)
    {
        if (nible >= 0 && nible < 10) return (unsigned char) ('0' + nible);
        else if (nible >= 10 && nible < 16) return (unsigned char) ('A' + (nible - 10));
        else return (unsigned char)(0xff);
    }

    static unsigned int decode_char(char c)
    {
        if (c >= 'A' && c <= 'Z') return (unsigned int)c - 'A' + 10;
        else if (c >= '0' && c <= '9') return (unsigned int)c - '0';
        return (unsigned char)0xff;
    }
};

#endif //CRYPTOPALS_HEX_H
