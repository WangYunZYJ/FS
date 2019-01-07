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

        /**
         * 成组链接法空闲栈的移动
         * @param pre_stack_size
         * @param next_stack
         * @param pre_addr
         */
        void next_stack_remove(uint32& pre_stack_size, const uint32 next_stack, const uint32 pre_addr);

        /**
         * 申请空闲块时出错
         * @param applied_blocks
         * @param stack_size
         */
        void apply_error_occurs(vector<uint32>& applied_blocks, uint32& stack_size);

        /**
         * 填充inode结构体
         * @param new_inode
         * @param owner
         * @param group
         * @param fileMode
         * @param fileSize
         * @param filePermision
         */
        uint32 fill_inode_structure(inode &new_inode, char *owner, uint32 group, FileMode fileMode, uint32 fileSize,
                                    FilePermision filePermision);

        /**
         * 分配inode对应文件所需的block,并将多级地址映射写入各级块。
         * @param new_inode
         */
        void alloc_blocks_for_inode(inode& new_inode);

        /**
         * added by wy on 19-1-2
         * 初始化超级块的inode表。
         */
        void init_inode_table();

        /**
         * 初始化成组链接法管理磁盘
         * @param block_id 初始空闲栈块号
         */
        void init_link_disk(const uint32 block_id);


    public:

        /**
         * 获取volume的单例
         * @return volume的单例智能指针
         */
        static shared_ptr<volume>& get_instance();

        /**
         * 创建32MB的磁盘
         */
        void create_disk();

        /**
         * 成组链接法的测试
         * @param block_id 初始空闲块号
         */
        void disk_init_read_test(const uint32 block_id);

        /**
         * 申请空闲块
         * @param amount 空闲块数量
         * @return 返回所有空闲块编号的向量
         */
        std::vector<uint32> apply_for_free_block(uint32 amount);

        /**
         * 根据块号释放块
         * @param blk 所需释放块号
         * @return 释放后对应位置空闲栈内空闲块数量
         */
        const uint32 release_free_block(uint32 blk);

        /**
         * added by wy on 19-1-2 20:00
         * 磁盘初始化
         * 0.
         * 1. 初始化inode表（所有inode的地址信息，以inode_block的格式存储）
         * 2.
         * 3.
         * 4. 成组链接法首个空闲栈
         */
        void disk_init();

        /**
         * added by wy on 19-1-2
         * 创建一个文件，分配文件所需空间，在磁盘inode表中增加inode信息。
         * @param owner 文件所有者
         * @param group 文件所属组
         * @param fileMode 文件格式（目录|普通）
         * @param fileSize 文件大小
         * @param filePermision 文件权限
         */
        uint32 create_file(char *file_name, char *owner, uint32 group, FileMode fileMode = FileMode::NORMAL_FILE,
                           uint32 fileSize = 0, FilePermision filePermision = {7, 7, 5});

        /**
         * added by wy on 19-1-3 20:30
         * @param owner 文件所有者
         * @param group 文件所属组
         * @param fileSize 文件大小
         * @param filePermision 文件权限
         */
        void create_menu_file(char *owner, uint32 group, uint32 fileSize = 0, FilePermision filePermision = {7, 7, 5});

        /**
         * added by wy on 19-1-3 20:30
         * 增加用户后对磁盘的操作
         */
        void add_user_table();

        /**
         * added by wy on 19-1-2 20:00
         * 添加文件后对磁盘信息正确性的检验
         */
        void inode_msg_test();

        /**
         * added by wy on 19-1-2 22:00
         * 返回inode节点对应文件的信息
         * @param _inode 某个inode节点
         * @return 文件数据
         */
        vector<file_msg> read_file_msg(const inode& _inode);

        /**
         * TODO
         * added by wy on 19-1-2 22:00
         * @param path 文件路径
         * @return 文件数据
         */
        vector<file_msg> read_file_msg(char* path);

        /**
         * added by wy on 19-1-2 23:00
         * 将file_msg装配成真正的文件信息并返回
         * @param file
         * @return 返回文件的字符串
         */
        const string decode_file_msg(const vector<file_msg>& file);

        /**
         * 存储树形结构
         */
        void init_root_block();

        void init_username_password();

        void update_tree_lists(uint32 father_inode_addr, uint32 son_inode_addr);

        void init_cache();

        void add_user_pwd(string username, string pwd);

    };
}

#endif //FS_VOLUME_H
