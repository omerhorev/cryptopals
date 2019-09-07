//
// Created by omerh on 06/09/2019.
//

#include <cstddef>
#include <gtest/gtest.h>
#include "breaks/internal/ecb-block-parser.h"

using namespace breaks;
using namespace breaks::internal;

TEST(ecb_block_parser, work)
{
    const size_t block_size = 16;
    const size_t blocks_count = 4;
    unsigned char buffer[block_size * blocks_count] = {0};

    ecb_block_parser p(buffer, sizeof(buffer), block_size);

    auto a = p.begin();
    auto b = p.begin();

    //  begin               end
    //   |                   |
    // [ 0 ][ 1 ][ 2 ][ 3 ][ X ]
    //

    ASSERT_EQ(a, p.begin());
    ASSERT_EQ(a + 1, ((p.begin() + 2) - 1));
    ASSERT_EQ(a + 2, p.end() - 2);
    ASSERT_EQ(a + 4, p.end());
    ASSERT_EQ(a, p.end() - 4);
    ASSERT_EQ(a++, b);
    ASSERT_EQ(a, b + 1);
    ASSERT_EQ(++a, b + 2);
    ASSERT_EQ(a, b + 2);
}