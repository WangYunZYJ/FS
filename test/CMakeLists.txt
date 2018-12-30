file(GLOB_RECURSE c_files ./*.cpp)
  
add_executable(FS ${c_files} ../include/io/io.h)
     
target_link_libraries(FS wyfs)
