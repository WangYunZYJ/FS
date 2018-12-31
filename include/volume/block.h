//
// Created by wy on 18-12-30.
//

#ifndef FS_BLOCK_H
#define FS_BLOCK_H

namespace wyfs
{
    struct block_id
    {
        unsigned int addr : 16;
    };

    class block
    {
        block_id id;
    public:
        block() = default;
        const int get_addr() {return id.addr;}
        block(int _id) {id.addr = _id;}
    };
}

#endif //FS_BLOCK_H
