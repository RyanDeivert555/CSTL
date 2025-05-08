cmake_minimum_required(VERSION 3.13)
project(main)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(${PROJECT_NAME}
    include/allocator.h
    include/fba.h
    include/vec.h
    include/list.h
    include/hashmap.h
    src/main.c
    src/allocator.c
    src/fba.c
)

# TODO: keep this?
add_custom_target(
    check
    DEPENDS ${PROJECT_NAME}
    COMMAND valgrind --leak-check=full --track-origins=yes -s ./${PROJECT_NAME}
)

target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -g3 -fsanitize=address,undefined)
target_link_libraries(${PROJECT_NAME} PRIVATE -lasan -lubsan)

