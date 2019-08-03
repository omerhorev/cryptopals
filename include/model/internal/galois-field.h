//
// Created by omerh on 01/08/2019.
//

#ifndef CRYPTOPALS_GALOIS_INT_H
#define CRYPTOPALS_GALOIS_INT_H

namespace model
{
    namespace internal
    {
        class galois
        {
        public:

            /**
             * Perform a multiplication in the galois field
             * @param a The first number
             * @param b The second number
             * @return (unsigned int) The result
             */
            static auto mul(unsigned char a, unsigned char b)
            {
                unsigned char p = 0;

                for (int i = 0; i < 8; ++i)
                {
                    if ((b & 0x01) != 0)
                    {
                        p ^= a;
                    }

                    b >>= 1;

                    auto carry = (a & 0x80) != 0;

                    a <<= 1;

                    if (carry)
                    {
                        a ^= 0x1b;
                    }
                }

                return p;
            }

            /**
             * Raise to 2 to the exponent of a, in a finite field 2^8
             * @param a The number to raise 2 to the exponent to
             * @return (unsigned char) The result.
             */
            static unsigned char exponent2(unsigned char a)
            {
                unsigned char p = 1;

                for (int i = 0; i < a; ++i)
                {
                    p = mul(p, 2);
                }

                return p;
            }
        };
    }
}

#endif //CRYPTOPALS_GALOIS_INT_H
