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
    uint32 group;

    void touch();

    void useradd();

    void mkdir();
}

#endif //FS_COMMANDS_H
