//
// Created by wy on 19-1-1.
//

#include <include/inode/inode.h>
#include <iostream>
#include <time.h>
#include <cstdio>
//
//wyfs::inode::inode(const uint32 file_mode, const uint32 owner_permision, const uint32 group_member_permision,
//                   const uint32 others_permision, const std::string owner, const uint32 file_size,
//                   const uint32 group, const uint32 inode_id) :file_mode(file_mode),
//                   file_permision({owner_permision, group_member_permision, others_permision}),
//                   owner(owner), file_size(file_size), group(group), inode_id(inode_id){
//
//}
//
//wyfs::inode::inode(const std::string owner, const uint32 group)  :file_mode(FileMode::NORMAL_FILE),
//                   file_permision({7, 7, 5}),owner(owner), file_size(0), group(group){
//    inode_id = 1/*TODO*/;
//}
void wyfs::inode::print_test() {
    printf("文件模式为 %d\n", file_mode);
    printf("文件权限为 %d %d %d\n", file_permision.owner, file_permision.group_menber, file_permision.others);
    printf("文件所有者为%s\n", owner);
    std::cout << "系统时间戳为:";
    std::cout << timestamp << std::endl;
    printf("文件大小为 %d\n", file_size);
    printf("文件所占块数 %d\n", block_count);
    printf("文件所属组 %d\n", group);
    printf("文件的inode编号 %d\n", inode_id);
}