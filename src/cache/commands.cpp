//
// Created by wy on 19-1-7.
//

#include <include/cache/commands.h>
#include <include/disk_op/volume.h>
#include <include/cache/corlor_cout.h>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>

std::vector<uint32> wyfs::curr_path;
std::map<std::string, uint32> wyfs::file_name2id;
std::map<uint32, std::string> wyfs::file_id2name;
std::map<std::string, std::string> wyfs::name2pwd;
std::map<uint32, std::string> wyfs::file_id2name_full;
wyfs::FilePermision wyfs::filePermision = {7, 7, 5};
char wyfs::owner[NAME_SIZE];
char wyfs::group[NAME_SIZE];
uint32 wyfs::curr_file_each_line = 0;


void wyfs::update_cache(std::string filename, uint32 file_inode_addr) {
    file_name2id[filename] = file_inode_addr;
    file_id2name[file_inode_addr] = get_single_name(filename);
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
        if(file_name2id[wyfs::get_full_name(dirName, pathVec)]) {
            pathVec.push_back(file_name2id[wyfs::get_full_name(dirName, pathVec)]);
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
    if(file_name2id[wyfs::get_full_name(path, pathVec)]) {
        pathVec.push_back(file_name2id[wyfs::get_full_name(path, pathVec)]);
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


std::string wyfs::get_father_path(std::string path) {
    auto p = path.find_last_of('/');
    if(p == string::npos)
        return get_full_name(curr_path);
    else {
        path = path.substr(0, p);
        return get_full_name(split_path_string(path));
    }
}

std::string wyfs::get_full_name(const std::vector<uint32>& pathVec)
{
    string path = "/";
    for(size_t i = 1; i < pathVec.size() - 1; ++i) {
        path += file_id2name[pathVec[i]];
        path += "/";
    }
    if(pathVec.size() > 1)
        path += file_id2name[pathVec[pathVec.size() - 1]];
    return path;
}

std::string wyfs::get_full_name(const std::string path, const std::vector<uint32> &pathVec) {
    if(pathVec.size() == 1)
        return wyfs::get_full_name(pathVec) + path;
    else
        return wyfs::get_full_name(pathVec) + '/' + path;
}

std::string wyfs::union_uint2path(const std::vector<uint32> &pathVec) {
    string path = wyfs::get_full_name(pathVec);
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

    string tmpPath = wyfs::get_full_name(curr_path);
    tmpPath += "/"; tmpPath += file_name;
    char trueUsername[NAME_SIZE];
    strcpy(trueUsername, tmpPath.c_str());

    uint32 file_inode_addr = _volume->create_file(trueUsername, owner, group);
    _volume->update_tree_lists(curr_path[curr_path.size() - 1], file_inode_addr);
    update_cache(trueUsername, file_inode_addr);
}

void wyfs::useradd() {

    auto _volume = volume::get_instance();
    char username[NAME_SIZE];
    string password;
    cin >> username;
    cout << "请输入用户" << username << "的口令： ";
    cin >> password;

    _volume->add_user_pwd(username, password);

    string tmpStr = "/"; tmpStr += username;
    char trueUsername[NAME_SIZE];
    strcpy(trueUsername, tmpStr.c_str());

    uint32 file_inode_addr = _volume->create_file(trueUsername, username, group, FileMode::MENU_FILE);
    _volume->update_tree_lists(0, file_inode_addr);

    update_cache(trueUsername, file_inode_addr);
    name2pwd[username] = password;


    if(owner[0]!='\0') {
        strcpy(owner, username);
        strcpy(group, username);
    }
}

void wyfs::mkdir() {
    if(curr_path.size() <= 1) {
        cout << "Permission Denied!\n";
        return;
    }
    auto _volume = volume::get_instance();
    char file_name[NAME_SIZE];
    cin >> file_name;

    string tmpPath = wyfs::get_full_name(curr_path);
    tmpPath += "/"; tmpPath += file_name;
    char trueUsername[NAME_SIZE];
    strcpy(trueUsername, tmpPath.c_str());

    uint32 file_inode_addr = _volume->create_file(trueUsername, owner, group, FileMode::MENU_FILE);
    _volume->update_tree_lists(curr_path[curr_path.size() - 1], file_inode_addr);

    update_cache(trueUsername, file_inode_addr);
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

            //TODO
            //不再使用口令控制文件夹，通过rwx权限控制显示。
//            if((curr_path.size() == 1 && pathVec.size()) || curr_path[1] != pathVec[1])  {
//                cout << "请输入用户" << file_id2name[pathVec[0]] << "的口令： ";
//                string password;
//                cin >> password;
//                if(password != name2pwd[file_id2name[pathVec[1]]]) {
//                    cout << "用户口令错误\n";
//                    return;
//                }
//                strcpy(owner, file_id2name[pathVec[1]].c_str());
//            }
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
            if(!_volume->is_normal_file(sons[i]))
                cblue << std::left << setw(WIDTH) << file_id2name[sons[i]];
            else
                cpurple << std::left << setw(WIDTH) << file_id2name[sons[i]];
            if (!(curr_file_each_line % 5)) {
                curr_file_each_line = 0;
                if(i != sons.size() - 1)
                    cblue << endl;
            }
        }
        if(sons.size())
            cout << endl;
        curr_file_each_line = 0;
    };

    if(getchar() == '\n') {
        print_files();
    }else {
        string sub_command;
        cin >> sub_command;
        if(sub_command == "-a") {
            cblue << std::left << setw(WIDTH) << ".";
            cblue << std::left << setw(WIDTH) << "..";
            curr_file_each_line = 2;
            print_files();
        }
        else if(sub_command == "-l") {
            auto _volume = volume::get_instance();
            _volume->print_inode_msg(curr_path);
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
    }else {
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
            file_name2id[get_full_name(file_id2name[sons_inode_addr[i]],curr_path)] = 0;
            file_id2name[sons_inode_addr[i]] = "";
            return;
        }
    }
    cwhite << "rm: 无法删除\'" << file_name << "\':没有那个文件或目录\n";
}

void wyfs::mv() {
    string srcPath, desPath;
    cin >> srcPath >> desPath;
    auto srcPathVec = split_path_string(srcPath);
    auto desFatherPath = get_father_path(desPath);
    auto fatherPathVec = split_path_string(desFatherPath);
    if(!srcPathVec.size()) {
        cwhite << "mv: failed to access \'" << srcPath << "\'没有那个目录或文件\n";
        return;
    }
    if(!fatherPathVec.size()) {
        cwhite << "mv: failed to access \'" << desPath << "\'没有那个目录或文件\n";
        return;
    }
    if(fatherPathVec.size() == 1) {
        cwhite << "Permission Denied!\n";
        return;
    }

    auto _volume = volume::get_instance();
    char file_name[NAME_SIZE];
    strcpy(file_name, string(desFatherPath +'/' + get_single_name(desPath)).c_str());
    if(fatherPathVec == curr_path) {
        _volume->rename(srcPathVec.back(), file_name);
    }else {
        _volume->update_tree_lists(fatherPathVec.back(), srcPathVec.back());
        update_cache(file_name, srcPathVec.back());
        _volume->update_treelist_for_mv(srcPathVec.back(), srcPathVec[srcPathVec.size() - 2]);
        file_name2id.erase(get_full_name(srcPathVec));
    }
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
    update_cache(get_full_name(const_cast<char*>((file_id2name[srcVec[srcVec.size() - 1]]+"_cpy").c_str()), destVec), file_inode_addr);

}

void wyfs::echo() {
    string msg, op, des;
    cin >> msg;

    if(getchar() == ' ') {
        cin >> op >> des;
        auto pathVec = split_path_string(des);
        if(pathVec.size() == 0) {
            cwhite << "echo: 没有那个目录或文件\n";
            return;
        }
        if(op == ">" && pathVec.size()) {
            //releas
            auto _volume = volume::get_instance();
            _volume->release_blocks(pathVec.back(), curr_path.back());
            _volume->echo_msg_to_file(pathVec.back(), msg);
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
    cwhite << _volume->get_msg(pathVec.back()) << endl;
}

void wyfs::login() {

    if(!name2pwd.size()) {
        cwhite << "创建一个用户\n请输入用户名: ";
        firstUseradd();
        return;
    }
    while(true) {
        string username, password;
        auto _volume = volume::get_instance();
        cwhite << "请输入用户名: ";
        cin >> username;
        cwhite << "请输入用户密码: ";
        cin >> password;
        if (name2pwd[username] != password) {
            cblue << "用户名或密码错误，请重新输入\n";
            continue;
        }
        else {
            strcpy(owner, username.c_str());
            strcpy(group, username.c_str());
            break;
        }
    }
}

void wyfs::fill() {

    string des;
    cin >> des;
    auto pathVec = split_path_string(des);
    if(pathVec.size() == 0) {
        cwhite << "fill: 没有那个目录或文件\n";
        return;
    }
    string msg, tmpAns;
    ifstream file(FILE_MSG_PATH);
    while (getline(file, tmpAns))
        msg += tmpAns + '\n';
    file.close();
    auto _volume = volume::get_instance();
    _volume->echo_msg_to_file(pathVec.back(), msg);
}

std::string wyfs::get_single_name(std::string path) {
    auto p = path.find_last_of('/');
    if(p == string::npos)
        return path;
    else return path.substr(p+1);
}

void wyfs::chmod() {
    string permission, path;
    auto _volume = volume::get_instance();
    cin >> permission >> path;
    auto pathVec = split_path_string(path);
    if(!pathVec.size()) {
        cwhite << "chmod: 文件或目录不存在\n";
        return;
    }
    if(pathVec.size() <= 2) {
        cwhite << "Permission Denied!\n";
        return;
    }
    _volume->chmod(pathVec.back(), permission);
}

void wyfs::chown() {
    string onr, path;
    auto _volume = volume::get_instance();
    cin >> onr >> path;
    auto pathVec = split_path_string(path);
    if(!pathVec.size()) {
        cwhite << "chmod: 文件或目录不存在\n";
        return;
    }
    if(pathVec.size() <= 2) {
        cwhite << "Permission Denied!\n";
        return;
    }
    _volume->chown(pathVec.back(), onr);
}

void wyfs::chgrp() {
    string grp, path;
    cin >> grp >> path;
    auto pathVec = split_path_string(path);
    if(!pathVec.size()) {
        cwhite << "chmod: 文件或目录不存在\n";
        return;
    }
    if(pathVec.size() <= 2) {
        cwhite << "Permission Denied!\n";
        return;
    }
    auto _volume = volume::get_instance();
    _volume->chgrp(pathVec.back(), grp);
}

void wyfs::ln() {
    string srcPath, desPath;
    cin >> srcPath >> desPath;
    auto srcPathVec = split_path_string(srcPath);
    auto desFatherPath = get_father_path(desPath);
    auto fatherPathVec = split_path_string(desFatherPath);
    if(!srcPathVec.size()) {
        cwhite << "ln: failed to access \'" << srcPath << "\'没有那个目录或文件\n";
        return;
    }
    if(!fatherPathVec.size()) {
        cwhite << "ln: failed to access \'" << desPath << "\'没有那个目录或文件\n";
        return;
    }
    if(fatherPathVec.size() == 1) {
        cwhite << "Permission Denied!\n";
        return;
    }
    auto _volume = volume::get_instance();

    char file_name[NAME_SIZE];
    strcpy(file_name, string(desFatherPath +'/' + get_single_name(desPath)).c_str());

    uint32 file_inode_addr = _volume->create_file(file_name, owner, group, FileMode::LINK_FILE);
    _volume->link(file_inode_addr, srcPathVec.back());
    _volume->update_tree_lists(fatherPathVec.back(), file_inode_addr);
    update_cache(file_name, file_inode_addr);
}

void wyfs::unmask() {
    string permission;
    cin >> permission;
    if(permission.length() > 3) {
        cwhite << "unmask: 默认权限错误\n";
        return;
    }
    filePermision.owner = permission[0] - '0';
    filePermision.group_menber = permission[1] - '0';
    filePermision.others = permission[2] - '0';
    auto _volume = volume::get_instance();
    _volume->save_mask(filePermision);
}

void wyfs::firstUseradd() {
        auto _volume = volume::get_instance();
        char username[NAME_SIZE];
        string password;
        cin >> username;
        cout << "请输入用户" << username << "的口令： ";
        cin >> password;

        _volume->add_user_pwd(username, password);

        string tmpStr = "/"; tmpStr += username;
        char trueUsername[NAME_SIZE];
        strcpy(trueUsername, tmpStr.c_str());

        uint32 file_inode_addr = _volume->create_file(trueUsername, username, group, FileMode::MENU_FILE);
        _volume->update_tree_lists(0, file_inode_addr);

        update_cache(trueUsername, file_inode_addr);
        name2pwd[username] = password;


        strcpy(owner, username);
        strcpy(group, username);
}

void wyfs::superblock() {
    auto _volume = volume::get_instance();
    _volume->superblock_print_test();
    _volume->print_inode();
}

void wyfs::create_file(uint32 blk_count) {
    ofstream file(FILE_MSG_PATH);
    for(size_t i = 0; i < BLOCK_SIZE * blk_count; ++i) {
        file << 'u';
    }
}

void wyfs::file() {
    uint32 count;
    cin >> count;
    create_file(count);
}
