//
// Created by omerh on 20/08/2019.
//

#ifndef CRYPTOPALS_BYTE_AT_A_TIME_ECB_DECRYPTION_H
#define CRYPTOPALS_BYTE_AT_A_TIME_ECB_DECRYPTION_H

#include <cstddef>
#include <functional>
#include <algorithm>
#include "moo-detector.h"

namespace breaks
{
    class byte_in_a_time_ecb_decryption
    {
    public:
        typedef std::function<size_t(const unsigned char *, size_t, unsigned char *, size_t)> encryption_function_t;

        explicit byte_in_a_time_ecb_decryption(encryption_function_t encryption_oracle)
                : _encryption_oracle(std::move(encryption_oracle))
        {}

        void run(char hidden_message[], size_t hidden_message_max_size)
        {
            // First, lets detect the block size
            size_t block_size = detect_block_size();

            if (block_size == 0)
            { return; }

            unsigned char buffer[max_hidden_messge_length] = {0};
            unsigned char output[max_encrypted_data_length] = {0};

            for (int i = 0; i < 256; ++i)
            {
                auto c = (unsigned char)i;

                std::generate_n(buffer, block_size * 2, []() { return 'x'; });
                buffer[block_size - 1] = c;

                auto l  = _encryption_oracle(buffer, block_size * 2 - 1, output, sizeof(output));

                if (breaks::moo_detector::is_ecb(output, block_size * 2, block_size))
                {
                    l = 2;
                };
            }
        }

        size_t detect_block_size()
        {
            const size_t min_block_size = 1;
            const size_t max_block_size = 64;

            unsigned char data[max_block_size * 2] = {0};

            for (size_t block_size = min_block_size; block_size < max_block_size; block_size++)
            {
                unsigned char buffer[max_encrypted_data_length];

                auto length = _encryption_oracle(data, block_size * 2, buffer, max_encrypted_data_length);

                if (breaks::moo_detector::is_ecb(buffer, block_size * 2, block_size))
                {
                    return block_size;
                }
            }

            return 0;
        }

    private:
        encryption_function_t _encryption_oracle;

    private:
        static const size_t max_encrypted_data_length = 256;
        static const size_t max_hidden_messge_length = 128;
    };
}

#endif //CRYPTOPALS_BYTE_AT_A_TIME_ECB_DECRYPTION_H
