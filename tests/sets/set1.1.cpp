//
// Created by omerh on 20/07/2019.
//

#include <hex.h>
#include <base64.h>
#include "gtest/gtest.h"

TEST(sets, set_1_1)
{
    char test_vector_hex[] = "49276d206b696c6c696e6720796f757220627261696e206c696b65206120706f69736f6e6f7573206d757368726f6f6d";
    char test_vector_base64[] = "SSdtIGtpbGxpbmcgeW91ciBicmFpbiBsaWtlIGEgcG9pc29ub3VzIG11c2hyb29t";

    unsigned char decoded[256];
    char base_64_decoded[256];
    char hex_decoded[256];

    auto decoded_length = hex::decode(test_vector_hex, strlen(test_vector_hex), decoded, sizeof(decoded));
    base64::encode(decoded, decoded_length, base_64_decoded, sizeof(base_64_decoded));

    ASSERT_STREQ(test_vector_base64, base_64_decoded);

}

