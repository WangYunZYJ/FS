//
// Created by wy on 19-1-1.
//

#ifndef FS_INODE_H
#define FS_INODE_H

#include <time.h>
#include <string>
#include <tiff.h>
#include <include/constant/constants.h>

namespace wyfs
{
    enum FileMode
    {
        NORMAL_FILE,
        MENU_FILE
    };

    struct FilePermision
    {
        uint32 owner : 3;
        uint32 group_menber : 3;
        uint32 others : 3;
    };


    struct inode
    {
        uint32 file_mode;
        FilePermision file_permision;
        char owner[14];
        time_t timestamp;
        uint32 file_size;
        uint32 dirct_block[INODE_DIRECT_BLOCK_COUNT];
        uint32 first_block;
        uint32 second_block;
        uint32 block_count;
        uint32 group;
        uint32 inode_id;
        void print_test();
    };
}

#endif //FS_INODE_H
