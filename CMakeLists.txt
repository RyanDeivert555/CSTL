cmake_minimum_required(VERSION 3.11)
project(main)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic -g)

add_executable(${PROJECT_NAME}
    src/main.c
    include/vec.h
    include/list.h
    include/hashmap.h
    include/array.h
)