//
// Created by wy on 19-1-1.
//

#ifndef FS_DISK_INIT_H
#define FS_DISK_INIT_H

#include <vector>
#include <tiff.h>

namespace wyfs
{
    void create_disk();
    void init(const uint32 block_id);
    void disk_init_read_test(const uint32 block_id);
    std::vector<uint32> apply_for_free_block(uint32 amount);
}

#endif //FS_DISK_INIT_H
