//
// Created by omerh on 28/09/2019.
//

#include <cstddef>
#include <breaks/bitflipping.h>
#include <breaks/internal/ecb-block-parser.h>

using namespace breaks;
using namespace breaks::internal;

void bitflippig::do_flip(unsigned char *buffer, size_t length, size_t block_size)
{
    internal::ecb_block_parser helper(buffer, length, block_size);

    helper[2].data()[5] ^= 1;
}
