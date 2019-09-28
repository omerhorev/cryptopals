//
// Created by omerh on 13/09/2019.
//

#ifndef CRYPTOPALS_SANITATION_H
#define CRYPTOPALS_SANITATION_H

#include <string>
#include <algorithm>

namespace utils
{
    class sanitaion
    {
    public:
        static void encode(std::string &text)
        {
            find_and_replace_all(text, "=", "&--&");
            find_and_replace_all(text, ";", "&.,&");
        }

        static void decode(std::string &text)
        {
            find_and_replace_all(text, "&--&", "=");
            find_and_replace_all(text, "&.,&", ";");
        }

    private:

        static void find_and_replace_all(std::string &str, const std::string &find_str, const std::string &replace_str)
        {
            size_t index = 0;
            while (true)
            {
                /* Locate the substring to replace. */
                index = str.find(find_str, index);
                if (index == std::string::npos) break;

                /* Make the replacement. */
                //str.replace(index, replace_str.length(), replace_str);
                str.erase(index, find_str.length());
                str.insert(index, replace_str);

                /* Advance index forward so the next iteration doesn't pick it up as well. */
                index += replace_str.length();
            }
        }
    };
}

#endif //CRYPTOPALS_SANITATION_H
