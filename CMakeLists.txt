cmake_minimum_required(VERSION 3.13)
project(main)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options(-Wall -Wextra -Wpedantic -g3 -fsanitize=address -fno-omit-frame-pointer)

add_executable(${PROJECT_NAME}
    include/allocator.h
    include/fba.h
    include/arena.h
    include/vec.h
    include/list.h
    include/hashmap.h
    src/main.c
    src/allocator.c
    src/fba.c
    src/arena.c
)

target_link_libraries(main asan)

