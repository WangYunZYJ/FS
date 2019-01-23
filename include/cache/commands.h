//
// Created by wy on 19-1-7.
//

#ifndef FS_COMMANDS_H
#define FS_COMMANDS_H

#include <vector>
#include <tiff.h>
#include <map>
#include <include/constant/constants.h>
#include <include/inode/inode.h>

namespace wyfs{
    extern std::vector<uint32> curr_path;
    extern std::map<std::string, uint32> file_name2id;
    extern std::map<uint32, std::string> file_id2name;
    extern std::map<std::string, std::string> name2pwd;
    extern std::map<uint32, std::string> file_id2name_full;
    extern char owner[];
    extern char group[];
    extern FilePermision filePermision;
    extern uint32 curr_file_each_line;

    std::string get_single_name(std::string path);

    void update_cache(std::string filename, uint32 file_inode_addr);

    std::vector<uint32> split_path_string(std::string path);

    std::string union_uint2path(const std::vector<uint32>& pathVec);

    std::string get_full_name(const std::vector<uint32>& pathVec);

    std::string get_full_name(const std::string path, const std::vector<uint32> &pathVec);

    std::string get_father_path(std::string path);

    void create_file(uint32 blk_count);

    void print_curr_path();

    void touch();

    void useradd();

    void firstUseradd();

    void mkdir();

    void cd();

    void ls();

    void pwd();

    void passwd();

    void rm();

    void mv();

    void cp();

    void echo();

    void chmod();

    void chown();

    void chgrp();

    void unmask();

    void ln();

    void cat();

    void login();

    void fill();

    void superblock();

    void file();
}

#endif //FS_COMMANDS_H
