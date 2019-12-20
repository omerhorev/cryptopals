//
// Created by omerh on 04/12/2019.
//

#ifndef CRYPTOPALS_Z_H
#define CRYPTOPALS_Z_H

#include <cstddef>
#include <iosfwd>

namespace math
{
    namespace internal
    {
        /**
         * This class holds operations that can be done on big numbers including some of the modular arithmetic.
         * The implementations here are far from being optimal, but are safe and better than the trivial ones.
         */
        class bignum
        {
        public:
            /**
             * Sets the value of a specific number from another number
             *
             * @param number The number to set
             * @param length The length of the number
             * @param value  The value to set to
             * @param value_length The length of the value
             */
            static void set(unsigned char *number, size_t length, const unsigned char *value, size_t value_length);

            /**
             * Adds a specific number to another number
             *
             * The length of the number to add to must be bigger or equal to the length of the number added.
             *
             * Done in O(n)
             *
             * @param number The number to add to
             * @param length The length of the number to add to
             * @param value  The value to add to the number
             * @param value_length The length of the number to add to the number
             */
            static void add(unsigned char *number, size_t length, const unsigned char *value, size_t value_length);

            /**
             * Adds a number to another modulus N
             *
             * @param number The number to add to
             * @param length The length of the number
             * @param value  The number to add
             * @param value_length The length of the number to add
             * @param N The modulus
             * @param N_length The length of the modulus
             */
            static void modadd(unsigned char *number, size_t length,
                               const unsigned char *value, size_t value_length,
                               const unsigned char *N, size_t N_length);

            /**
             * Subtracts a specific number from another
             *
             * Done in O(n)
             *
             * @param number The number to subtract from
             * @param length The length of the number to subtract from
             * @param value  The number to subtract from the number
             * @param value_length The length of the number to subtract
             */
            static void subtract(unsigned char *number, size_t length, const unsigned char *value, size_t value_length);

            /**
             * Compares two number.
             *
             * Done in O(max(n,m))
             *
             * @param first         The first number
             * @param first_length  The first number length
             * @param second        The second number
             * @param second_length The second number length
             * @return 0 if equal | <0 if the first number is smaller | >0 if the first number is bigger
             */
            static int compare(const unsigned char *first, size_t first_length,
                               const unsigned char *second, size_t second_length);

            /**
             * Calculate the modulus of a number inplace.
             *
             * @note This is not implemented in an cpu efficient way...
             *
             * @param number   The number to calculate the modulus to and put the result in
             * @param length   The length of the number
             * @param N        The modulus to calculate by
             * @param N_length The length of the modulus
             */
            static void mod(unsigned char number[], size_t length, const unsigned char N[], size_t N_length);

            /**
             * Performs shift left on the number
             *
             * @param number The number to shift
             * @param length The length of the number to shift
             * @param count  The number of bits to shift to the left
             */
            static void shift_left(unsigned char number[], size_t length, unsigned int count);


            /**
             * Performs shift right on the number
             *
             * @param number The number to shift
             * @param length The length of the number to shift
             * @param count  The number of bits to shift to the right
             */
            static void shift_right(unsigned char number[], size_t length, unsigned int count);

            /**
             * Modular power. Calculates the 'number' raise to the power of 'exponent' modulus 'N' and puts it in the
             * 'number' var.
             *
             * @param number           The number to raise
             * @param number_length    The length of the number
             * @param exponent         The exponent to raise to
             * @param exponent_length  The length of the exponent
             * @param N                The modulus
             * @param N_length         The length of the modulus
             */
            static void modpow(unsigned char number[], size_t number_length,
                               const unsigned char exponent[], size_t exponent_length,
                               const unsigned char N[], size_t N_length);

            /**
             * Modular power. Calculates the 'number' raise to the power of 'exponent' modulus 'N'.
             *
             * @param number           The number to raise
             * @param number_length    The length of the number
             * @param exponent         The exponent to raise to
             * @param exponent_length  The length of the exponent
             * @param N                The modulus
             * @param N_length         The length of the modulus
             * @param result           The result
             * @param result_length    The length of the result
             */
            static void modpow(unsigned char base[], size_t base_length,
                               unsigned char exponent[], size_t exponent_length,
                               const unsigned char N[], size_t N_length,
                               unsigned char result[], size_t result_length);

            static void modmul(unsigned char number[], size_t length,
                               const unsigned char value[], size_t value_length,
                               const unsigned char N[], size_t N_length);

            static void modsquare_unsafe(unsigned char number[], size_t length, const unsigned char N[],
                                         size_t N_length);

            static bool is_odd(const unsigned char number[], size_t length);

            static bool is_even(const unsigned char number[], size_t length);

            static bool is_empty(const unsigned char number[], size_t length);

            static void decrease(unsigned char number[], size_t length);

            static void increase(unsigned char number[], size_t length);

            static std::ostream &stream_out(std::ostream &ostream, const unsigned char number[], size_t length);

        private:

            /**
             * Unsafe version of the modmul without the checks, used for internal needs only
             */
            static void modmul_unsafe(unsigned char number[], size_t length,
                                      const unsigned char value[], size_t value_length,
                                      const unsigned char N[], size_t N_length);


            static void modmul_internal(unsigned char a[], size_t a_length,
                                        unsigned char b[], size_t b_length,
                                        const unsigned char N[], size_t N_length);

            static size_t minimum_required_length(const unsigned char number[], size_t length);
        };
    }
}

#endif //CRYPTOPALS_Z_H
