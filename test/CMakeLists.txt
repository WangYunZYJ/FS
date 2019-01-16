file(GLOB_RECURSE c_files ./*.cpp)
  
add_executable(FS ${c_files} ../include/constant/constants.h disk_op/disk_init_test.cpp ../include/inode/inode.h ../include/disk_op/volume.h ../include/disk_op/data_structs.h ../include/cache/commands.h ../include/cache/corlor_cout.h)
     
target_link_libraries(FS wyfs)
set(CMAKE_BUILD_TYPE Debug)