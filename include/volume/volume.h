
#ifndef FS_VOLUME_H
#define FS_VOLUME_H


#include <stack>
#include <inode.h>
#include <memory>
#include "block.h"
#include "constant.h"

namespace wyfs
{
    class volume
    {
        static int ID;
        volume();
        static std::shared_ptr<volume> instance;
        int curr_free;
        std::stack<std::shared_ptr<block>> blocks;
        std::shared_ptr<std::shared_ptr<block>> next_block;
        std::stack<inode> inode_stack;
    public:
        ~volume();
        static std::shared_ptr<volume> get_instance();
    };
}


#endif //FS_VOLUME_H
