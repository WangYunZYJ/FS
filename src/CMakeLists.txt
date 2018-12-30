file(GLOB_RECURSE c_files "*.cpp")
 
add_library(FS_lib_shared  SHARED ${c_files})
add_library(FS_lib_static STATIC ${c_files})

set_target_properties(FS_lib_shared PROPERTIES OUTPUT_NAME "wyfs")
set_target_properties(FS_lib_static PROPERTIES OUTPUT_NAME "wyfs")

set_target_properties(FS_lib_shared PROPERTIES VERSION 1.2 SOVERSION 1)

INSTALL(TARGETS FS_lib_shared FS_lib_static
    LIBRARY DESTINATION lib
    ARCHIVE DESTINATION lib)

INSTALL(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include)