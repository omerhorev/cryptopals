//
// Created by omerh on 01/08/2019.
//

#ifndef CRYPTOPALS_GALOIS_INT_H
#define CRYPTOPALS_GALOIS_INT_H

namespace model
{
    namespace internal
    {
        /**
         * Galois field with p=2 and q= 8 (256)
         */
        class galois256
        {
        public:

            /**
             * Initializes a new instance of the galois256 field with an initial value
             *
             * @param v The initial value
             */
            explicit galois256(unsigned char v) : _value(v)
            {}

            /**
             * Adds a number to the this number and return a new one that is the result of the operation.
             *
             * @param v The number to add
             * @return (galois256) a new number that is this number added to the number provided
             */
            galois256 operator+(galois256 v) const;

            /**
             * Multiplies a number to this number and return a new one that is the result of the operation.
             *
             * @param v The number to add
             * @return (galois256) a new number that is this number multiplied to the number provided
             */
            galois256 operator*(galois256 v) const;

            /**
             * Returns the power of this number and the number provided
             * @param v The number to power
             * @return (galois256) a nenw number that is the power of this number and the number provided
             */
            galois256 operator^(galois256 v) const;

            /**
             * Changes the value of this number to the number provided
             *
             * @param v The number to change the value to
             * @return (galois256&) This value after the value change.
             */
            galois256 &operator=(galois256 v);

            /**
             * Adds a number to this number and change this number.
             *
             * @param v The number to add.
             * @return (galois256&) This value after the value change.
             */
            galois256 &operator+=(galois256 v);

            /**
             * Compares two numbers, returns true if equal and false otherwise.
             *
             * @param v The number to compare to.
             * @return (bool) True if equal, False otherwise.
             */
            bool operator==(galois256 v) const;

            /**
             * Adds one to the number provided.
             * @return (galois256) The number before the addition.
             */
            galois256 operator++();

            /**
             * Adds one to the number provided.
             * @return (galois256) The number before the addition.
             */
            const galois256 operator++(int);


            /**
             * Converts this type to unsigned char with the real value of the field converted to int.
             */
            explicit operator unsigned char();

        private:
            unsigned char _value;
        };
    }
}

#endif //CRYPTOPALS_GALOIS_INT_H
