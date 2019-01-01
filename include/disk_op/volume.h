//
// Created by wy on 19-1-1.
//

#ifndef FS_VOLUME_H
#define FS_VOLUME_H


#include <memory>
#include <tiff.h>
#include <vector>

using namespace std;
namespace wyfs
{
    class volume
    {
        static shared_ptr<volume> instance;
        volume();
        void next_stack_remove(uint32& pre_stack_size, const uint32 next_stack, const uint32 pre_addr);
        void apply_error_occurs(vector<uint32>& applied_blocks, uint32& stack_size);
    public:
        static shared_ptr<volume>& get_instance();
        void create_disk();
        void init(const uint32 block_id);
        void disk_init_read_test(const uint32 block_id);
        std::vector<uint32> apply_for_free_block(uint32 amount);
        const uint32 release_free_block(uint32 blk);
    };
}

#endif //FS_VOLUME_H
