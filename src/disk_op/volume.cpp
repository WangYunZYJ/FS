/**
 * @author wy
 * @date 19-1-1
 */
#define DEBUG
#include <include/disk_op/volume.h>
#include <include/constant/constants.h>
#include <include/disk_op/inode_block.h>
#include <fstream>
#include <cstring>
#include <include/io/io.h>
#include <iostream>
#include <cassert>
#include <cmath>

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

void wyfs::volume::init_link_disk(const uint32 block_id) {
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
        init_link_disk(block_id + stack_size);
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
        pos += sizeof(uint32);
    }else {
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

/**
 * 用户创建文件
 * @param owner
 * @param group
 * @param fileMode
 * @param fileSize
 */
//uint32 file_mode;
//FilePermision file_permision;
//char owner[14 * 8];
//time_t timestamp;
//uint32 file_size;
//uint32 dirct_block[10];
//uint32 first_block;
//uint32 second_block;
//uint32 third_block;
//uint32 block_count;
//uint32 group;
//uint32 inode_block;
void volume::create_file(char *owner, uint32 group, FileMode fileMode, uint32 fileSize, FilePermision filePermision) {
    inode_block inodeBlock;
    auto _ = io::get_instance();
    uint32 inode_block_pos = INODE_TABLE * BLOCK_SIZE;
    _->seekg(inode_block_pos);
    _->read(reinterpret_cast<char*>(&inodeBlock), sizeof(inode_block));
    while(inodeBlock.inode_size == INODE_COUNT_PER_BLOCK){
        inode_block_pos = inodeBlock.next * BLOCK_SIZE;
        _->seekg(inode_block_pos);
        _->read(reinterpret_cast<char*>(&inodeBlock), sizeof(inode_block));
    }
    /**
     * @param inodeBlock 存在空闲位置的inode表，inode表以链式结构进行管理
     */
    inode new_inode;
    fill_inode_structure(new_inode, owner, group, fileMode, fileSize, filePermision);
    //将inode地址写入inodeBlock
    inodeBlock.ids[inodeBlock.inode_size] = new_inode.inode_id;
    inodeBlock.inode_size++;
    _->seekp(inode_block_pos);
    _->write(reinterpret_cast<char*>(&inodeBlock), sizeof(inode_block));
}

/**
 * 初始化一个inode，并分配对应文件所需的block,并在磁盘写入inode
 * @param new_inode
 * @param owner
 * @param group
 * @param fileMode
 * @param fileSize
 * @param filePermision
 */
void volume::fill_inode_structure(inode &new_inode, char *owner, uint32 group, FileMode fileMode, uint32 fileSize,
                                  FilePermision filePermision) {
    /**
     * @param inode_msg_block 存储当前inode所有信息的地址
     */
    auto inode_msg_block = apply_for_free_block(1);
    new_inode.inode_id = inode_msg_block[0];
    new_inode.group = group;
    new_inode.file_size = fileSize;
    strcpy(new_inode.owner, owner);
    new_inode.file_permision = filePermision;
    new_inode.timestamp = time(nullptr);
    new_inode.file_mode = fileMode;
    new_inode.block_count = (uint32)ceil(fileSize * 1.0 / BLOCK_SIZE);

    alloc_blocks_for_inode(new_inode);

    auto _ = io::get_instance();
    _->seekp(new_inode.inode_id * BLOCK_SIZE);
    _->write(reinterpret_cast<char*>(&new_inode), sizeof(inode));
}

void volume::alloc_blocks_for_inode(inode &new_inode) {
    if(!new_inode.block_count) return;
    auto _ = io::get_instance();
    auto applied_blocks = apply_for_free_block(new_inode.block_count);
    if(new_inode.block_count <= INODE_DIRECT_BLOCK_COUNT){
        for(size_t i = 0; i < applied_blocks.size(); ++i){
            new_inode.dirct_block[i] = applied_blocks[i];
        }
    }
    else if(new_inode.block_count > INODE_DIRECT_BLOCK_COUNT && new_inode.block_count <= INODE_SECOND_BLOCK_COUNT){
        auto applied_first_block = apply_for_free_block(1);
        new_inode.first_block = applied_first_block[0];
        _->seekp(BLOCK_SIZE * applied_first_block[0]);
        for(size_t i = 0; i < applied_blocks.size(); ++i){
            _->write(reinterpret_cast<char*>(&applied_blocks[i]), sizeof(uint32));
        }
    }
    else{
        auto applied_first_block = apply_for_free_block(1);
        auto applied_second_block = apply_for_free_block((uint32)ceil(new_inode.block_count * 1.0 / INODE_SECOND_BLOCK_COUNT));
        new_inode.second_block = applied_first_block[0];
        _->seekp(BLOCK_SIZE * applied_first_block[0]);

        for(size_t i = 0; i < applied_second_block.size(); ++i){
            _->write(reinterpret_cast<char*>(&applied_second_block[i]), sizeof(uint32));
        }
        size_t index = 0;
        for(size_t i = 0; i < applied_second_block.size(); ++i){
            _->seekp(BLOCK_SIZE * applied_second_block[i]);
            for(size_t j = 0;index < applied_blocks.size()&&j<INODE_SECOND_BLOCK_COUNT; ++index, ++j){
                _->write(reinterpret_cast<char*>(applied_blocks[index]), sizeof(uint32));
            }
        }
    }
}

void volume::init_inode_table() {
    inode_block inodeBlock;
    inodeBlock.inode_size = 0;
    inodeBlock.next = 0;
    auto _ = io::get_instance();
    _->seekp(BLOCK_SIZE * INODE_TABLE);
    _->write(reinterpret_cast<char*>(&inodeBlock), sizeof(inode_block));
}

void volume::disk_init() {
    init_inode_table();
    init_link_disk(FREE_BLOCK_BEGIN);
}

void volume::inode_msg_test() {
    auto _ = io::get_instance();
    _->seekg(BLOCK_SIZE * INODE_TABLE);
    inode_block inodeBlock;
    _->read(reinterpret_cast<char*>(&inodeBlock), sizeof(inode_block));
    printf("当前inode表的大小为%d, 下一张inode表地址为%d\n", inodeBlock.inode_size, inodeBlock.next);
    printf("当前inode表中各inode地址如下：\n");
    for(size_t i = 0; i < inodeBlock.inode_size; ++i){
        printf("第%4d各inode其坐标为%8d\n", i, inodeBlock.ids[i]);
        puts("");
        _->seekg(BLOCK_SIZE * inodeBlock.ids[i]);
        inode tmp_inode;
        _->read(reinterpret_cast<char*>(&tmp_inode), sizeof(inode));
        tmp_inode.print_test();
    }
}

vector<file_msg> volume::read_file_msg(const inode &_inode) {
    vector<file_msg> file;
    auto _ = io::get_instance();
    if(_inode.block_count <= INODE_DIRECT_BLOCK_COUNT){
        for(size_t i = 0; i < _inode.block_count; ++i) {
            _->seekg(_inode.dirct_block[i] * BLOCK_SIZE);
            file_msg tmp_msg;
            _->read(reinterpret_cast<char*>(&tmp_msg), sizeof(file_msg));
            file.push_back(tmp_msg);
        }
    }
    else if(_inode.block_count > INODE_DIRECT_BLOCK_COUNT && _inode.block_count <= INODE_SECOND_BLOCK_COUNT) {
        //uint32 first_addr_size = static_cast<uint32 >(ceil(_inode.block_count * 1.0 / INODE_SECOND_BLOCK_COUNT));
        _->seekg(_inode.first_block * BLOCK_SIZE);
        vector<uint32> block_addrs;
        for(size_t i = 0; i < _inode.block_count; ++i) {
            uint32 block_addr;
            _->read(reinterpret_cast<char*>(&block_addr), sizeof(uint32));
            block_addrs.push_back(block_addr);
        }
        for(size_t i = 0; i < block_addrs.size(); ++i) {
            file_msg tmp_msg;
            _->seekg(block_addrs[i]);
            _->read(reinterpret_cast<char*>(&tmp_msg), sizeof(file_msg));
            file.push_back(tmp_msg);
        }
    }
    else {
        uint32 first_addr_size = static_cast<uint32 >(ceil(_inode.block_count * 1.0 / INODE_SECOND_BLOCK_COUNT));
        _->seekg(_inode.second_block * BLOCK_SIZE);
        vector<uint32> second_cache_addr;
        for(size_t i = 0; i < first_addr_size; i++){
            uint32 second_addr;
            _->read(reinterpret_cast<char*>(&second_addr), sizeof(uint32));
            second_cache_addr.push_back(second_addr);
        }
        vector<uint32> final_addrs;
        for (size_t i = 0, count = 0, per_count = 0; i < second_cache_addr.size() && count < _inode.block_count; ++i) {
            per_count = 0;
            _->seekg(second_cache_addr[i] * BLOCK_SIZE);
            while (count < _inode.block_count && per_count < INODE_SECOND_BLOCK_COUNT){
                uint32 final_block_addr;
                _->read(reinterpret_cast<char*>(&final_block_addr), sizeof(uint32));
                final_addrs.push_back(final_block_addr);
            }
        }
        for (size_t i = 0; i < final_addrs.size(); ++i) {
            file_msg tmp_msg;
            _->seekg(final_addrs[i] * BLOCK_SIZE);
            _->read(reinterpret_cast<char*>(&tmp_msg), sizeof(file_msg));
            file.push_back(tmp_msg);
        }
    }
    return file;
}