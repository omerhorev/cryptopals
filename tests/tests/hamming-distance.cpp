//
// Created by omerh on 26/07/2019.
//

#include <gtest/gtest.h>
#include <utils/hamming-distance.h>

using namespace utils;

TEST(utils, hamming_distance)
{
    ASSERT_EQ(hamming_distance::calculate((const unsigned char *) "this is a test",
                                          (const unsigned char *) "wokka wokka!!!",
                                          14), 37);
}
