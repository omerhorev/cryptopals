//
// Created by omerh on 20/08/2019.
//

#ifndef CRYPTOPALS_BYTE_AT_A_TIME_ECB_DECRYPTION_H
#define CRYPTOPALS_BYTE_AT_A_TIME_ECB_DECRYPTION_H

#include <iomanip>
#include <iostream>
#include <cstddef>
#include <functional>
#include <algorithm>
#include <climits>
#include "utils/general.h"
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

        size_t run(unsigned char hidden_message_output[], size_t hidden_message_output_max_size)
        {
            // First, lets detect the block size
            size_t block_size = detect_block_size();
            size_t hidden_message_length = 0;
            size_t write_index = 0;

            if (block_size == 0)
            { return 0; }

            size_t hidden_message_block_count = detect_hidden_block_count(block_size);
            size_t calculated_hidden_message_max_length = hidden_message_block_count * block_size;

            unsigned char plain[max_hidden_messge_length * 2];
            unsigned char cipher[max_encrypted_data_length];

            std::generate_n(plain, sizeof(plain), []() { return 15; });

            for (size_t i = 0; i < calculated_hidden_message_max_length; i++)
            {
                size_t plain_length = calculated_hidden_message_max_length * 2 - i - 1;
                size_t unknown_char_index = calculated_hidden_message_max_length - 1;

                utils::general::shift_left(plain, calculated_hidden_message_max_length, 1, (unsigned char) 0x00);

                bool found = false;

                for (int j = 0; j <= UCHAR_MAX; ++j)
                {
                    auto c = (unsigned char) j;

                    plain[unknown_char_index] = c;

                    _encryption_oracle(plain, plain_length, cipher, sizeof(cipher));

                    if (std::equal(cipher + calculated_hidden_message_max_length * 0,
                                   cipher + calculated_hidden_message_max_length * 1,
                                   cipher + calculated_hidden_message_max_length * 1,
                                   cipher + calculated_hidden_message_max_length * 2))
                    {
                        found = true;

                        // Write into the buffer if we can!
                        if (write_index < hidden_message_output_max_size) hidden_message_output[write_index++] = c;

                        break;
                    }
                }

                if (!found)
                {
                    // We did'nt found any data, that means we ended up in a data that isn't consistent through two
                    // different runs. That can be:
                    //  1) Random data (Not supported)
                    //  2) Data that is depended on the data provided (Not supported, needs to be modulated)
                    //  3) The PKCS7 padding (This is the case in this break!)
                    //
                    // If its indeed the PKCS7 padding we will be able to found the \01 padding at the end but not the
                    // \02 (because once we decrease the input length the \01 will because \02.
                    // Meaning, the weite_index, which represents the length includes the \01. Lets decrease it by 1
                    // so we will get a correct length!
                    write_index--;
                    break;
                }
            }

            return write_index;
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

        size_t detect_hidden_block_count(size_t block_size)
        {
            unsigned char buffer[1] = {0};
            unsigned char output[max_encrypted_data_length] = {0};

            return _encryption_oracle(buffer, 0, output, sizeof(output)) / block_size;
        }

    private:
        encryption_function_t _encryption_oracle;

    private:
        static const size_t max_encrypted_data_length = 1024;
        static const size_t max_hidden_messge_length = 256;
    };
}

#endif //CRYPTOPALS_BYTE_AT_A_TIME_ECB_DECRYPTION_H
