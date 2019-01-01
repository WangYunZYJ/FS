//
// Created by wy on 19-1-1.
//

#include <include/disk_op/volume.h>
#include <include/constant/constants.h>
#include <fstream>
#include <include/io/io.h>
#include <iostream>
#include <assert.h>

using namespace wyfs;

shared_ptr<volume> volume::instance = nullptr;

shared_ptr<volume> &wyfs::volume::get_instance() {
    if(instance != nullptr)
        instance = shared_ptr<volume>(new volume());
    return instance;
}

volume::volume() {

}

void wyfs::volume::create_disk() {
    ofstream file(DISK_PATH);
    for(long long i = 0; i < (1 << 25); ++i){
        file << 'a';
    }
    file.flush();
    file.close();
}

void wyfs::volume::init(const uint32 block_id) {
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

void wyfs::volume::disk_init_read_test(const uint32 block_id) {
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

std::vector<uint32> volume::apply_for_free_block(uint32 amount) {
    std::vector<uint32> applied_blocks;
    auto _ = io::get_instance();
    uint32 stack_size = 0;
    uint32 pos = FREE_BLOCK_BEGIN * BLOCK_SIZE;
    _->seekg(pos);
    _->read(reinterpret_cast<char*>(&stack_size), sizeof(uint32));
    pos += stack_size * sizeof(uint32);
    while (amount--){
        if(!stack_size){
            apply_error_occurs(applied_blocks, stack_size);
            return applied_blocks;
        }
        uint32 param = 0;
        _->seekg(pos);
        _->read(reinterpret_cast<char*>(&param), sizeof(uint32));
        stack_size --;
        pos -= sizeof(uint32);
        applied_blocks.push_back(param);
        if(!stack_size && param){
            next_stack_remove(stack_size, param, pos);
            _->seekp(pos);
            _->write(reinterpret_cast<char*>(&stack_size), sizeof(uint32));
            pos += sizeof(uint32) * stack_size;
        }else if(!param){
            apply_error_occurs(applied_blocks, stack_size);
            return applied_blocks;
        }
    }
    _->seekp(FREE_BLOCK_BEGIN * BLOCK_SIZE);
    _->write(reinterpret_cast<char*>(&stack_size), sizeof(uint32));
    return applied_blocks;
}

const uint32 volume::release_free_block(uint32 blk) {

    uint32 stack_size = 0;
    auto _ = io::get_instance();
    _->seekg(FREE_BLOCK_BEGIN * BLOCK_SIZE);
    _->read(reinterpret_cast<char*>(&stack_size), sizeof(uint32));
    uint32 pos = FREE_BLOCK_BEGIN * BLOCK_SIZE + sizeof(uint32) * (stack_size + 1);
    if(stack_size < FREE_BLOCK_MAX_COUNT_EACH_STACK){
        _->seekp(pos);
        _->write(reinterpret_cast<char*>(&blk), sizeof(uint32));
        stack_size++;
        _->seekp(FREE_BLOCK_BEGIN * BLOCK_SIZE);
        _->write(reinterpret_cast<char*>(&stack_size), sizeof(uint32));
        pos+= sizeof(uint32);
    }else{
        uint32 new_block_pos = blk * BLOCK_SIZE;
        pos = FREE_BLOCK_BEGIN * BLOCK_SIZE;
        _->seekp(new_block_pos);
        _->write(reinterpret_cast<char*>(&stack_size), sizeof(uint32));
        for(int i = 0; i < stack_size; ++i){
            uint32 param = 0;
            new_block_pos += sizeof(uint32);
            pos += sizeof(uint32);

            _->seekg(pos);
            _->read(reinterpret_cast<char*>(&param), sizeof(uint32));
            _->seekp(new_block_pos);
            _->write(reinterpret_cast<char*>(&param), sizeof(uint32));
        }
        stack_size = 1;
        _->seekp(FREE_BLOCK_BEGIN * BLOCK_SIZE);
        _->write(reinterpret_cast<char*>(&stack_size), sizeof(uint32));
        _->seekp(FREE_BLOCK_BEGIN * BLOCK_SIZE + sizeof(uint32));
        _->write(reinterpret_cast<char*>(&blk), sizeof(uint32));
    }
    return stack_size;
}

void volume::next_stack_remove(uint32& pre_stack_size, const uint32 next_stack,
        const uint32 pre_addr) {
    auto _ = io::get_instance();
    uint32 pos = next_stack * BLOCK_SIZE;
    uint32 stack_size = 0;
    uint32 write_pre_pos = pre_addr;
    bool if_add_zero = 0;

    _->seekg(pos);
    _->read(reinterpret_cast<char*>(&stack_size), sizeof(uint32));
    if(stack_size < FREE_BLOCK_MAX_COUNT_EACH_STACK){
        pre_stack_size = stack_size + 1;
        if_add_zero = 1;
    }else
        pre_stack_size = stack_size;
    if(if_add_zero){
        uint32 end = 0;
        write_pre_pos += sizeof(uint32);

        _->seekp(write_pre_pos);
        _->write(reinterpret_cast<char*>(&end), sizeof(uint32));
    }
    for(uint32 i = 0; i < stack_size; ++i){
        pos += sizeof(uint32);
        write_pre_pos += sizeof(uint32);
        uint32 param = 0;
        _->seekg(pos);
        _->read(reinterpret_cast<char*>(&param), sizeof(uint32));
        _->seekp(write_pre_pos);
        _->write(reinterpret_cast<char*>(&param), sizeof(uint32));
    }
}

void volume::apply_error_occurs(vector<uint32> &applied_blocks, uint32 &stack_size) {
    auto _ = io::get_instance();
    std::cerr << "磁盘空间不足\n";
    _->seekp(FREE_BLOCK_BEGIN * BLOCK_SIZE);
    _->write(reinterpret_cast<char*>(&stack_size), sizeof(uint32));
    for(uint32 i = 0; i < applied_blocks.size(); ++i)
        stack_size = release_free_block(applied_blocks[applied_blocks.size() - 1 - i]);
    applied_blocks.clear();
}
