cmake_minimum_required(VERSION 2.6)
project (FS)

set(CMAKE_CXX_STANDARD 11)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/lib)
 
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/build/bin)

function(include_sub_directories_recursively root_dir)
    if (IS_DIRECTORY ${root_dir})            
        message("include dir: " ${root_dir})
        include_directories(${root_dir})
    endif()

    file(GLOB ALL_SUB RELATIVE ${root_dir} ${root_dir}/*) 
    foreach(sub ${ALL_SUB})
        if (IS_DIRECTORY ${root_dir}/${sub})
            include_sub_directories_recursively(${root_dir}/${sub}) 
        endif()
    endforeach()
endfunction()

include_sub_directories_recursively(${PROJECT_SOURCE_DIR})

link_directories(
    ${PROJECT_SOURCE_DIR}/build/lib
    )

add_subdirectory(src)
add_subdirectory(test)

set(CMAKE_INSTALL_PREFIX ${PROJECT_SOURCE_DIR}/install)