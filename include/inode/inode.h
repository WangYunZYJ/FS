//
// Created by wy on 19-1-1.
//

#ifndef FS_INODE_H
#define FS_INODE_H

#include <time.h>
#include <string>
#include <tiff.h>

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

    class inode
    {
        uint32 file_mode;
        FilePermision file_permision;
        std::string owner;
        time_t timestamp;
        uint32 file_size;
        uint32 dirct_block[10];
        uint32 first_block;
        uint32 second_block;
        uint32 third_block;
        uint32 block_count;
        uint32 group;
        uint32 inode_id;
    public:
        inode(const uint32 file_mode, const uint32 owner_permision, const uint32 group_member_permision, const uint32 others_permision,
                const std::string owner, const uint32 file_size, const uint32 group, const uint32 inode_id);
        inode(const std::string owner, const uint32 group);
    };
}

#endif //FS_INODE_H
