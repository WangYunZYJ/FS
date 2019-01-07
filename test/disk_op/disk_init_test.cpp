//
// Created by wy on 19-1-1.
//
#define CATCH_CONFIG_MAIN

#include <include/disk_op/volume.h>
#include "catch.h"
#include "constant/constants.h"

using namespace wyfs;

/**
 * 磁盘创建成功
 * 磁盘初始化空块成功
 * 磁盘块读写成功
 */
TEST_CASE("disk init_link_disk", "[disk_init]")
{
    auto _volume = volume::get_instance();
    _volume->disk_init();
}

/**
 * 成组链接法块分配与释放成功
 */
TEST_CASE("apply for block", "[apply_block]")
{
    auto _volume = volume::get_instance();
    auto vec = _volume->apply_for_free_block(10086);
    for(int i = 0; i < vec.size(); ++i)
        _volume->release_free_block(vec[i]);
    _volume->disk_init_read_test(FREE_BLOCK_BEGIN);
}


TEST_CASE("init disk", "[init]")
{
    auto _volume = volume::get_instance();
    _volume->disk_init();
}