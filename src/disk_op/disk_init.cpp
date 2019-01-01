//
// Created by wy on 19-1-1.
//

#include <include/disk_op/disk_init.h>
#include <tiff.h>
#include <iostream>
#include <include/constant/constants.h>
#include "io.h"
#include <fstream>

using namespace wyfs;

void wyfs::create_disk() {
    ofstream file(DISK_PATH);
    for(long long i = 0; i < (1 << 25); ++i){
        file << 'a';
    }
    file.flush();
    file.close();
}

void wyfs::init(const uint32 block_id) {
    auto _ = io::get_instance();
    uint32 pos = block_id * BLOCK_SIZE;
    uint32 stack_size = min(MAX_FREE_BLOCK_COUNT - block_id, FREE_BLOCK_MAX_COUNT_EACH_STACK);
    _->seekp(pos);
    _->write(reinterpret_cast<char*>(&stack_size), sizeof (uint32));

    for(uint32 end = block_id + stack_size; end > block_id; end --){
       pos += sizeof (uint32);
       _->seekp(pos);
       _->write(reinterpret_cast<char*>(&end), sizeof(uint32));
    }

    if(block_id + stack_size < MAX_FREE_BLOCK_COUNT)
        init(block_id + stack_size);
}

void wyfs::disk_init_read_test(const uint32 block_id) {
    auto _ = io::get_instance();
    uint32 pos = block_id * BLOCK_SIZE;
    uint32 size = 0;
    _->seekg(pos);
    _->read(reinterpret_cast<char*>(&size), sizeof(uint32));
    std::cout << "size " << size << std::endl;
    uint32 next_block = 0;
    for(uint32 i = 0; i < size; ++i){
        uint32 tmp = 0;
        pos += sizeof(uint32);
        _->seekg(pos);
        _->read(reinterpret_cast<char*>(&tmp), sizeof(uint32));

        if(!next_block) next_block = tmp;
        std::cout << tmp << std::endl;
    }
    cout << std::endl << std::endl << std::endl;
    if(next_block && next_block != MAX_FREE_BLOCK_COUNT)
        disk_init_read_test(next_block);
}

std::vector<uint32> apply_for_free_block(uint32 amount) {
    std::vector<uint32> applied_blocks;


    return applied_blocks;
}
