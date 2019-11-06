//
// Created by omerh on 29/10/2019.
//

#include <gtest/gtest.h>
#include <model/stream.h>
#include <utils/random.h>
#include <utils/string-score.h>
#include "../../utils/test-utils.h"

TEST(set_3_3, run)
{
    std::array<unsigned char, 16> hidden_key = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                                0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08};

    std::array<test_utils::base64_test_vector<>, 40> vectors{
            "SSBoYXZlIG1ldCB0aGVtIGF0IGNsb3NlIG9mIGRheQ==",
            "Q29taW5nIHdpdGggdml2aWQgZmFjZXM=",
            "RnJvbSBjb3VudGVyIG9yIGRlc2sgYW1vbmcgZ3JleQ==",
            "RWlnaHRlZW50aC1jZW50dXJ5IGhvdXNlcy4=",
            "SSBoYXZlIHBhc3NlZCB3aXRoIGEgbm9kIG9mIHRoZSBoZWFk",
            "T3IgcG9saXRlIG1lYW5pbmdsZXNzIHdvcmRzLA==",
            "T3IgaGF2ZSBsaW5nZXJlZCBhd2hpbGUgYW5kIHNhaWQ=",
            "UG9saXRlIG1lYW5pbmdsZXNzIHdvcmRzLA==",
            "QW5kIHRob3VnaHQgYmVmb3JlIEkgaGFkIGRvbmU=",
            "T2YgYSBtb2NraW5nIHRhbGUgb3IgYSBnaWJl",
            "VG8gcGxlYXNlIGEgY29tcGFuaW9u",
            "QXJvdW5kIHRoZSBmaXJlIGF0IHRoZSBjbHViLA==",
            "QmVpbmcgY2VydGFpbiB0aGF0IHRoZXkgYW5kIEk=",
            "QnV0IGxpdmVkIHdoZXJlIG1vdGxleSBpcyB3b3JuOg==",
            "QWxsIGNoYW5nZWQsIGNoYW5nZWQgdXR0ZXJseTo=",
            "QSB0ZXJyaWJsZSBiZWF1dHkgaXMgYm9ybi4=",
            "VGhhdCB3b21hbidzIGRheXMgd2VyZSBzcGVudA==",
            "SW4gaWdub3JhbnQgZ29vZCB3aWxsLA==",
            "SGVyIG5pZ2h0cyBpbiBhcmd1bWVudA==",
            "VW50aWwgaGVyIHZvaWNlIGdyZXcgc2hyaWxsLg==",
            "V2hhdCB2b2ljZSBtb3JlIHN3ZWV0IHRoYW4gaGVycw==",
            "V2hlbiB5b3VuZyBhbmQgYmVhdXRpZnVsLA==",
            "U2hlIHJvZGUgdG8gaGFycmllcnM/",
            "VGhpcyBtYW4gaGFkIGtlcHQgYSBzY2hvb2w=",
            "QW5kIHJvZGUgb3VyIHdpbmdlZCBob3JzZS4=",
            "VGhpcyBvdGhlciBoaXMgaGVscGVyIGFuZCBmcmllbmQ=",
            "V2FzIGNvbWluZyBpbnRvIGhpcyBmb3JjZTs=",
            "SGUgbWlnaHQgaGF2ZSB3b24gZmFtZSBpbiB0aGUgZW5kLA==",
            "U28gc2Vuc2l0aXZlIGhpcyBuYXR1cmUgc2VlbWVkLA==",
            "U28gZGFyaW5nIGFuZCBzd2VldCBoaXMgdGhvdWdodC4=",
            "VGhpcyBvdGhlciBtYW4gSSBoYWQgZHJlYW1lZA==",
            "QSBkcnVua2VuLCB2YWluLWdsb3Jpb3VzIGxvdXQu",
            "SGUgaGFkIGRvbmUgbW9zdCBiaXR0ZXIgd3Jvbmc=",
            "VG8gc29tZSB3aG8gYXJlIG5lYXIgbXkgaGVhcnQs",
            "WWV0IEkgbnVtYmVyIGhpbSBpbiB0aGUgc29uZzs=",
            "SGUsIHRvbywgaGFzIHJlc2lnbmVkIGhpcyBwYXJ0",
            "SW4gdGhlIGNhc3VhbCBjb21lZHk7",
            "SGUsIHRvbywgaGFzIGJlZW4gY2hhbmdlZCBpbiBoaXMgdHVybiw=",
            "VHJhbnNmb3JtZWQgdXR0ZXJseTo=",
            "QSB0ZXJyaWJsZSBiZWF1dHkgaXMgYm9ybi4="
    };

    for (auto &vector : vectors)
    {
        model::aes128_ctr _;
        _.initialize_encryption(hidden_key.data(), hidden_key.size(), 0, 0);
        _.encrypt(vector.data, vector.length);
    }

    const auto min_length = std::min_element(vectors.begin(), vectors.end(),
                                             [](auto &v1, auto &v2) { return v1.length < v2.length; })->length;

    //
    // We know all these buffers were encrypted with the key-buffer (from the CTR) marked as B
    // So...
    //
    //     Pi ^ B = Ci ->
    //     Pi = Ci ^ B ->
    //     P1 ^ P2 = (C1 ^ B) ^ (C2 ^ B) ->
    //     P1 ^ P2 ^ .... ^ Pn = C1 ^ C2 ^ ... ^ Cn
    //
    // So because we have allot of buffers, lets iterate over all the possible bytes in B (byte at a time)
    // and check for what byte we have the most printable letters in all the other plaintexts (string scoring)

    std::vector<std::array<float, UCHAR_MAX + 1>> score;

    // Iterate over each char in the vectors
    for (auto i = 0; i < min_length; ++i)
    {
        std::array<float, UCHAR_MAX + 1> vector_possible_key_char_score = {0};

        // Iterator over each possible bytes
        for (int possible_key_char = 0; possible_key_char < UCHAR_MAX + 1; ++possible_key_char)
        {
            // Iterate over all the vectors and calculate the score accordingly
            for (auto &vector : vectors)
            {
                vector_possible_key_char_score[possible_key_char] += utils::string_score::get_letter_english_frequency(
                        (unsigned char) (vector.data[i] ^ possible_key_char));
            }
        }

        score.push_back(vector_possible_key_char_score);
    }

    auto broken_key_xor_vector = new unsigned char[min_length];
    auto temp_plain = new unsigned char[min_length];

    for (int k = 0; k < min_length; ++k)
    {
        broken_key_xor_vector[k] = (unsigned char) std::distance(score[k].begin(),
                                                                 std::max_element(score[k].begin(), score[k].end()));
    }

    for (auto &vector : vectors)
    {
        // Decrypt by the broken key
        utils::general::xor_buffers(vector.data, broken_key_xor_vector, temp_plain, min_length);

        // Decrypt normally
        model::aes128_ctr model;
        model.initialize_decryption(hidden_key.data(), hidden_key.size(), 0, 0);
        model.decrypt(vector.data, min_length);

        // compare the two
        ASSERT_TRUE(memcmp(vector.data, temp_plain, min_length) == 0);
    }

    delete[] broken_key_xor_vector;
    delete[] temp_plain;
}

