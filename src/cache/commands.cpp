//
// Created by wy on 19-1-7.
//

#include <include/cache/commands.h>
#include <include/disk_op/volume.h>
#include <iostream>
#include <iomanip>

std::vector<uint32> wyfs::split_path_string(std::string path) {
    string cpyPath = path;
    vector<uint32> pathVec;
    if(path[0] == '/'){
        pathVec.push_back(0);
        path = path.substr(1);
    }
    string::size_type p = path.find_first_of('/');
    while(p != string::npos){
        string dirName = path.substr(0, p);
        path = path.substr(p);
        if(file_name2id[dirName]) {
            pathVec.push_back(file_name2id[dirName]);
        }else{
            std::cerr << "cd: " << cpyPath << ": 没有那个文件或目录\n";
            pathVec.clear();
            return pathVec;
        }
    }
    if(file_name2id[path]) {
        pathVec.push_back(file_name2id[path]);
    }else{
        std::cerr << "cd: " << cpyPath << ": 没有那个文件或目录\n";
        pathVec.clear();
        return pathVec;
    }
    auto _volume = volume::get_instance();
    if(!_volume->is_normal_file(pathVec[pathVec.size() - 1])){
        std::cerr << "cd: " << cpyPath << ": 不是目录\n";
        pathVec.clear();
    }
    return pathVec;
}

std::string wyfs::union_uint2path(const std::vector<uint32> &pathVec) {
    string path = "/";
    for(size_t i = 1; i < pathVec.size(); ++i) {
        path += file_id2name[pathVec[i]];
        path += "/";
    }
    path += "$ ";
    return path;
}

void wyfs::print_curr_path() {
    string path = union_uint2path(curr_path);
    cout << "\033[32m" << path;
}

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
    char file_name[NAME_SIZE];
    cin >> file_name;
    uint32 file_inode_addr = _volume->create_file(file_name, owner, group, FileMode::MENU_FILE);
    _volume->update_tree_lists(curr_path[curr_path.size() - 1], file_inode_addr);
}

void wyfs::cd() {
    auto _volume = volume::get_instance();
    string path;
    cin >> path;
    if(path == "."){
        return;
    }else if(path == ".."){
        auto it = curr_path.end();
        --it;
        curr_path.erase(it);
    }else{
        auto pathVec = split_path_string(path);
        if(!path.size()) return;
        else curr_path = pathVec;
    }
}

void wyfs::ls() {
    auto _volume = volume::get_instance();
    vector<uint32> sons = _volume->get_sons_inode_addr(curr_path);
    uint32 index = 2;
    cout << "\033[32" << setw(30) << ".";
    cout << "\033[32" << setw(30) << "..";
    for(size_t i = 0; i < sons.size(); ++i) {
        index ++;
        cout << "\033[32" << setw(30) << file_id2name[sons[i]];
        if(!(index % 5)) {
            index = 0;
            cout << endl;
        }
    }
}
