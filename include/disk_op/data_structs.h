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

    struct filename_and_inode_addr{
        char filename[NAME_SIZE];
        uint32 inode_addr;
    };

    struct username_saved_as_menu{
        uint32 name_counts;
        filename_and_inode_addr fiaddr[ID_NAME_COUNT_PER_BLOCK];  // 18字节
        uint32 next_menu_block_addr;
    };

    struct tree_list {
        uint32 sons_size;
        uint32 sons_inode_addr[MAX_SONS_COUNT];
    };

    struct username_pwd_in{
        char password[NAME_SIZE];
        char username[NAME_SIZE];
    };

    struct username_password{
        uint32 user_counts;
        username_pwd_in users[BLOCK_SIZE / sizeof(username_pwd_in)];//28B
    };
}

#endif //FS_DATA_STRUCTS_H
