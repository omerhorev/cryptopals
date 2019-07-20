//
// Created by omerh on 13/07/2019.
//

#ifndef CRYPTOPALS_BASE64_H
#define CRYPTOPALS_BASE64_H

#include <cstddef>

/**
 * base64 encoding/decoding according to RFC4648 specifications
 */
class base64
{
public:

    /**
     * Decode a base64 encoded string buffer according to RFC4648
     *
     * @param base64         (input)  The base64 encoded string
     * @param length         (input)  The length of the base64 string
     * @param buffer         (output) The decoded data buffer
     * @param buffer_length  (input)  The decoded data buffer length
     * @return (size_t) The actual length of the decoded data
     */
    static size_t decode(const char base64[], size_t length, unsigned char buffer[], size_t buffer_length);

    /**
     * Encodes data into base64 encoding according to RFC4648
     *
     * @param buffer        (input)  The buffer of data to encode into base64
     * @param length        (input)  The length of the data to encode
     * @param base64        (output) The base64 encoded string of the data
     * @param base64_length (input)  The base64 string buffer size
     * @return (size_t) The length of the base64 encoded string
     */
    static size_t encode(unsigned const char buffer[], size_t length, char base64[], size_t base64_length);

private:

    static unsigned char decode_char(char encoded_char);

    static const size_t chars_in_encoding_group = 4;
    static const size_t bytes_in_encoding_group = 3;
    static const char padd_char;
    static const char encoded_char_table[];
};

#endif //CRYPTOPALS_BASE64_H
