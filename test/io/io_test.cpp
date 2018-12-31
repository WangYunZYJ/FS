#define CATCH_CONFIG_MAIN
#include "io.h"
#include "catch.h"
#include "constant.h"

using namespace std;
using namespace wyfs;

TEST_CASE("init file", "[init_file]")
{
    ofstream file(DISK_PATH);

    for(int i = 0; i < 1024; i++)
        file << 's';
}

TEST_CASE("io write", "[io_write]")
{
    char *ch = new char('a');
    auto _io = io::get_instance();
    for(int i = 0; i < 1024; i++)
       _io->write_Byte(ch, i);
    delete ch;
}

TEST_CASE("io_read", "[io_read]")
{
    auto _io = io::get_instance();
    auto str = _io->read_Byte(2, 10);
    cout << str << endl;
    cout << strlen(str) << endl;
}