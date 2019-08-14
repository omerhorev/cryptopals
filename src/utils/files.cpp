//
// Created by omerh on 25/07/2019.
//

#include <utils/files.h>
#include <istream>
#include <utils/hex.h>
#include <fstream>
#include <algorithm>
#include <string>
#include <utils/base64.h>

size_t utils::files::read_hex_line(std::istream &f, unsigned char o_buffer[], size_t buffer_length)
{
    std::string s;

    std::getline(f, s);

    if (s.length() == 0)
    {
        return 0;
    }

    while (true)
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

size_t utils::files::read_base64(const char *filename, unsigned char *o_buffer, size_t buffer_length)
{
    std::ifstream f;
    f.open(filename, std::fstream::in);

    std::string base64;
    std::string line;

    while (std::getline(f, line))
    {
        base64 += line;
    }

    f.close();

    base64.erase(std::remove(base64.begin(), base64.end(), '\n'), base64.end());
    base64.erase(std::remove(base64.begin(), base64.end(), '\r'), base64.end());

    return base64::decode(base64.c_str(), base64.length(), o_buffer, buffer_length);
}