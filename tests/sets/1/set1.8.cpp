//
// Created by omerh on 14/08/2019.
//

#include <gtest/gtest.h>
#include <fstream>
#include "utils/files.h"
#include "breaks/moo-detector.h"
using namespace utils;

TEST(set_1_8, run)
{
    unsigned char data[204][160];

    size_t num_of_lines = files::read_hex_lines("data/sets/1/8.txt", data);

    ASSERT_EQ(num_of_lines, 204);

    bool found_ecb_mode = false;

    for (auto& line : data)
    {
        if (breaks::moo_detector::is_ecb(line, 160, 16))
        {
            found_ecb_mode = true;
        }
    }

    ASSERT_TRUE(found_ecb_mode);
}