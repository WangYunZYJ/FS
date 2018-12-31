#define CATCH_CONFIG_MAIN
#include "free_stack.h"
#include <iostream>
#include "constant.h"
#include "catch/catch.h"
using namespace wyfs;
using namespace std;

void print_test(shared_ptr<free_stack> ptr)
{
    decltype(ptr) it = ptr;
    for(it = ptr; it != nullptr; it = it->next_stack)
    {
        cout << it->get_id() << ' ' << it->get_curr_size() << endl;
    }
}

TEST_CASE("free_block_link_init", "[group_link]")
{
    auto ptr = make_shared<free_stack>();
    ptr->group_link();
    print_test(ptr);
}

TEST_CASE("apply_for_blocks", "[apply_for_block]")
{
    auto ptr = make_shared<free_stack>();
    ptr->group_link();
    auto vec = ptr->apply_for_block(MAGIC);
//    print_test(ptr);
    for(int i = 0; i < 100; ++i) {
        ptr->add_free_block(vec[0]);
    }
    print_test(ptr);
}