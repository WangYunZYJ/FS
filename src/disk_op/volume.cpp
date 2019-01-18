/**
 * @author wy
 * @date 19-1-1
 */
#define DEBUG
#include <include/disk_op/volume.h>
#include <include/constant/constants.h>
#include <fstream>
#include <cstring>
#include <include/io/io.h>
#include <iostream>
#include <cassert>
#include <cmath>
#include <ctime>
#include <include/cache/commands.h>
#include <include/cache/corlor_cout.h>
#include <iomanip>

using namespace wyfs;

shared_ptr<volume> volume::instance = nullptr;

shared_ptr<volume> &wyfs::volume::get_instance() {
    if(instance == nullptr)
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
 * TODO DEBUG
 * @bug inode_block结构提格式不对
 */

uint32 volume::create_file(char *file_name, char *owner, char *group, FileMode fileMode, uint32 fileSize,
                           FilePermision filePermision) {
    username_saved_as_menu inodeBlock;
    auto _ = io::get_instance();
    uint32 inode_block_pos = INODE_TABLE * BLOCK_SIZE;
    _->seekg(inode_block_pos);
    _->read(reinterpret_cast<char*>(&inodeBlock), sizeof(username_saved_as_menu));
    while(inodeBlock.name_counts == INODE_COUNT_PER_BLOCK){
        if(inodeBlock.next_menu_block_addr == 0){
            auto blks = apply_for_free_block(1);
            inodeBlock.next_menu_block_addr = blks[0];
            username_saved_as_menu tmp;
            tmp.name_counts = 0;
            _->seekp(BLOCK_SIZE * blks[0]);
            _->write(reinterpret_cast<char*>(&tmp), sizeof(username_saved_as_menu));
        }
        inode_block_pos = inodeBlock.next_menu_block_addr * BLOCK_SIZE;
        _->seekg(inode_block_pos);
        _->read(reinterpret_cast<char*>(&inodeBlock), sizeof(username_saved_as_menu));
    }
    /**
     * @param inodeBlock 存在空闲位置的inode表，inode表以链式结构进行管理
     */
    inode new_inode;
    uint32 inode_addr = fill_inode_structure(new_inode, owner, group, fileMode, fileSize, filePermision);
    //将inode地址写入inodeBlock
    inodeBlock.fiaddr[inodeBlock.name_counts].inode_addr = new_inode.inode_id;
    strcpy(inodeBlock.fiaddr[inodeBlock.name_counts].filename, file_name);
    inodeBlock.name_counts++;

    _->seekp(inode_block_pos);
    _->write(reinterpret_cast<char*>(&inodeBlock), sizeof(username_saved_as_menu));
    return inode_addr;
}

void volume::init_cache() {
    wyfs::file_name2id["/"] = 0;
    wyfs::file_id2name[0] = "/";

    auto _ = io::get_instance();
    _->seekg(BLOCK_SIZE * MASK_BLOCK);
    _->read(reinterpret_cast<char*>(&wyfs::filePermision), sizeof(filePermision));
    read_all_file_name_id_to_cache();
    read_all_name_pwd_to_cache();

    strcpy(group, "\0");
    strcpy(owner, "\0");
    curr_path = {0};

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
uint32 volume::fill_inode_structure(inode &new_inode, char *owner, char *group, FileMode fileMode, uint32 fileSize,
                                    FilePermision filePermision) {
    /**
     * @param inode_msg_block 存储当前inode所有信息的地址
     */
    auto inode_msg_block = apply_for_free_block(1);
    new_inode.inode_id = inode_msg_block[0];
    strcpy(new_inode.group, group);
    new_inode.file_size = fileSize;
    strcpy(new_inode.owner, owner);
    new_inode.file_permision = filePermision;
    new_inode.timestamp = time(nullptr);
    new_inode.file_mode = fileMode;
    new_inode.block_count = (uint32)ceil(fileSize * 1.0 / BLOCK_SIZE);
    memset(new_inode.dirct_block, 0, INODE_DIRECT_BLOCK_COUNT * sizeof(uint32));
    new_inode.second_block = 0;
    new_inode.first_block = 0;
    alloc_blocks_for_inode(new_inode);

    auto _ = io::get_instance();
    _->seekp(new_inode.inode_id * BLOCK_SIZE);
    _->write(reinterpret_cast<char*>(&new_inode), sizeof(inode));
    return inode_msg_block[0];
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
    username_saved_as_menu inodeBlock;
//    inode_block inodeBlock;
    inodeBlock.name_counts = 0;
    inodeBlock.next_menu_block_addr = 0;
    auto _ = io::get_instance();
    _->seekp(BLOCK_SIZE * INODE_TABLE);
    _->write(reinterpret_cast<char*>(&inodeBlock), sizeof(username_saved_as_menu));
}

void volume::disk_init() {
    init_root_block();
    init_inode_table();
    init_username_password();
    init_link_disk(FREE_BLOCK_BEGIN);
}

//void volume::inode_msg_test() {
//    auto _ = io::get_instance();
//    _->seekg(BLOCK_SIZE * INODE_TABLE);
//    inode_block inodeBlock;
//    _->read(reinterpret_cast<char*>(&inodeBlock), sizeof(inode_block));
//    printf("当前inode表的大小为%d, 下一张inode表地址为%d\n", inodeBlock.inode_size, inodeBlock.next);
//    printf("当前inode表中各inode地址如下：\n");
//    for(size_t i = 0; i < inodeBlock.inode_size; ++i){
//        printf("第%4d各inode其坐标为%8d\n", i, inodeBlock.ids[i]);
//        puts("");
//        _->seekg(BLOCK_SIZE * inodeBlock.ids[i]);
//        inode tmp_inode;
//        _->read(reinterpret_cast<char*>(&tmp_inode), sizeof(inode));
//        tmp_inode.print_test();
//    }
//}

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
        uint32 first_addr_size = static_cast<uint32>(ceil(_inode.block_count * 1.0 / INODE_SECOND_BLOCK_COUNT));
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
                per_count++;
                count++;
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

vector<file_msg> volume::read_file_msg(uint32 inode_addr) {
    auto _ = io::get_instance();
    inode tmp_inode;
    _->seekg(BLOCK_SIZE * inode_addr);
    _->read(reinterpret_cast<char*>(&tmp_inode), sizeof(inode));
    return read_file_msg(tmp_inode);
}

const string volume::decode_file_msg(const vector<file_msg> &file) {
    std::__cxx11::string file_content = "";
    for_each(file.begin(), file.end(), [&](file_msg msg) {
        for (size_t i = 0; i < msg.msg_length; ++i) {
            file_content += msg.byte[i];
        }
    });
    return file_content;
}

void volume::init_root_block() {
    tree_list treeList;
    treeList.sons_size = 0;
    auto _ = io::get_instance();
    _->seekp(TREE_STRUCT * BLOCK_SIZE);
    _->write(reinterpret_cast<char*>(&treeList), sizeof(tree_list));
}

void volume::update_tree_lists(uint32 father_inode_addr, uint32 son_inode_addr) {

    if(!father_inode_addr){
        auto _ = io::get_instance();
        tree_list treeList;
        _->seekg(father_inode_addr * BLOCK_SIZE);
        _->read(reinterpret_cast<char*>(&treeList), sizeof(tree_list));
        treeList.sons_inode_addr[treeList.sons_size] = son_inode_addr;
        treeList.sons_size ++;
        _->seekp(father_inode_addr * BLOCK_SIZE);
        _->write(reinterpret_cast<char*>(&treeList), sizeof(tree_list));
        return;
    }

    auto _ = io ::get_instance();
    inode father_inode;
    _->seekg(BLOCK_SIZE * father_inode_addr);
    _->read(reinterpret_cast<char*>(&father_inode), sizeof(inode));
    tree_list treeList;
    treeList.sons_size = 0;
    if(father_inode.block_count == 1){
        _->seekg(father_inode.dirct_block[0] * BLOCK_SIZE);
        _->read(reinterpret_cast<char*>(&treeList), sizeof(tree_list));
    }else{
        auto blocks = apply_for_free_block(1);
        father_inode.block_count = 1;
        father_inode.dirct_block[0] = blocks[0];
    }
    treeList.sons_inode_addr[treeList.sons_size] = son_inode_addr;
    treeList.sons_size++;

    _->seekp(BLOCK_SIZE * father_inode.dirct_block[0]);
    _->write(reinterpret_cast<char*>(&treeList), sizeof(tree_list));
    _->seekp(BLOCK_SIZE * father_inode_addr);
    _->write(reinterpret_cast<char*>(&father_inode), sizeof(inode));
}

void volume::init_username_password() {
    auto _ = io::get_instance();
    username_password usernamePassword;
    usernamePassword.user_counts = 0;
    _->seekp(USERNAME_PASSWORD * BLOCK_SIZE);
    _->write(reinterpret_cast<char*>(&usernamePassword), sizeof(username_password));
}

void volume::add_user_pwd(string username, string pwd) {
    auto _ = io::get_instance();
    username_password usernamePassword;
    _->seekg(USERNAME_PASSWORD * BLOCK_SIZE);
    _->read(reinterpret_cast<char*>(&usernamePassword), sizeof(username_password));
    //std::clog << "usernamePassword: " << usernamePassword.user_counts << endl;
    strcpy(usernamePassword.users[usernamePassword.user_counts].username, username.c_str());
    strcpy(usernamePassword.users[usernamePassword.user_counts].password, pwd.c_str());
    usernamePassword.user_counts++;
    _->seekp(USERNAME_PASSWORD * BLOCK_SIZE);
    _->write(reinterpret_cast<char*>(&usernamePassword), sizeof(username_password));
}

vector<uint32> volume::get_sons_inode_addr(const vector<uint32> &inode_path) {
    vector<uint32> sons_inode_addr;
    auto _ = io::get_instance();
    tree_list treeList;
    _->seekg(TREE_STRUCT * BLOCK_SIZE);
    _->read(reinterpret_cast<char*>(&treeList), sizeof(tree_list));
    if(inode_path.size() == 1){
        for(size_t i = 0; i < treeList.sons_size; ++i) {
            sons_inode_addr.push_back(treeList.sons_inode_addr[i]);
        }
    }else{
        inode curr_dir_inode;
        _->seekg(BLOCK_SIZE * inode_path[inode_path.size() - 1]);
        _->read(reinterpret_cast<char*>(&curr_dir_inode), sizeof(inode));
        if(curr_dir_inode.block_count == 0){
            sons_inode_addr.clear();
            return sons_inode_addr;
        }
        _->seekg(BLOCK_SIZE * curr_dir_inode.dirct_block[0]);
        _->read(reinterpret_cast<char*>(&treeList), sizeof(tree_list));
        for(size_t i = 0; i < treeList.sons_size; ++i) {
            sons_inode_addr.push_back(treeList.sons_inode_addr[i]);
        }
    }
    return sons_inode_addr;
}

bool volume::is_normal_file(uint32 inode_addr) {
    auto _ = io::get_instance();
    inode file_inode;
    _->seekg(inode_addr * BLOCK_SIZE);
    _->read(reinterpret_cast<char*>(&file_inode), sizeof(inode));
    if(file_inode.file_mode == FileMode::NORMAL_FILE)
        return true;
    return false;
}


void volume::read_all_file_name_id_to_cache() {
    auto _ = io::get_instance();
    username_saved_as_menu usernameSavedAsMenu;
    _->seekg(BLOCK_SIZE * INODE_TABLE);
    _->read(reinterpret_cast<char *>(&usernameSavedAsMenu), sizeof(username_saved_as_menu));
    while (true) {
        for (size_t i = 0; i < usernameSavedAsMenu.name_counts; ++i) {
            wyfs::file_id2name_full[usernameSavedAsMenu.fiaddr[i].inode_addr] = usernameSavedAsMenu.fiaddr[i].filename;
            wyfs::file_name2id[usernameSavedAsMenu.fiaddr[i].filename] = usernameSavedAsMenu.fiaddr[i].inode_addr;
            wyfs::file_id2name[usernameSavedAsMenu.fiaddr[i].inode_addr] = wyfs::get_single_name(string(usernameSavedAsMenu.fiaddr[i].filename));
        }
        if(!usernameSavedAsMenu.next_menu_block_addr)
            break;
        _->seekg(BLOCK_SIZE * usernameSavedAsMenu.next_menu_block_addr);
        _->read(reinterpret_cast<char *>(&usernameSavedAsMenu), sizeof(username_saved_as_menu));
    }
}

void volume::read_all_name_pwd_to_cache() {
    auto _ = io::get_instance();
    username_password usernamePassword;
    _->seekg(BLOCK_SIZE * USERNAME_PASSWORD);
    _->read(reinterpret_cast<char*>(&usernamePassword), sizeof(username_password));
    for(size_t i = 0; i < usernamePassword.user_counts; ++i) {
        wyfs::name2pwd[usernamePassword.users[i].username] = usernamePassword.users[i].password;
    }
}

bool volume::update_user_pwd(string username, string pwd) {
    auto _ = io::get_instance();
    username_password usernamePassword;
    _->seekg(USERNAME_PASSWORD * BLOCK_SIZE);
    _->read(reinterpret_cast<char*>(&usernamePassword), sizeof(username_password));
    for(size_t i = 0; i < usernamePassword.user_counts; ++i) {
        if(usernamePassword.users[i].username == username) {
            strcpy(usernamePassword.users[i].password, pwd.c_str());
            _->seekp(USERNAME_PASSWORD * BLOCK_SIZE);
            _->write(reinterpret_cast<char*>(&usernamePassword), sizeof(username_password));
            return true;
        }
    }
    return false;
}

std::vector<inode> volume::get_sub_inodes(uint32 father_inode_addr) {
    vector<inode> inodeVec;
    auto _ = io::get_instance();

    if(!father_inode_addr){
        auto _ = io::get_instance();
        tree_list treeList;
        _->seekg(father_inode_addr * BLOCK_SIZE);
        _->read(reinterpret_cast<char*>(&treeList), sizeof(tree_list));
        for(size_t i = 0; i < treeList.sons_size; ++i) {
            inode tmpInode;
            _->seekg(BLOCK_SIZE * treeList.sons_inode_addr[i]);
            _->read(reinterpret_cast<char*>(&tmpInode), sizeof(inode));
            inodeVec.push_back(tmpInode);
        }
        return inodeVec;
    }

    inode father_inode;
    _->seekg(BLOCK_SIZE * father_inode_addr);
    _->read(reinterpret_cast<char*>(&father_inode), sizeof(inode));
    tree_list treeList;
    treeList.sons_size = 0;
    if(father_inode.block_count == 1){
        _->seekg(father_inode.dirct_block[0] * BLOCK_SIZE);
        _->read(reinterpret_cast<char*>(&treeList), sizeof(tree_list));
    }else{
        auto blocks = apply_for_free_block(1);
        father_inode.block_count = 1;
        father_inode.dirct_block[0] = blocks[0];
    }

    for(size_t i = 0; i < treeList.sons_size; ++i) {
        inode tmpInode;
        _->seekg(BLOCK_SIZE * treeList.sons_inode_addr[i]);
        _->read(reinterpret_cast<char*>(&tmpInode), sizeof(inode));
        inodeVec.push_back(tmpInode);
    }

    return inodeVec;
}

void volume::print_inode_msg(uint32 father_inode_addr) {
    auto inodeVec = get_sub_inodes(father_inode_addr);
    char permission[] = {'r', 'w', 'x'};

    for(size_t i = 0; i < inodeVec.size(); ++i) {
        inode tmpInode = inodeVec[i];
        auto localtm = gmtime(&tmpInode.timestamp);
        cwhite << (tmpInode.file_mode == FileMode::NORMAL_FILE ? '-' : 'd');

        cwhite << ((tmpInode.file_permision.owner >> 2) & 1 ? 'r' : '-');
        cwhite << ((tmpInode.file_permision.owner >> 1) & 1 ? 'w' : '-');
        cwhite << ((tmpInode.file_permision.owner >> 0) & 1 ? 'x' : '-');

        cwhite << ((tmpInode.file_permision.group_menber >> 2) & 1 ? 'r' : '-');
        cwhite << ((tmpInode.file_permision.group_menber >> 1) & 1 ? 'w' : '-');
        cwhite << ((tmpInode.file_permision.group_menber >> 0) & 1 ? 'x' : '-');

        cwhite << ((tmpInode.file_permision.others >> 2) & 1 ? 'r' : '-');
        cwhite << ((tmpInode.file_permision.others >> 1) & 1 ? 'w' : '-');
        cwhite << ((tmpInode.file_permision.others >> 0) & 1 ? 'x' : '-');
        cwhite << ' ';

        if(tmpInode.file_mode == FileMode::MENU_FILE)
            cwhite << get_sub_inodes(tmpInode.inode_id).size();
        else {
            if(tmpInode.file_mode == FileMode::NORMAL_FILE) {
                cwhite << 1;
            }
            else {
                vector<uint32 > tmp = {0, 0, tmpInode.inode_id};
                cwhite << get_sons_inode_addr(tmp).size();
            }
        }

        cwhite << ' ';
        cwhite << tmpInode.owner << ' ';
        cwhite << tmpInode.group << ' ';

        cwhite << ' ' << get_sum_son_blocks_count(inodeVec[i].inode_id) << ' ';
        cwhite << localtm->tm_mon + 1 << ' ' << localtm->tm_mday << ' ' << localtm->tm_hour << ':' << localtm->tm_min << ' ';
        cgreen << wyfs::file_id2name[tmpInode.inode_id] << endl;

    }
}

uint32 volume::get_sum_son_blocks_count(uint32 father_inode_addr) {
    uint32 sum_blocks = 0;
    inode tmp_inode;
    auto _ = io::get_instance();
    _->seekg(BLOCK_SIZE * father_inode_addr);
    _->read(reinterpret_cast<char*>(&tmp_inode), sizeof(inode));
    if(tmp_inode.file_mode == FileMode::NORMAL_FILE)
        return tmp_inode.block_count;
    else {
        sum_blocks += 2;
        tree_list treeList;
        treeList.sons_size = 0;
        if(tmp_inode.block_count == 1){
            _->seekg(tmp_inode.dirct_block[0] * BLOCK_SIZE);
            _->read(reinterpret_cast<char*>(&treeList), sizeof(tree_list));
        }else{
            auto blocks = apply_for_free_block(1);
            tmp_inode.block_count = 1;
            tmp_inode.dirct_block[0] = blocks[0];
        }
        for(uint32 i = 0; i < treeList.sons_size; ++i){
            sum_blocks += get_sum_son_blocks_count(treeList.sons_inode_addr[i]);
        }
    }
    return sum_blocks;
}

void volume::release_dir_or_file(uint32 inode_addr, uint32 father_inode_addr) {
    auto _ = io::get_instance();
    inode tmp_inode;
    _->seekg(inode_addr * BLOCK_SIZE);
    _->read(reinterpret_cast<char*>(&tmp_inode), sizeof(inode));
    if(tmp_inode.file_mode == FileMode::NORMAL_FILE) {
        //TODO Release all file_blocks
    }else {
        //TODO release all file_blocks
    }
    inode father_inode;
    _->seekg(BLOCK_SIZE * father_inode_addr);
    _->read(reinterpret_cast<char*>(&father_inode), sizeof(inode));

    tree_list treeList;

    _->seekg(father_inode.dirct_block[0] * BLOCK_SIZE);
    _->read(reinterpret_cast<char*>(&treeList), sizeof(tree_list));

    for(size_t i = 0; i < treeList.sons_size; ++i) {
        if(treeList.sons_inode_addr[i] == inode_addr) {
            for(size_t j = i + 1; j < treeList.sons_size; ++j)
                treeList.sons_inode_addr[j - 1] = treeList.sons_inode_addr[j];
            break;
        }
    }
    treeList.sons_size--;
    _->seekp(BLOCK_SIZE * father_inode.dirct_block[0]);
    _->write(reinterpret_cast<char*>(&treeList), sizeof(tree_list));

    username_saved_as_menu usernameSavedAsMenu;
    _->seekg(BLOCK_SIZE * INODE_TABLE);
    _->read(reinterpret_cast<char*>(&usernameSavedAsMenu), sizeof(username_saved_as_menu));
    bool flag = true;
    uint32 pos = BLOCK_SIZE * INODE_TABLE;
    while(flag) {
        for(size_t i = 0; i < usernameSavedAsMenu.name_counts; ++i) {
            if(usernameSavedAsMenu.fiaddr[i].inode_addr == inode_addr) {
                for(size_t j = i+1; j < usernameSavedAsMenu.name_counts; ++j) {
                    usernameSavedAsMenu.fiaddr[j - 1].inode_addr = usernameSavedAsMenu.fiaddr[j].inode_addr;
                    strcpy(usernameSavedAsMenu.fiaddr[j - 1].filename, usernameSavedAsMenu.fiaddr[j].filename);
                }
                _->seekp(pos);
                _->write(reinterpret_cast<char*>(&usernameSavedAsMenu), sizeof(username_saved_as_menu));
                flag = false;
            }
        }
        if(!usernameSavedAsMenu.next_menu_block_addr) flag = false;
        else {
            _->seekg(BLOCK_SIZE * usernameSavedAsMenu.next_menu_block_addr);
            pos = BLOCK_SIZE * usernameSavedAsMenu.next_menu_block_addr;
            _->read(reinterpret_cast<char*>(&usernameSavedAsMenu), sizeof(username_saved_as_menu));
        }
    }

}

void volume::echo_msg_to_file(uint32 inode_addr, string msg) {

    uint32 msg_size = msg.length();
    inode new_inode;
    uint32 final_pos = BLOCK_SIZE * inode_addr;
    auto _ = io::get_instance();
    _->seekg(BLOCK_SIZE * inode_addr);
    _->read(reinterpret_cast<char*>(&new_inode), sizeof(inode));

    if(new_inode.file_mode == FileMode::LINK_FILE) {
        _->seekp(BLOCK_SIZE * new_inode.inode_id);
        _->read(reinterpret_cast<char*>(&new_inode), sizeof(inode));
        final_pos = BLOCK_SIZE * new_inode.inode_id;
    }

    new_inode.block_count = ceil(msg_size * 1.0 / BLOCK_SIZE);
    auto applied_blocks = apply_for_free_block(ceil(msg_size * 1.0/ BLOCK_SIZE));
    size_t msg_index = 0;
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

        //_->write(reinterpret_cast<char*>(&applied_second_block.size()), sizeof(uint32));
        for(size_t i = 0; i < applied_second_block.size(); ++i){
            _->write(reinterpret_cast<char*>(&applied_second_block[i]), sizeof(uint32));
        }
         size_t index = 0;
        for(size_t i = 0; i < applied_second_block.size(); ++i){
            _->seekp(BLOCK_SIZE * applied_second_block[i]);
            for(size_t j = 0;index < applied_blocks.size()&&j<INODE_SECOND_BLOCK_COUNT; ++index, ++j){
                _->write(reinterpret_cast<char*>(&applied_blocks[index]), sizeof(uint32));
            }
        }
    }
    for(size_t i = 0; i < applied_blocks.size(); ++i) {
        file_msg fileMsg;
        fileMsg.msg_length = min(BLOCK_SIZE - sizeof(uint32), msg.length() - msg_index);
        strcpy(fileMsg.byte, msg.substr(msg_index, fileMsg.msg_length).c_str());
        msg_index += min(BLOCK_SIZE - sizeof(uint32), msg.length() - msg_index);
        _->seekp(BLOCK_SIZE * applied_blocks[i]);
        _->write(reinterpret_cast<char*>(&fileMsg), sizeof(file_msg));
    }
    _->seekp(final_pos);
    _->write(reinterpret_cast<char*>(&new_inode), sizeof(inode));
}

string volume::get_msg(uint32 inode_addr) {

    inode tmp_inode;
    auto _ = io :: get_instance();
    _->seekg(BLOCK_SIZE * inode_addr);
    _->read(reinterpret_cast<char*>(&tmp_inode), sizeof(inode));
    if(tmp_inode.file_mode == FileMode::LINK_FILE)
        inode_addr = tmp_inode.inode_id;
    auto file = read_file_msg(inode_addr);
    return decode_file_msg(file);
}

void volume::chmod(uint32 inode_addr, const string &permission) {
    auto _ = io::get_instance();
    inode tmp_inode;
    _->seekg(BLOCK_SIZE * inode_addr);
    _->read(reinterpret_cast<char*>(&tmp_inode), sizeof(inode));
    tmp_inode.file_permision.owner = permission[0] - '0';
    tmp_inode.file_permision.group_menber = permission[1] - '0';
    tmp_inode.file_permision.others = permission[2] - '0';
    _->seekp(BLOCK_SIZE * inode_addr);
    _->write(reinterpret_cast<char*>(&tmp_inode), sizeof(inode));
}

void volume::chown(const uint32 inode_addr, const string &owner) {
    auto _ = io::get_instance();
    inode tmp_inode;
    _->seekg(BLOCK_SIZE * inode_addr);
    _->read(reinterpret_cast<char*>(&tmp_inode), sizeof(inode));
    strcpy(tmp_inode.owner, owner.c_str());
    _->seekp(BLOCK_SIZE * inode_addr);
    _->write(reinterpret_cast<char*>(&tmp_inode), sizeof(inode));
}

void volume::link(uint32 link_cp_addr, uint32 src_addr) {
    auto _ = io::get_instance();
    inode link_cp;
    _->seekg(BLOCK_SIZE * link_cp_addr);
    _->read(reinterpret_cast<char*>(&link_cp), sizeof(inode));

    link_cp.inode_id = src_addr;

    _->seekp(BLOCK_SIZE * link_cp_addr);
    _->write(reinterpret_cast<char*>(&link_cp), sizeof(inode));
}

bool volume::rename(uint32 inode_addr, std::string file_name) {
    username_saved_as_menu inodeBlock;
    auto _ = io::get_instance();
    uint32 inode_block_pos = INODE_TABLE * BLOCK_SIZE;
    _->seekg(inode_block_pos);
    _->read(reinterpret_cast<char*>(&inodeBlock), sizeof(username_saved_as_menu));
    while(inodeBlock.next_menu_block_addr) {
        for(auto i = 0; i < inodeBlock.name_counts; ++i) {
            if(inodeBlock.fiaddr[i].inode_addr == inode_addr) {
                strcpy(inodeBlock.fiaddr[i].filename, file_name.c_str());
                _->seekp(inode_block_pos);
                _->write(reinterpret_cast<char*>(&inodeBlock), sizeof(username_saved_as_menu));
                return true;
            }
        }
        inode_block_pos = BLOCK_SIZE * inodeBlock.next_menu_block_addr;
        _->seekg(inode_block_pos);
        _->read(reinterpret_cast<char*>(&inodeBlock), sizeof(username_saved_as_menu));
    }
    return false;
}

void volume::save_mask(FilePermision filePermision) {
    auto _ = io::get_instance();
    _->seekg(MASK_BLOCK * BLOCK_SIZE);
    _->write(reinterpret_cast<char*>(&filePermision), sizeof(FilePermision));
}
