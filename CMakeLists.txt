cmake_minimum_required(VERSION 3.13)
project(
    cstl 
    VERSION 0.0.1
    DESCRIPTION "A library with generic implementations of common data stuctures"
    LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(CSTL_RUN_TESTS "Run tests" ON)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_library(${PROJECT_NAME})

target_sources(${PROJECT_NAME}
    PRIVATE
    src/allocator.c
    src/fba.c
    src/untyped_vec.c
    src/untyped_hashmap.c
    src/intrusive_list.c
)

target_include_directories(${PROJECT_NAME} PUBLIC include/)
target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall -Wextra -Wpedantic -Werror -std=c23
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)
target_link_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)

if(CSTL_RUN_TESTS)
    add_executable(cstl_tests src/main.c)
    target_compile_options(cstl_tests PRIVATE 
        $<$<CONFIG:Debug>:-fsanitize=address,undefined> 
    )
    target_link_options(cstl_tests PRIVATE
        $<$<CONFIG:Debug>:-fsanitize=address,undefined>
    )
    target_link_libraries(cstl_tests PRIVATE 
        ${PROJECT_NAME} 
    )
    enable_testing()
    add_test(cstl_tests cstl_tests)
endif()

