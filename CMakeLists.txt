cmake_minimum_required(VERSION 3.13)
project(main)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic -g)

add_executable(${PROJECT_NAME}
    include/allocator.h
    include/vec.h
    include/list.h
    src/main.c
    src/allocator.c
)
