//
// Created by wy on 18-12-30.
//

#ifndef FS_FREE_STACK_H
#define FS_FREE_STACK_H

#include <stack>
#include <memory>
#include "block.h"
#include <vector>

///成组链接法

using namespace std;
namespace wyfs
{
    class free_stack
    {
        std::stack<shared_ptr<block>> blocks;
        int curr_size;                                                 //当前组所含空闲块数量
        static int ID;
        void next_stack_remove();                                      //将下一个组移到当前组并更新curr_size
    public:
        shared_ptr<free_stack> next_stack;
        const int get_id(){ return blocks.top()->get_addr();}
        const int get_curr_size() {return curr_size;}
        free_stack();
        free_stack(shared_ptr<free_stack> next, int size);
        ~free_stack() {while (!blocks.empty()) blocks.pop();}
        void group_link();
        vector<shared_ptr<block>> apply_for_block(int size = 1);
        void add_free_block(shared_ptr<block> blk);
    };
}

#endif //FS_FREE_STACK_H
