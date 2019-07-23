//
// Created by omerh on 22/07/2019.
//

#ifndef CRYPTOPALS_XOR_BREAK_H
#define CRYPTOPALS_XOR_BREAK_H

#include <cstddef>

namespace breaks
{
    class xor_break
    {
    public:
        /**
         * Breaks the single xor cipher given the cipher is in english alphabet
         *
         * @param cipher        The cipher data
         * @param cipher_length The length of the encrypted data
         * @param o_key         The key extracted
         */
        void single_xor_cipher_break(const unsigned char cipher[],
                                     size_t cipher_length,
                                     char& o_key);

    private:

        void xor_with_key(const unsigned char cipher[], size_t, unsigned char plain[], unsigned char k);

    };
}

#endif //CRYPTOPALS_XOR_BREAK_H
