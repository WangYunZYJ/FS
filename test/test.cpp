#include "constants.h"
#include <include/disk_op/volume.h>
#include <include/cache/commands.h>
#include <iostream>
using namespace std;



int main()
{
    auto _volume = wyfs::volume::get_instance();

//aa    _volume->disk_init();

    _volume->init_cache();
    wyfs::login();

    wyfs::print_curr_path();
    string command;
    while (true) {
        cin >> command;
        if(command == "quit")
            break;
        else if(command == "ls")
            wyfs::ls();
        else if(command == "cd")
            wyfs::cd();
        else if(command == "mkdir")
            wyfs::mkdir();
        else if(command == "touch")
            wyfs::touch();
        else if(command == "useradd")
            wyfs::useradd();
        else if(command == "pwd")
            wyfs::pwd();
        else if(command == "passwd")
            wyfs::passwd();
        else if(command == "rm")
            wyfs::rm();
        else if(command == "cp")
            wyfs::cp();
        else if(command == "mv")
            wyfs::mv();
        else if(command == "echo")
            wyfs::echo();
        else if (command == "cat")
            wyfs::cat();
        else if(command == "fill")
            wyfs::fill();
        else if(command == "chmod")
            wyfs::chmod();
        else if(command == "chown")
            wyfs::chown();
        else if(command == "chgrp")
            wyfs::chgrp();
        else if(command == "unmask")
            wyfs::unmask();
        else if(command == "ln")
            wyfs::ln();
        else if(command == "superblock")
            wyfs::superblock();
        else if(command == "file")
            wyfs::file();
        else
            cout << command <<": 未找到命令\n";
        wyfs::print_curr_path();
    }
}