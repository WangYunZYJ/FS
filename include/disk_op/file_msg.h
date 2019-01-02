//
// Created by wy on 19-1-2.
//

#ifndef FS_FILE_MSG_H
#define FS_FILE_MSG_H

#include <tiff.h>
#include <include/constant/constants.h>

namespace wyfs
{
    struct file_msg
    {
        uint32 msg_length;
        char byte[BLOCK_SIZE - sizeof(uint32)];
    };
}

#endif //FS_FILE_MSG_H
