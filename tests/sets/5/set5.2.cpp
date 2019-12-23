//
// Created by omerh on 20/12/2019.
//

#include <model/diffie-hellman.h>
#include <model/block.h>
#include <utils/random.h>
#include "gtest/gtest.h"

using namespace model;

template<size_t N>
using data_t = std::array<unsigned char, N>;

struct msg1
{
    data_t<8> p;
    data_t<8> g;
    data_t<8> A;
};

struct msg2
{
    data_t<8> B;
};

TEST(set_5_2, run)
{

}