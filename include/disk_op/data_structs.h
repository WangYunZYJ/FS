//
// Created by wy on 19-1-3.
//

#ifndef FS_DATA_STRUCTS_H
#define FS_DATA_STRUCTS_H

#include <tiff.h>
#include <include/constant/constants.h>

namespace wyfs
{
    struct file_msg
    {
        uint32 msg_length;
        char byte[BLOCK_SIZE - sizeof(uint32)];
    };
//
//    struct inode_block
//    {
//        uint32 inode_size; // 4
//        char file_name[NAME_SIZE]; //14
//        uint32 ids[INODE_COUNT_PER_BLOCK];
//        uint32 next;
//        inode_block() = default;
//    };

    struct filename_and_inode_addr{
        char username[NAME_SIZE];
        uint32 inode_addr;
    };

    struct username_saved_as_menu{
        uint32 name_counts;
        filename_and_inode_addr fiaddr[28];  // 18字节
        uint32 next_menu_block_addr;
    };
}

#endif //FS_DATA_STRUCTS_H
