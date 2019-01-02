//
// Created by wy on 19-1-1.
//

#ifndef FS_INODE_ID_H
#define FS_INODE_ID_H

#include <tiff.h>
#include <include/constant/constants.h>

using namespace std;

namespace wyfs
{
    struct inode_block
    {
        uint32 inode_size;
        uint32 ids[INODE_COUNT_PER_BLOCK];
        uint32 next;
        inode_block() = default;
    };
}

#endif //FS_INODE_ID_H
