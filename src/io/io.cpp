#include <iostream>
#include "io/io.h"
#include "constant.h"
#include <assert.h>
#include <cstring>

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


void io::write_Byte(char* msg, int offset)
{
    fs.seekp(offset, ios::beg);
    fs.write(msg, 1);
}

char* io::read_Byte(int offset, int len)
{
    char *buf = new char[len+1];
    fs.seekg(offset, ios::beg);
    fs.read(buf, len);
    buf[len] = '\0';
    return buf;
}


