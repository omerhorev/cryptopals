//
// Created by omerh on 22/07/2019.
//

#include <utils/string-score.h>
#include <algorithm>
#include <array>
#include <numeric>
#include <cmath>

using namespace utils;

float string_score::score_by_frequency(const char string[], size_t length)
{
    float score = 1;
    std::array<float, 0x100> chars_percent = {0};
    std::array<size_t, 0x100> chars_count = {0};

    for (int i = 0; i < length; ++i)
    {
        auto char_index = ((unsigned int) string[i]) % 0xff;
        chars_count[char_index]++;
    }

    for (int j = 0; j < 0x100; ++j)
    {
        auto c = (char) j;
        float predicted_count = get_letter_english_frequency(c) * length;
        float actual_count = chars_count[j];

        if (predicted_count < 0 || predicted_count > length)
        {
            if (actual_count > 0)
            {
                if (!isprint(c) && c != '\n')
                {
                    // Its a not printable char, penalty!
                    score *= std::pow(0.75, actual_count);
                }
                else
                {
                    // Its a printable char without a frequency knowledge, penalty!
                    // Its a not printable char, penalty!
                    score *= std::pow(0.85, actual_count);
                }
            }
        }
        else
        {
            size_t total_letters_with_score = 26 * 2;
            float diff = std::abs(actual_count - predicted_count);
            float relative_diff = 1 - (diff / total_letters_with_score);

            score *= relative_diff;
        }
    }

    return score;
}

float string_score::get_letter_english_frequency(char c)
{
    switch (c)
    {
        case 'a':
        case 'A':
            return 0.0812;
        case 'b':
        case 'B':
            return 0.0149;
        case 'c':
        case 'C':
            return 0.0271;
        case 'd':
        case 'D':
            return 0.0432;
        case 'e':
        case 'E':
            return 0.1202;
        case 'f':
        case 'F':
            return 0.0230;
        case 'g':
        case 'G':
            return 0.0203;
        case 'h':
        case 'H':
            return 0.0592;
        case 'i':
        case 'I':
            return 0.0731;
        case 'j':
        case 'J':
            return 0.0010;
        case 'k':
        case 'K':
            return 0.0069;
        case 'l':
        case 'L':
            return 0.0398;
        case 'm':
        case 'M':
            return 0.0261;
        case 'n':
        case 'N':
            return 0.0695;
        case 'o':
        case 'O':
            return 0.0768;
        case 'p':
        case 'P':
            return 0.0182;
        case 'q':
        case 'Q':
            return 0.0011;
        case 'r':
        case 'R':
            return 0.0602;
        case 's':
        case 'S':
            return 0.0628;
        case 't':
        case 'T':
            return 0.0910;
        case 'u':
        case 'U':
            return 0.0288;
        case 'v':
        case 'V':
            return 0.0111;
        case 'w':
        case 'W':
            return 0.0209;
        case 'x':
        case 'X':
            return 0.0017;
        case 'y':
        case 'Y':
            return 0.0211;
        case 'z':
        case 'Z':
            return 0.0007;
        case ' ':
            return 0.2;
        case '\'':;
            return 0.01;
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
        case '0':
            return 0.01;
        default:
            return -1;
    }
}

