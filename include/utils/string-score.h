//
// Created by omerh on 22/07/2019.
//

#ifndef CRYPTOPALS_STRING_SCORE_H
#define CRYPTOPALS_STRING_SCORE_H

#include <cstddef>

namespace utils
{
    class string_score
    {
    public:
        /**
         * Scores a string in the english alphabet by its characters frequency likelihood. The longer the string
         * the better result this method will make.
         *
         * The scoring is between 0 to 1
         *
         * @param string The string to score
         * @param length The length of the string
         * @return (float) Representing how likely is the string to appear in the english language
         */
        static float score_by_frequency(const char string[], size_t length);

        /**
         * Returns the expected frequency of a specific letter in a general english text. Used for scoring strings
         * @param c The char to check
         * @return Its frequency.
         */
        static float get_letter_english_frequency(char c);
    private:
        static const size_t number_of_chars_in_english = 26;
    };
}

#endif //CRYPTOPALS_STRING_SCORE_H
