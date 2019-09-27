//
// Created by omerh on 13/09/2019.
//

#include <gtest/gtest.h>
#include <utils/sanitation.h>

TEST(sanitaion, test)
{
    std::string encoded_test = "key1&--&value1&.,&key2&--&value2";
    std::string decoded_test = "key1=value1;key2=value2";

    std::string str = decoded_test;

    utils::sanitaion::encode(str);
    ASSERT_EQ(str, encoded_test);

    utils::sanitaion::decode(str);
    ASSERT_EQ(str, decoded_test);
}