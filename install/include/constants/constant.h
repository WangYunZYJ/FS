//
// Created by wy on 18-12-30.
//

#ifndef FS_CONSTANT_H
#define FS_CONSTANT_H

#include <tiff.h>
#include <cstddef>
#include <string>

namespace wyfs
{
    const int MAGIC = 10086;
    const int16 BLOCK_SIZE = 512;
    const size_t MAX_STACK_FREE_SIZE = 100;
    const unsigned int BLOCK_AMOUNT = 1 << 16;
    const std::string DISK_PATH = "/home/wy/FS/disk";
}


#endif //FS_CONSTANT_H
