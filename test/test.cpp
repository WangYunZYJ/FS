#define CATCH_CONFIG_MAIN
#include "catch.h"
#include "constants.h"
#include <include/disk_op/volume.h>
#include <iostream>
using namespace std;

TEST_CASE("touch", "[touch]")
{
    auto _volume = wyfs::volume::get_instance();
    _volume->init_cache();
}