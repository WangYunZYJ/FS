//
// Created by wy on 19-1-1.
//

#ifndef FS_CONSTANTS_H
#define FS_CONSTANTS_H

#include <string>
#include <tiff.h>

namespace wyfs
{
    const std::string DISK_PATH = "/home/wy/CLionProjects/FS/disk";
    const uint32 FREE_BLOCK_MAX_COUNT_EACH_STACK = 100;
    const uint32 MAX_FREE_BLOCK_COUNT = 65535;
    const uint32 BLOCK_SIZE = 512;
    const uint32 FREE_BLOCK_BEGIN = 4;
}

#endif //FS_CONSTANTS_H
