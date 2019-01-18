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
