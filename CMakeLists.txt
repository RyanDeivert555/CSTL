cmake_minimum_required(VERSION 3.13)
project(main)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(
    SRC
    src/main.c
    src/allocator.c
    src/fba.c
    src/untyped_vec.c
)

set(
    INCLUDE
    include/common.h
    include/allocator.h
    include/fba.h
    include/vec.h
    include/list.h
    include/hashmap.h
    include/untyped_vec.h
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Debug")
endif()

add_executable(${PROJECT_NAME} ${INCLUDE} ${SRC})
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_options(${PROJECT_NAME} PRIVATE -g3 -fsanitize=address,undefined)
    target_link_libraries(${PROJECT_NAME} PRIVATE asan ubsan)
elseif(CMAKE_BUILD_TYPE STREQUAL "Release")
    target_compile_options(${PROJECT_NAME} PRIVATE -O3)
endif()

