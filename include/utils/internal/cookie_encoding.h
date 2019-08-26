//
// Created by omerh on 25/08/2019.
//

#ifndef CRYPTOPALS_COOKIE_ENCODING_H
#define CRYPTOPALS_COOKIE_ENCODING_H

#include <string>
#include <sstream>
#include <regex>

namespace utils
{
    namespace internal
    {
        class encode_cookie_base
        {
        protected:
            encode_cookie_base() : _first_print(true)
            {}

            /**
             * WARNING: UNSAFE METHOD, written only for rapid development.
             *
             * @tparam T    The type of data to encode
             * @param name  The name of the field
             * @param val   The value of the field
             * @return      The string of the field including seperator.
             */
            template<typename T>
            std::string encode_field(const std::string &name, T val)
            {
                std::stringstream ss;

                if (!_first_print)
                {
                    ss << "&";
                }

                ss << name << "=" << val;

                _first_print = false;

                return ss.str();
            }

            /**
             * Decodes a field in the form name="value"
             *
             * @tparam T      The type of the data to decode
             * @param encoded The encoded string (containing all the fields)
             * @param name    The name of the field to decode
             * @param value   (output) The object to fill
             */
            template<typename T>
            void decode_field(const std::string &encoded, const std::string &name, T &value)
            {
                auto pos = encoded.find(name + "=");

                pos += (name + "=").length();

                auto end_pos = encoded.find('&', pos);

                if (pos == std::string::npos)
                {
                    auto content = encoded.substr(pos);
                    devalue(value, content);
                }
                else
                {
                    auto length = end_pos - pos;
                    auto content = encoded.substr(pos, length);
                    devalue(value, content);
                }
            }

            /**
             * Decodes a value. General implementation.
             *
             * @tparam T      The type of the value decode into.
             * @param value   (output) Where to put the decoded value
             * @param content the string represented value to decode
             */
            template<typename T>
            void devalue(T &value, const std::string &content)
            {
                std::stringstream ss;

                ss << content;
                ss >> value;
            }

            /**
             * Decodes a string value. Special implementation for strings
             *
             * @tparam N      The length of the string array to decode into.
             * @param value   (output) Where to put the decodede value
             * @param content the string represented value to decode
             */
            template<size_t N>
            void devalue(char (&value)[N], const std::string &content)
            {
                if (content.length() >= N)
                {
                    return;
                }

                std::string temp = content;

                temp = regex_replace(temp, std::regex("\\&"), "\\&");

                std::copy(temp.cbegin(), temp.cend(), value);
                value[content.length()] = 0;
            }

        private:
            bool _first_print;
        };
    }

}

#endif //CRYPTOPALS_COOKIE_ENCODING_H
