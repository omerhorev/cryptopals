//
// Created by omerh on 25/07/2019.
//

#include <utils/files.h>
#include <istream>
#include <utils/hex.h>

size_t utils::files::read_hex_line(std::istream& f, unsigned char o_buffer[], size_t buffer_length)
{
    std::string s;

    std::getline(f, s);

    if (s.length() == 0)
    {
        return 0;
    }

    while(true)
    {
        auto last = s[s.length() - 1];
        if (last != '\r' && last != ' ' && last != '\n')
        {
            break;
        }

        s = s.substr(0, s.length() - 1);
    }

    return utils::hex::decode(s.c_str(), s.length(), o_buffer, buffer_length);
}

