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
    const uint32 INODE_COUNT_PER_BLOCK = 122;
    const uint32 INODE_DIRECT_BLOCK_COUNT = 10;
    const uint32 INODE_SECOND_BLOCK_COUNT = 128;
    const uint32 MAX_SONS_COUNT = 127;
    const uint32 NAME_SIZE = 14;
    const uint32 ID_NAME_COUNT_PER_BLOCK = 28;
    const uint32 FREE_BLOCK_BEGIN = 4;
    const uint32 USERNAME_PASSWORD = 2;
    const uint32 INODE_TABLE = 1;
    const uint32 TREE_STRUCT = 0;
    const uint32 WIDTH = 10;
}

#endif //FS_CONSTANTS_H
