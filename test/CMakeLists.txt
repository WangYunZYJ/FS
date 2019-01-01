file(GLOB_RECURSE c_files ./*.cpp)
  
add_executable(FS ${c_files} ../include/constant/constants.h ../include/disk_op/disk_init.h disk_op/disk_init_test.cpp ../include/inode/inode.h ../include/disk_op/volume.h)
     
target_link_libraries(FS wyfs)
