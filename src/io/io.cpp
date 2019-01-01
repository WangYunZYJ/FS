//
// Created by wy on 19-1-1.
//

#include <iostream>
#include "io/io.h"
#include <assert.h>
#include <cstring>
#include <include/io/io.h>
#include <include/constant/constants.h>


using namespace wyfs;

shared_ptr<io> io::_instance = nullptr;

io::io()
{
    fs.open(DISK_PATH, ios::binary | ios::out | ios::in);
}


shared_ptr<io>& io::get_instance()
{
    if(_instance.get() == nullptr)
        _instance = std::shared_ptr<io>(new io());
    return _instance;
}

fstream &io::write(const char *s, streamsize count) {
    fs.write(s, count);
    return fs;
}

fstream &io::seekp(int offset, ios_base::seekdir dir) {
    fs.seekp(offset, dir);
    return fs;
}

fstream &io::seekg(int offset, ios_base::seekdir dir) {
    fs.seekg(offset, dir);
    return fs;
}

fstream &io::read(char *s, streamsize count) {
    fs.read(s, count);
    return fs;
}

