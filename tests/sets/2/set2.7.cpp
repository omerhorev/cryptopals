//
// Created by omerh on 13/09/2019.
//

#include <gtest/gtest.h>
#include <model/padding.h>

TEST(set_2_7, run)
{
    const unsigned char *vectors[]
            {
                    (unsigned char *) "ICE ICE BABY\x05\x05\x05\x05",
                    (unsigned char *) "ICE ICE BABY\x01\x02\x03\x04"
            };
    for (auto data : vectors)
    {
        const auto block_size = 16;

        ASSERT_THROW(model::padding::pkcs7::decode(data, block_size, block_size),
                     model::internal::padding::pkcs7::invalid_padding);
    }
}