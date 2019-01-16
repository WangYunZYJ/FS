//
// Created by wy on 19-1-7.
//

#include <include/cache/commands.h>
#include <include/disk_op/volume.h>
#include <include/cache/corlor_cout.h>
#include <iostream>
#include <iomanip>
#include <cstring>

std::vector<uint32> wyfs::curr_path;
std::map<std::string, uint32> wyfs::file_name2id;
std::map<uint32, std::string> wyfs::file_id2name;
std::map<std::string, std::string> wyfs::name2pwd;
char wyfs::owner[NAME_SIZE];
uint32 wyfs::group;
uint32 wyfs::curr_file_each_line = 0;

void wyfs::update_cache(std::string filename, uint32 file_inode_addr) {
    file_name2id[filename] = file_inode_addr;
    file_id2name[file_inode_addr] = filename;
}

std::vector<uint32> wyfs::split_path_string(std::string path) {
    string cpyPath = path;
    vector<uint32> pathVec;
    if(path[0] == '/'){
        pathVec.push_back(0);
        path = path.substr(1);
    }
    else {
        for(size_t i = 0; i < curr_path.size(); ++i) {
            pathVec.push_back(curr_path[i]);
        }
    }
    string::size_type p = path.find_first_of('/');
    while(p != string::npos){
        string dirName = path.substr(0, p);
        path = path.substr(p + 1);
        if(file_name2id[dirName]) {
            pathVec.push_back(file_name2id[dirName]);
        }else{
            if(dirName == "..") {
                if(pathVec.size() == 1) {
                    p = path.find_first_of('/');
                    continue;
                }
                auto it = pathVec.end();
                --it;
                pathVec.erase(it);
            }else {
//                std::cout << "cd: " << cpyPath << ": 没有那个文件或目录\n";
                pathVec.clear();
                return pathVec;
            }
        }
        p = path.find_first_of('/');
    }
    if(file_name2id[path]) {
        pathVec.push_back(file_name2id[path]);
    }else{
        if(path == "..") {
            if(pathVec.size() == 1) {
                p = path.find_first_of('/');
                return pathVec;
            }
            auto it = pathVec.end();
            --it;
            pathVec.erase(it);
        }else if(path == ".") {
            return pathVec;
        }
        else {
//            std::cout << "cd: " << cpyPath << ": 没有那个文件或目录\n";
            pathVec.clear();
            return pathVec;
        }
    }

    return pathVec;
}

std::string wyfs::union_uint2path(const std::vector<uint32> &pathVec) {
    string path = "/";
    for(size_t i = 1; i < pathVec.size() - 1; ++i) {
        path += file_id2name[pathVec[i]];
        path += "/";
    }
    if(pathVec.size() > 1)
        path += file_id2name[pathVec[pathVec.size() - 1]];
    path += "$ ";
    return path;
}

void wyfs::print_curr_path() {
    string path = union_uint2path(curr_path);
    cyellow << path;
}

void wyfs::touch() {

    if(curr_path.size() <= 1) {
        cout << "Permission Denied!\n";
        return;
    }

    auto _volume = volume::get_instance();
    char file_name[NAME_SIZE];
    cin >> file_name;
    uint32 file_inode_addr = _volume->create_file(file_name, owner, group);
    _volume->update_tree_lists(curr_path[curr_path.size() - 1], file_inode_addr);
    update_cache(file_name, file_inode_addr);
}

void wyfs::useradd() {

    auto _volume = volume::get_instance();
    char username[NAME_SIZE];
    string password;
    cin >> username;
    cout << "请输入用户" << username << "的口令： ";
    cin >> password;

    _volume->add_user_pwd(username, password);

    uint32 file_inode_addr = _volume->create_file(username, username, group, FileMode::MENU_FILE);
    _volume->update_tree_lists(0, file_inode_addr);

    update_cache(username, file_inode_addr);
    name2pwd[username] = password;
}

void wyfs::mkdir() {
    if(curr_path.size() <= 1) {
        cout << "Permission Denied!\n";
        return;
    }
    auto _volume = volume::get_instance();
    char file_name[NAME_SIZE];
    cin >> file_name;
    uint32 file_inode_addr = _volume->create_file(file_name, owner, group, FileMode::MENU_FILE);
    _volume->update_tree_lists(curr_path[curr_path.size() - 1], file_inode_addr);

    update_cache(file_name, file_inode_addr);
}

void wyfs::cd() {
    auto _volume = volume::get_instance();
    string path;
    cin >> path;
    if(path == "."){
        return;
    }else if(path == ".."){
        if(curr_path.size() == 1) return;
        auto it = curr_path.end();
        --it;
        curr_path.erase(it);
    }else{
        auto pathVec = split_path_string(path);
        if(!pathVec.size()) {
            std::cout << "cd: " << path << ": 不是目录或该目录不存在\n";
            return;
        }
        else {
            auto _volume = volume::get_instance();
            if(_volume->is_normal_file(pathVec[pathVec.size() - 1])){
                std::cout << "cd: " << path << ": 不是目录或该目录不存在\n";
                return;
            }

            if((curr_path.size() == 1 && pathVec.size()) || curr_path[1] != pathVec[1])  {
                cout << "请输入用户" << file_id2name[pathVec[0]] << "的口令： ";
                string password;
                cin >> password;
                if(password != name2pwd[file_id2name[pathVec[1]]]) {
                    cout << "用户口令错误\n";
                    return;
                }
                strcpy(owner, file_id2name[pathVec[1]].c_str());
            }
            curr_path = pathVec;
        }
    }
}

void wyfs::ls() {

    auto print_files = [&](){
        auto _volume = volume::get_instance();
        vector<uint32> sons = _volume->get_sons_inode_addr(curr_path);
        for (size_t i = 0; i < sons.size(); ++i) {
            curr_file_each_line++;
            cgreen << std::left << setw(WIDTH) << file_id2name[sons[i]];
            if (!(curr_file_each_line % 5)) {
                curr_file_each_line = 0;
                cgreen << endl;
            }
        }
        cout << endl;
        curr_file_each_line = 0;
    };

    if(getchar() == '\n') {
        print_files();
    }else {
        string sub_command;
        cin >> sub_command;
        if(sub_command == "-a") {
            cgreen << std::left << setw(WIDTH) << ".";
            cgreen << std::left << setw(WIDTH) << "..";
            curr_file_each_line = 2;
            print_files();
        }
        else if(sub_command == "-l") {
            auto _volume = volume::get_instance();
            _volume->print_inode_msg(curr_path[curr_path.size() - 1]);
        }
        else {
            cwhite << "ls: 无效选项 -- " << sub_command.substr(1) << endl;
            return;
        }
    }
}

void wyfs::pwd() {
    string path = union_uint2path(curr_path);
    path = path.substr(0, path.length() - 2);
    cwhite << path;
    cwhite << endl;
}

void wyfs::passwd() {
    string username;
    cin >> username;
    if(name2pwd[username] == "") {
        cred << "用户" << username << "不存在\n";
    }
    cblue << "请输入用户" << username << "的口令: ";
    string password;
    cin >> password;
    if(password == name2pwd[username]){
        cblue << "请输入用户" << username << "的新口令: ";
        cin >> password;
        name2pwd[username] = password;
        auto _volume = volume::get_instance();
        _volume->update_user_pwd(username, password);
    }else{
        cred << "用户" << username << "的口令错误，请重新尝试\n";
        return;
    }
}

void wyfs::rm() {
    string file_name;
    cin >> file_name;
    if(curr_path.size() == 1) {
        cwhite << "rm: 无法删除\'" << file_name << "\':没有那个文件或目录\n";
        return;
    }
    auto _volume = volume::get_instance();
    auto sons_inode_addr = _volume->get_sons_inode_addr(curr_path);
    for(size_t i = 0; i < sons_inode_addr.size(); ++i) {
        if(file_id2name[sons_inode_addr[i]] == file_name) {
            _volume->release_dir_or_file(sons_inode_addr[i], curr_path[curr_path.size() - 1]);
            file_name2id[file_id2name[sons_inode_addr[i]]] = 0;
            file_id2name[sons_inode_addr[i]] = "";
            return;
        }
    }
    cwhite << "rm: 无法删除\'" << file_name << "\':没有那个文件或目录\n";
}

void wyfs::mv() {

}

void wyfs::cp() {
    string src, dest;
    cin >> src >> dest;
    auto srcVec = split_path_string(src);
    auto destVec = split_path_string(dest);
    if(!srcVec.size() || !destVec.size()) {
        cwhite << "cp: 路径\'" << src << "\'或\'" << dest << "\'错误\n";
        return;
    }

    auto _volume = volume::get_instance();

    uint32 file_inode_addr = _volume->create_file(const_cast<char*>((file_id2name[srcVec[srcVec.size() - 1]]+"_cpy").c_str()), owner, group);
    _volume->update_tree_lists(destVec[destVec.size() - 1], file_inode_addr);
    _volume->echo_msg_to_file(file_inode_addr, _volume->get_msg(srcVec[srcVec.size() - 1]));
    update_cache(const_cast<char*>((file_id2name[srcVec[srcVec.size() - 1]]+"_cpy").c_str()), file_inode_addr);

}

void wyfs::echo() {
    string msg, op, des;
    cin >> msg;

    if(getchar() == ' ') {
        cin >> op >> des;
        auto pathVec = split_path_string(des);
        if(op == ">" && pathVec.size()) {
            auto _volume = volume::get_instance();
            _volume->echo_msg_to_file(pathVec[pathVec.size() - 1], msg);
        }else {
            cwhite << "echo: 没有参数\'" << op << "\'\n";
            return;
        }
    }else {
        cwhite << msg << endl;
        return;
    }
}

void wyfs::cat() {
    string path;
    cin >> path;
    auto _volume = volume::get_instance();
    auto pathVec = split_path_string(path);
    if(!pathVec.size()) {
        cwhite << "cat: " << path << ": 没有那个文件或目录\n";
        return;
    }
    cwhite << _volume->get_msg(pathVec[pathVec.size() - 1]) << endl;
}
