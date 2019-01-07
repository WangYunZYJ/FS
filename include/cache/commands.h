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
    static std::vector<uint32> curr_path({0});
    static std::map<std::string, uint32> file_name2id;
    static std::map<uint32, std::string> file_id2name;
    static std::map<std::string, std::string> name2pwd;
    static char owner[NAME_SIZE];
    static uint32 group;

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
