//
//#include "constants.h"
//#include <include/disk_op/volume.h>
//#include <include/cache/commands.h>
//#include <iostream>
//using namespace std;
//
//int main()
//{
//    auto _volume = wyfs::volume::get_instance();
//    _volume->init_cache();
//    wyfs::print_curr_path();
//    string command;
//    while (true) {
//        cin >> command;
//        if(command == "quit")
//            break;
//        else if(command == "ls")
//            wyfs::ls();
//        else if(command == "cd")
//            wyfs::cd();
//        else if(command == "mkdir")
//            wyfs::mkdir();
//        else if(command == "touch")
//            wyfs::touch();
//        else if(command == "useradd")
//            wyfs::useradd();
//        wyfs::print_curr_path();
//    }
//}