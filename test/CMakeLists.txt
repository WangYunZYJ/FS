file(GLOB_RECURSE c_files ./*.cpp)
  
add_executable(FS ${c_files} ../include/io/io.h ../include/volume/volume.h ../include/volume/block.h ../include/constants/constant.h ../include/volume/free_stack.h volume/free_stack_test.cpp io/io_test.cpp)
     
target_link_libraries(FS wyfs)
