//
// Created by omerh on 26/07/2019.
//

#ifndef CRYPTOPALS_XOR_H
#define CRYPTOPALS_XOR_H

#include <cstddef>

namespace model
{
    class xor_byte_key
    {
    public:
        /**
         * Decrypts with a single byte xor encryption
         *
         * @param cipher The cipher text to decrypt
         * @param plain  Output. The plain decrypted text
         * @param length The length of the cipher buffer and the plain buffer
         * @param key    The key to use
         */
        static void decrypt(const unsigned char cipher[], unsigned char plain[], size_t length, unsigned char key);

        /**
         * Encrypts with a single byte xor encryption
         *
         * @param plain  The plain buffer to encrypt
         * @param cipher Output. The encrypted cipher
         * @param length The length of the cipher buffer and the plain buffer
         * @param key    The key to use
         */
        static void encrypt(const unsigned char plain[], unsigned char cipher[], size_t length, unsigned char key);

    private:
        static void xor_single_byte_key(const unsigned char src[], unsigned char dst[], size_t length, unsigned char key);
    public:
    };
}

#endif //CRYPTOPALS_XOR_H
