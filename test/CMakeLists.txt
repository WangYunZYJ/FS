file(GLOB_RECURSE c_files ./*.cpp)
  
add_executable(FS ${c_files})
     
target_link_libraries(FS wyfs)
