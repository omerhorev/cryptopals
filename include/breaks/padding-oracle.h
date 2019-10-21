#include <utility>

//
// Created by omerh on 20/10/2019.
//

#ifndef CRYPTOPALS_PADDING_ORACLE_H
#define CRYPTOPALS_PADDING_ORACLE_H

#include <cstddef>
#include <functional>
#include <algorithm>
#include <vector>
#include <breaks/internal/ecb-block-parser.h>

namespace breaks
{
    template<size_t BlockSize>
    class padding_oracle
    {
    public:
        typedef std::function<bool(const unsigned char *, size_t, size_t)> check_padding_function_t;

        /**
         * Initialize a new instance of a padding oracle.
         *
         * @param cipher         The cipher to try to decrypt
         * @param length         The length of the cipher to decrypt
         * @param padding_oracle a function used to determine whether a ciphertext has a valid padding
         */
        explicit padding_oracle(const unsigned char cipher[],
                                size_t length,
                                check_padding_function_t padding_oracle)
                : _padding_oracle(std::move(padding_oracle)), _cipher(nullptr)
        {
            _cipher = new unsigned char[length];

            std::copy_n(cipher, length, _cipher);
        }

        ~padding_oracle()
        {
            delete[] _cipher;
        }

        /**
         * Run the acctual break.
         *
         * @param plain  (Output) The decrypted plaintext
         * @param length The length of the plaintext provided, must be at least as long as the length of the ciphertex
         * @param iv     The IV used to encrypt the data. must be in a size of 'BlockSize'.
         */
        void run(unsigned char plain[], size_t length, const unsigned char iv[])
        {
            unsigned int write_index = length - 1;

            std::fill_n(plain, length + 1, 0);

            //
            // Lets figure out the last char of the intermediate state
            // We know that the the P[last] = I[last] ^ C-prev[last]
            //
            // So, if we change C-prev[last] (and name it C'-prev) so I[last] ^ C'[last] == 1 (meaning, P'[last] == 1) we
            // would'nt get padding error, that means that we can iterate over C'-prev[last] and check the padding unit we
            // discover the right value.
            //
            // After discovering it, we know that:
            //      P'[last] = I[last] ^ C'-prev[last]           ->
            //      1 =        I[last] ^ C'-prev[last]           ->
            //      1 ^ C'-prev[last] = I[last]
            //
            // And...
            //      P[last] = I[last] ^ C-prev[last]             ->
            //      P[last] ^ I[last] = C-prev[last]             ->
            //      I[last] = P[last] ^ C-prev[last]
            //
            // So...
            //      1 ^ C'-prev[last] = I[last]                  ->
            //      1 ^ C'-prev[last] = P[last] ^ C-prev[last]   ->
            //   ____________________________________________________
            //  |                                                    |
            //  |   P[last] = C-prev[last] ^ C'-prev[last] ^ 1       |
            //  |____________________________________________________|
            //
            //
            // Given P'-prev[last] ^ I[last] = 1 and I[last] is constant, we need to change P'-prev[last] so it will increase
            //
            //     P'-prev[last] ^ I[last] = 1                  ->
            //     P'-prev[last] ^ I[last] ^ 1 = 0
            //
            // Therefore, after finding the first byte (and actually and byte) in the padding (the byte with padding=1)
            // we need to move one (ie, padding=2, P[last]=2, P[last-1]=2)
            //
            // So, after finding the valid padding we will set the base mask so the P[i]=0)
            // And so we can work on the base mask and mask it will result the correct values
            //
            // For example, lets mark the base mask as M for each block. We already know that:
            //
            //     P[last] = C-prev[last] ^ C'-prev[last] ^ 1
            //
            // Therefore, we can write it as follow (given i is the index for the byte and the and p is the padding length)
            //
            //     P[last] = C-prev[i] ^ M[i] ^ p
            //
            std::array<unsigned char, BlockSize> base_mask = {0};

            auto blocks_break_pairs = generate_break_blocks_pairs(iv, _cipher, length, BlockSize);

            for (auto a : blocks_break_pairs)
            {
                for (int byte_index = BlockSize - 1; byte_index >= 0; byte_index--)
                {
                    bool found = false;
                    auto padding_length = BlockSize - byte_index;
                    std::array<unsigned char, BlockSize> iteration_mask = generate_iteration_mask(base_mask,
                                                                                                  padding_length);

                    for (unsigned int j = 0; j <= UCHAR_MAX; ++j)
                    {
                        auto _C = (unsigned char) j;
                        auto C = a.first[byte_index];
                        auto _P = (unsigned char) padding_length;
                        auto I = _C ^_P;
                        auto P = I ^C;

                        iteration_mask[byte_index] = _C;

                        //
                        // Create a special cipher containing the two blocks, the 'mask' block and the block we are breaking
                        //        block 1  |  block 2
                        //     [   mask   ] *[  block   ]
                        //
                        std::array<std::array<unsigned char, BlockSize>, 2> cipher_two_blocks = {0};
                        std::copy_n(iteration_mask.begin(), BlockSize, cipher_two_blocks[0].begin());
                        std::copy_n(a.second, BlockSize, cipher_two_blocks[1].begin());

                        if (_padding_oracle((unsigned char *) &cipher_two_blocks, BlockSize * 2, padding_length))
                        {
                            base_mask[byte_index] = _C ^ _P;

                            plain[write_index--] = (unsigned char) (P);
                            found = true;

                            break;
                        }
                    }

                    if (!found)
                    {
                        throw std::runtime_error("Internal error, did not found any matching byte, must be a bug");
                    }
                }
            }
        }

        /**
         * Generates an mask to make the second block all zero's.
         * For example for iteration 5:
         *
         * EndOf( Decrypt([Iteration Mask][ Block ]) ) = 00000
         *
         * The iteration mask is build based on a base mask generated and expanded each iteration from the previous
         * iteration mask.
         *
         * @tparam block_size    The block size used
         * @param base_mask      The base mask generated in the previous iteration
         * @param padding_length The padding length needed, etc. the iteration number
         * @return               The iteration mask
         */
        template<size_t block_size>
        std::array<unsigned char, block_size>
        generate_iteration_mask(const std::array<unsigned char, block_size> base_mask,
                                unsigned int padding_length)
        {
            //
            // We know that if we encrypt with the base mask like this:
            //        block 1  |  block 2
            //     [   mask   ] [  block   ]
            //
            // It will result a plaintext with all zeros until the 'padding_length-1' byte (from the end).
            // Therefor, if we prepare the iteration mask for padding_length = i we need to modify each value in the mask
            // so the calculation will result the values 'padding-length' until the 'padding_length-1' (from the end)
            //
            std::array<unsigned char, block_size> iter_mask = {0};

            if (padding_length == 0)
            {
                throw std::runtime_error("There is no padding = 0, only padding=block_size in case of no padding");
            }

            for (int i = 0; i < padding_length - 1; ++i)
            {
                iter_mask[block_size - i - 1] = base_mask[block_size - i - 1] ^ padding_length;
            }

            return iter_mask;
        }

        /**
         * Generate a pairs of blocks to decrypt as following:
         *     (IV,     Block0)
         *     (Block0, Block1)
         *     (Block1, Block2)
         *     (Block2, Block3)
         *
         * @param iv            The IV used to decrypt the data
         * @param cipher        The ciphertext to decrypt
         * @param cipher_length The length of the ciphertext
         * @param block_size    The block size
         * @return A list of pairs as described in the description.
         */
        auto generate_break_blocks_pairs(const unsigned char iv[],
                                         unsigned char cipher[],
                                         size_t cipher_length,
                                         size_t block_size)
        {
            std::vector<std::pair<const unsigned char *, const unsigned char *>> v;

            std::copy_n(cipher, cipher_length, cipher);

            breaks::internal::ecb_block_parser helper(cipher, cipher_length, block_size);

            for (int i = helper.blocks_count() - 1; i >= 1; --i)
            {
                v.emplace_back(helper[i - 1].data(), helper[i].data());
            }

            v.emplace_back(iv, helper[0].data());

            return v;
        }

    private:
        check_padding_function_t _padding_oracle;
        unsigned char *_cipher;
    };
};

#endif //CRYPTOPALS_PADDING_ORACLE_H
