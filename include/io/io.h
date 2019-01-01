//
// Created by wy on 19-1-1.
//

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

        fstream& seekg(int offset, ios_base::seekdir dir = ios::beg);
        fstream& read(char *s, streamsize count);

        fstream& seekp(int offset, ios_base::seekdir dir = ios::beg);
        fstream& write(const char *s, streamsize count);
    };
}

#endif //FS_IO_H
