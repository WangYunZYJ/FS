//
// Created by wy on 19-1-7.
//

#ifndef FS_COMMANDS_H
#define FS_COMMANDS_H

#include <vector>
#include <tiff.h>
#include <map>
#include <include/constant/constants.h>

namespace wyfs{
    extern std::vector<uint32> curr_path;
    extern std::map<std::string, uint32> file_name2id;
    extern std::map<uint32, std::string> file_id2name;
    extern std::map<std::string, std::string> name2pwd;
    extern char owner[];
    extern uint32 group;

    void update_cache(std::string filename, uint32 file_inode_addr);

    std::vector<uint32> split_path_string(std::string path);

    std::string union_uint2path(const std::vector<uint32>& pathVec);

    void print_curr_path();

    void touch();

    void useradd();

    void mkdir();

    void cd();

    void ls();
}

#endif //FS_COMMANDS_H
