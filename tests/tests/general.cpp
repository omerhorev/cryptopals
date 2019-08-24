//
// Created by omerh on 24/08/2019.
//

#include <gtest/gtest.h>
#include <utils/general.h>

TEST(general_utils, shift_left_normal)
{
    int data[6] = {1, 2, 3, 4, 5, 6};

    utils::general::shift_left(data, 6, 2, 0);

    ASSERT_EQ(data[0], 3);
    ASSERT_EQ(data[1], 4);
    ASSERT_EQ(data[2], 5);
    ASSERT_EQ(data[3], 6);
    ASSERT_EQ(data[4], 0);
    ASSERT_EQ(data[5], 0);
}

TEST(general_utils, shift_left_all)
{
    int data[6] = {1, 2, 3, 4, 5, 6};

    utils::general::shift_left(data, 6, 6, 0);

    ASSERT_EQ(data[0], 0);
    ASSERT_EQ(data[1], 0);
    ASSERT_EQ(data[2], 0);
    ASSERT_EQ(data[3], 0);
    ASSERT_EQ(data[4], 0);
    ASSERT_EQ(data[5], 0);
}

TEST(general_utils, shift_left_over)
{
    int data[6] = {1, 2, 3, 4, 5, 6};

    utils::general::shift_left(data, 6, 7, 0);

    ASSERT_EQ(data[0], 0);
    ASSERT_EQ(data[1], 0);
    ASSERT_EQ(data[2], 0);
    ASSERT_EQ(data[3], 0);
    ASSERT_EQ(data[4], 0);
    ASSERT_EQ(data[5], 0);
}