//
// Created by wy on 18-12-30.
//

#ifndef FS_CONSTANT_H
#define FS_CONSTANT_H

#include <tiff.h>
#include <cstddef>

namespace wyfs
{
    const int16 BLOCK_SIZE = 512;
    const size_t MAX_STACK_FREE_SIZE = 100;
    const unsigned int BLOCK_AMOUNT = 1 << 16;
}


#endif //FS_CONSTANT_H
