//
// Created by omerh on 29/10/2019.
//

#ifndef CRYPTOPALS_EXCEPTIONS_H
#define CRYPTOPALS_EXCEPTIONS_H

#include <exception>
#include <sstream>

namespace model
{
    struct invalid_key : public std::exception
    {
        /**
         * Initializes an invalid key exception
         *
         * @param key    The key buffer provided.
         * @param length The length of the key buffer provided.
         */
        invalid_key(const unsigned char key[], size_t length) : _key(key), _length(length)
        {}

        const char *what() const noexcept override
        {
            std::ostringstream _("");
            _ << "invalid key provided (key:" << (void *) _key << ", length: " << _length << ")";
            return _.str().c_str();
        }

        const unsigned char *_key;
        size_t _length;
    };

    struct pkcs7_invalid_padding : public std::exception
    {
        /**
         * Initializes a general pkcs7 padding error.
         */
        pkcs7_invalid_padding() : pad_index_error(-1), invalid_pad_byte(0xff)
        {}

        /**
         * Initializes a pkcs7 error with arguemtns to print
         *
         * @param pad_index_error  The index in the buffer that the unexpected pad byte occurred
         * @param invalid_pad_byte The actual byte found in the buffer instead the correct byte (the pad index...)
         */
        explicit pkcs7_invalid_padding(int pad_index_error, unsigned char invalid_pad_byte) :
                pad_index_error(pad_index_error), invalid_pad_byte(invalid_pad_byte)
        {}

        const char *what() const noexcept override
        {
            return "invalid padding";
        }

        const int pad_index_error;
        const unsigned char invalid_pad_byte;
    };

    struct internal_error : public std::exception
    {
        explicit internal_error(const std::string &message) : message(message)
        {}

        const char *what() const noexcept override
        {
            return message.c_str();
        }

        const std::string message;
    };

}

#endif //CRYPTOPALS_EXCEPTIONS_H
