//
// Created by omerh on 25/08/2019.
//

#ifndef CRYPTOPALS_COOKIE_H
#define CRYPTOPALS_COOKIE_H

#include <string>
#include "utils/internal/cookie_encoding.h"

namespace utils
{
    template<typename T>
    class cookie_encoder {};

    /**
     * Encodes an object as a cookie representation:
     *
     *  field1="value1"&field2="value2"&...="..."&fieldN="valueN"
     *
     * The data type must be supported (The utils::cookie_encoder must be implemented for this type)
     *
     * @tparam T   The type of the data to encode.
     * @param val  The data to encode
     * @return (std::string) representing the data.
     */
    template<typename T>
    std::string encode_as_cookie(const T &val)
    {
        cookie_encoder<T> encoder;

        return encoder.encode(val);
    }

    /**
     * Decodes an object from a cookie represented string
     *
     *  field1="value1"&field2="value2"&...="..."&fieldN="valueN"
     *
     * The data type must be supported (The utils::cookie_encoder must be implemented for this type)
     *
     * @tparam T  The type of the object to decode
     * @param str The encoded object
     * @param val (output) The object to put the data into
     */
    template<typename T>
    void decode_as_cookie(const std::string &str, T &val)
    {
        cookie_encoder<T> decoder;

        decoder.decode(str, val);
    }
}

#endif //CRYPTOPALS_COOKIE_H
