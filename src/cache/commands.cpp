//
// Created by wy on 19-1-7.
//

#include <include/cache/commands.h>
#include <include/disk_op/volume.h>
#include <iostream>

void wyfs::touch() {

    if(curr_path.size() <= 1) {
        cerr << "Permission Denied!\n";
        return;
    }

    auto _volume = volume::get_instance();
    char file_name[NAME_SIZE];
    cin >> file_name;
    uint32 file_inode_addr = _volume->create_file(file_name, owner, group);
    _volume->update_tree_lists(curr_path[curr_path.size() - 1], file_inode_addr);
}

void wyfs::useradd() {

    auto _volume = volume::get_instance();
    char username[NAME_SIZE];
    char password[NAME_SIZE];
    cin >> username;
    cout << "请输入用户" << username << "的口令： ";
    cin >> password;

    _volume->add_user_pwd(username, password);
    //TODO mkdir
}

void wyfs::mkdir() {
    if(curr_path.size() <= 1) {
        cerr << "Permission Denied!\n";
        return;
    }
    auto _volume = volume::get_instance();
}

