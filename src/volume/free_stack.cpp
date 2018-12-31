#include "free_stack.h"
#include "constant.h"
#include <assert.h>
#include <iostream>
using namespace wyfs;

int free_stack::ID = 0;

free_stack::free_stack()
{
    curr_size = 0;
    for(auto i = 0; i < MAX_STACK_FREE_SIZE && ID < BLOCK_AMOUNT; ++i)
    {
        blocks.push(make_shared<block>(ID));
        ID++;
        curr_size++;
    }
}


free_stack::free_stack(shared_ptr<free_stack> next, int size)
:next_stack(next), curr_size(size)
{

}

void free_stack::group_link()
{
    next_stack = make_shared<free_stack>();
    while(ID < BLOCK_AMOUNT)
    {
        next_stack->group_link();
    }
}


void free_stack::next_stack_remove()
{
    assert(next_stack.get());
    while(!next_stack->blocks.empty())
    {
        auto tmp = next_stack->blocks.top();
        next_stack->blocks.pop();
        curr_size++;
        blocks.push(tmp);
    }
    next_stack = next_stack->next_stack;
}

/// 申请块，默认一块
/// \param size
/// \return
vector<shared_ptr<block>> free_stack::apply_for_block(int size)
{
    vector<shared_ptr<block>> returnVal;
    while(size--)
    {
        if(blocks.empty())
        {
            if(next_stack.get() == 0)
            {
                std::cerr << "磁盘空间不足\n";
                for (int i = 0; i < returnVal.size(); ++i) {
                    add_free_block(returnVal[i]);
                }
                returnVal.clear();
                return returnVal;
            }
            else
                next_stack_remove();
        }
        auto param = blocks.top();
        blocks.pop();
        curr_size--;
        returnVal.push_back(param);
    }
    return returnVal;
}

void free_stack::add_free_block(shared_ptr<block> blk)
{
    curr_size++;
    if(curr_size > MAX_STACK_FREE_SIZE)
    {
        auto next = make_shared<free_stack>(next_stack, MAX_STACK_FREE_SIZE);
        while (!blocks.empty())
        {
            auto tmp = blocks.top();
            blocks.pop();
            next->blocks.push(tmp);
        }
        curr_size -= MAX_STACK_FREE_SIZE;
        next_stack = next;
    }
    blocks.push(blk);
}
