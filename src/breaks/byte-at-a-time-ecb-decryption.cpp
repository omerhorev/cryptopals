//
// Created by omerh on 06/09/2019.
//

#include <cstddef>
#include <breaks/byte-at-a-time-ecb-decryption.h>
#include <breaks/internal/ecb-block-parser.h>
#include <utils/general.h>
#include <utils/debug.h>

using namespace breaks;
using namespace breaks::internal;

size_t byte_in_a_time_ecb_decryption::run(unsigned char *hidden_message_output, size_t hidden_message_output_max_size)
{
    unsigned char plain[max_hidden_messge_length * 2];
    unsigned char cipher[max_encrypted_data_length];
    size_t write_index = 0;

    // First, lets detect the block size
    detect_block_size();
    size_t calculated_hidden_message_max_length = detect_hidden_block_count() * _block_size;

    // Lets fill our malicious plain with 'clean' known data.
    std::generate_n(plain, sizeof(plain), []() { return 15; });

    //
    //  Lets breaks one char at a time...
    //
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

            auto l = encrypt(plain, plain_length, cipher, sizeof(cipher));

            if (l == 0)
            { throw std::runtime_error("Unable to encrypt the data"); }

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

size_t byte_in_a_time_ecb_decryption::encrypt(const unsigned char *data,
                                              size_t data_length,
                                              unsigned char *buffer,
                                              size_t buffer_size)
{
    const size_t max_tries = 160;

    if (_block_size == 0)
    {
        throw std::runtime_error("The block size was not set");
    }

    return encrypt_advanced(data, data_length, buffer, buffer_size, _block_size, max_tries);
}

size_t breaks::byte_in_a_time_ecb_decryption::encrypt_advanced(const unsigned char *message, size_t message_length,
                                                               unsigned char *buffer, size_t buffer_length,
                                                               size_t block_size, size_t max_tries)
{
    const size_t detection_blocks_to_add = 4;
    auto detection_data_length = detection_blocks_to_add * block_size;
    auto temp_msg_length = message_length + detection_data_length;
    auto temp_buffer_length = buffer_length + detection_data_length;
    auto *temp_msg = new unsigned char[temp_msg_length];
    auto *temp_buffer = new unsigned char[temp_buffer_length];

    std::fill_n(temp_msg, detection_data_length, (unsigned char) '?');
    std::copy_n(message, message_length, temp_msg + detection_data_length);

    size_t j = 0;
    size_t data_without_random_and_sync_size = 0;
    unsigned char *data_without_random_and_sync = nullptr;

    try
    {

        while (data_without_random_and_sync == nullptr && j < max_tries)
        {
            j++;
            auto encrypted_length = _encryption_oracle(temp_msg, temp_msg_length, temp_buffer,
                                                       temp_buffer_length);

            internal::ecb_block_parser helper(temp_buffer, encrypted_length, block_size);

            auto iter1 = std::adjacent_find(helper.begin(), helper.end());
            if (iter1 == helper.end())
            {
                // Didn't find two consecutive blocks, Maybe we found a wrong block size?
                break;
            }

            auto iter2 = std::search_n(helper.begin(), helper.end(), detection_blocks_to_add, *iter1);

            if (iter2 == helper.end())
            {
                continue;
            }

            size_t random_data_length = (iter2 - helper.begin()) * block_size;
            data_without_random_and_sync_size = encrypted_length - random_data_length - detection_data_length;
            data_without_random_and_sync = (iter2 + detection_blocks_to_add)._block.data();
        }

    } catch (std::exception &e)
    {
        data_without_random_and_sync = nullptr;
        data_without_random_and_sync_size = 0;
    }

    if (data_without_random_and_sync != nullptr)
    {
        std::copy_n(data_without_random_and_sync, data_without_random_and_sync_size, buffer);
    }

    delete[] temp_msg;
    delete[] temp_buffer;

    return data_without_random_and_sync_size;
}

void byte_in_a_time_ecb_decryption::detect_block_size()
{
    const size_t min_block_size = 1;
    const size_t max_block_size = 64;

    unsigned char data[max_block_size * 2] = {0};

    for (size_t block_size = min_block_size; block_size < max_block_size; block_size++)
    {
        unsigned char buffer[max_encrypted_data_length];

        auto length = encrypt_advanced(data, block_size * 2, buffer, max_encrypted_data_length, block_size, 200);

        if (length != 0)
        {
            _block_size = block_size;
            return;
        }
    }

    throw std::runtime_error("Unable to detect the block size");
}
