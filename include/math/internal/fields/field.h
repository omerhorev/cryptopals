//
// Created by omerh on 04/12/2019.
//

#ifndef CRYPTOPALS_FIELD_H
#define CRYPTOPALS_FIELD_H

#include <cstddef>

namespace math
{
    namespace internal
    {
        namespace fields
        {
            //
            // The field defines a few mathematical properties and actions that can be preformed on a number
            // each number, has a field.
            //
            // Because this class defines the very basic operations, all the numbers represented by an array of numbers
            // represented as "big endian".
            //
            class field
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
                virtual void set(unsigned char *number, size_t length,
                                 const unsigned char *value, size_t value_length) = 0;

                /**
                 * Adds a specific number to another number
                 *
                 * The length of the number to add to must be bigger or equal to the length of the number added.
                 *
                 * @param number The number to add to
                 * @param length The length of the number to add to
                 * @param value  The value to add to the number
                 * @param value_length The length of the number to add to the number
                 */
                virtual void add(unsigned char *number, size_t length,
                                 const unsigned char *value, size_t value_length) = 0;

                /**
                 * Subtracts a specific number from another
                 *
                 * @param number The number to subtract from
                 * @param length The length of the number to subtract from
                 * @param value  The number to subtract from the number
                 * @param value_length The length of the number to subtract
                 */
                virtual void subtract(unsigned char *number, size_t length,
                                      const unsigned char *value, size_t value_length) = 0;

                /**
                 * Compares two number
                 *
                 * @param first         The first number
                 * @param first_length  The first number length
                 * @param second        The second number
                 * @param second_length The second number length
                 * @return 0 if equal | <0 if the first number is smaller | >0 if the first number is bigger
                 */
                virtual int compare(const unsigned char *first, size_t first_length,
                                    const unsigned char *second, size_t second_length) = 0;
            };
        }
    }
};

#endif //CRYPTOPALS_FIELD_H
