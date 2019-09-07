//
// Created by omerh on 20/08/2019.
//

#ifndef CRYPTOPALS_BYTE_AT_A_TIME_ECB_DECRYPTION_H
#define CRYPTOPALS_BYTE_AT_A_TIME_ECB_DECRYPTION_H

#include <cstddef>
#include <functional>

namespace breaks
{
    class byte_in_a_time_ecb_decryption
    {
    public:
        typedef std::function<size_t(const unsigned char *, size_t, unsigned char *, size_t)> encryption_function_t;

        explicit byte_in_a_time_ecb_decryption(encryption_function_t encryption_oracle)
                : _encryption_oracle(std::move(encryption_oracle))
        {}

        size_t run(unsigned char hidden_message_output[], size_t hidden_message_output_max_size);

    private:

        /**
         * Because the encrypted data can consist of a random amount of bytes at the start, and assuming it is
         * distributed evenly, in average every 16 encryptions it we generated an exact block sized random data.
         * In that can, we can just erase that block and we will receive a data encrypted as it has no random data
         * at the start.
         *
         * The way we discover if the data added before the message was rounded to 'block size' is adding blocks
         * of known data (for example 'aaaaa...'), the amount of blocks must be at least as the hidden message itself
         * so we wont mix them up. if we discover that all the additional blocks are identical in the encrypted data
         * that means the random data was a multiplicand of the block size
         *
         * @param message         The message to encrypt
         * @param message_length  The length of the message to encrypt
         * @param buffer          (output) The buffer to encrypt into
         * @param buffer_length   The length of the output buffer
         * @param block_size      Default value is the class's member '_block_size'
         * @param max_ties        This method will encrypt multiple times, so we need to max it up
         * @return
         */
        size_t encrypt_advanced(const unsigned char *message,
                                size_t message_length,
                                unsigned char *buffer,
                                size_t buffer_length,
                                size_t block_size,
                                size_t max_tries);

        /**
         * Encrypts some data while canceling the random data before the 'data' and leaving only the hidden message
         * after the data.
         *
         * see the documentation of encrypt_advanced
         *
         * @note Must be used after the block_size was set.
         *
         * @param data        The data to encrypt
         * @param data_length The length of the data to encrypt
         * @param buffer      The output buffer to put the data into
         * @param buffer_size The length of the output buffer
         * @return            The size of the encrypted data
         */
        size_t encrypt(const unsigned char data[], size_t data_length, unsigned char buffer[], size_t buffer_size);

        /**
         * Used a sequence of encryptions to detect the block size
         */
        void detect_block_size();

        /**
         * After detecting the block size, detect the number of blocks the hidden message is
         */
        size_t detect_hidden_block_count()
        {
            if (_block_size == 0)
            { throw std::runtime_error("Must be used after the block_size was detected"); }

            unsigned char buffer[1] = {0};
            unsigned char output[max_encrypted_data_length] = {0};

            auto l = encrypt(buffer, 0, output, sizeof(output));

            return l / _block_size;
        }

    private:
        size_t _block_size;
        encryption_function_t _encryption_oracle;

    private:
        static const size_t max_encrypted_data_length = 1024;
        static const size_t max_hidden_messge_length = 256;
    };
}

#endif //CRYPTOPALS_BYTE_AT_A_TIME_ECB_DECRYPTION_H
