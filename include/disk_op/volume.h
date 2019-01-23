/**
 * @author wy
 * @date 19-1-1
 */

#ifndef FS_VOLUME_H
#define FS_VOLUME_H

#include <include/inode/inode.h>
#include <memory>
#include <algorithm>
#include <tiff.h>
#include <vector>
#include <include/disk_op/data_structs.h>
#include <include/cache/commands.h>

using namespace std;
namespace wyfs
{
//    uint32 file_mode;
//    FilePermision file_permision;
//    char owner[12*8];
//    time_t timestamp;
//    uint32 file_size;
//    uint32 dirct_block[10];
//    uint32 first_block;
//    uint32 second_block;
//    uint32 third_block;
//    uint32 block_count;
//    uint32 group;
//    uint32 inode_block;

    class volume
    {
        static shared_ptr<volume> instance;
        volume();

        void next_stack_remove(uint32& pre_stack_size, const uint32 next_stack, const uint32 pre_addr);

        void apply_error_occurs(vector<uint32>& applied_blocks, uint32& stack_size);

        uint32 fill_inode_structure(inode &new_inode, char *owner, char *group, FileMode fileMode, uint32 fileSize,
                                    FilePermision filePermision);

        void alloc_blocks_for_inode(inode& new_inode);

        void init_inode_table();

        void init_link_disk(const uint32 block_id);

        std::vector<inode> get_sub_inodes(uint32 father_inode_addr);
    public:

        static shared_ptr<volume>& get_instance();

        void create_disk();

        void disk_init_read_test(const uint32 block_id);

        std::vector<uint32> apply_for_free_block(uint32 amount);

        const uint32 release_free_block(uint32 blk);

        void disk_init();

        uint32 create_file(char *file_name, char *owner, char *group, FileMode fileMode = FileMode::NORMAL_FILE,
                           uint32 fileSize = 0, FilePermision filePermision = wyfs::filePermision);

        void create_menu_file(char *owner, uint32 group, uint32 fileSize = 0, FilePermision filePermision = {7, 7, 5});

        void add_user_table();

        void inode_msg_test();

        vector<file_msg> read_file_msg(const inode& _inode);

        vector<file_msg> read_file_msg(uint32 inode_id);

        const string decode_file_msg(const vector<file_msg>& file);

        void init_root_block();

        void init_username_password();

        void update_tree_lists(uint32 father_inode_addr, uint32 son_inode_addr);

        void init_cache();

        void read_all_file_name_id_to_cache();

        void read_all_name_pwd_to_cache();

        void add_user_pwd(string username, string pwd);

        bool update_user_pwd(string username, string pwd);

        vector<uint32> get_sons_inode_addr(const vector<uint32> &inode_path);

        bool is_normal_file(uint32 inode_addr);

        void print_inode_msg(vector<uint32> &father_inode_addr);

        uint32 get_sum_son_blocks_count(uint32 father_inode_addr);

        vector<uint32> release_blocks(uint32 inode_addr, uint32 father_inode_addr);

        void release_dir_or_file(uint32 inode_addr, uint32 father_inode_addr);

        void echo_msg_to_file(uint32 inode_addr, string msg);

        string get_msg(uint32 inode_addr);

        void chmod(const uint32 inode_addr, const string& permission);

        void chown(const uint32 inode_addr, const string& owner);

        void chgrp(const uint32 inode_addr, const string& grp);

        void link(uint32 link_cp_addr, uint32 src_addr);

        bool rename(uint32 inode_addr, std::string file_name);

        void save_mask(FilePermision filePermision);

        void superblock_print_test();

        std::vector<uint32> get_file_blocks(const inode& tmp_inode);

        uint32 calculate_all_blocks(uint32 inode_addr);

        void print_inode();

        void update_treelist_for_mv(uint32 inode_addr, uint32 father_inode_addr);
    };
}

#endif //FS_VOLUME_H
