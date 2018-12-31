#ifndef FS_IO_H
#define FS_IO_H

#include <memory>
#include <fstream>
using namespace std;

namespace wyfs
{
    class io
    {
        static shared_ptr<io> _instance;
        fstream fs;
        io();
    public:
        static shared_ptr<io>& get_instance();
        void write_Byte(char* msg, int offset);
        char* read_Byte(int offset, int len);
    };
}

#endif