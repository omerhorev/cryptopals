//
// Created by omerh on 27/09/2019.
//

#include <cstddef>

#ifndef CRYPTOPALS_BITFLIPPING_H
#define CRYPTOPALS_BITFLIPPING_H

#endif //CRYPTOPALS_BITFLIPPING_H

namespace breaks
{
    class bitflippig
    {
    public:

        /**
         * We know the message starts with 'comment1=cooking MCs;userdata=";' (32 bytes - two blocks)
         * Meaning the data we add is exactly aligned to block size.
         *
         * The data added by us is two blocks and was
         *          [    block 1   ][    block 2   ]
         *          0000000000000000admin<1000000000
         *
         * The char '=' in bits (ascii) is '00111101' (0x3D), so instead of '=' we put '<' in the message which is
         * '00111100' (0x3D).
         *
         * Because the '<' (or the '=') is in the 6th char and (0x3D | 0x3C) = 1 we need to
         * flip the LSB bit in the 6th byte in the 3rd block t to achieve the same flip in the 4th block
         *
         * @param buffer The encrypted data as specified in the description
         * @param length The length of the buffer, aligned to block size
         * @param block_size The size of the block
         */
        static void do_flip(unsigned char *buffer, size_t length, size_t block_size);
    };
}