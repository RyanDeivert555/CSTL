cmake_minimum_required(VERSION 3.13)
project(
    cstl 
    DESCRIPTION "A library with generic implementations of common data stuctures"
    LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

add_library(${PROJECT_NAME})

set_target_properties(${PROJECT_NAME} PROPERTIES
    C_STANDARD 11
    C_STANDARD_REQUIRED YES
    C_EXTENSIONS NO
)

target_sources(${PROJECT_NAME}
    PRIVATE
    src/allocator.c
    src/fba.c
    src/random.c
    src/xorshift.c
)

target_include_directories(${PROJECT_NAME} PUBLIC include/)
target_compile_options(${PROJECT_NAME} PRIVATE 
    -Wall -Wextra -Wpedantic -Werror
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)
target_link_options(${PROJECT_NAME} PRIVATE
    $<$<CONFIG:Debug>:-fsanitize=address,undefined>
)

# tests
option(CSTL_BUILD_TESTS "Build tests" OFF)
if(CSTL_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

# benchmarking
option(CSTL_BUILD_BENCHMARKS "Build performance benchmarks" OFF)
if(CSTL_BUILD_BENCHMARKS)
    enable_language(CXX)
    add_subdirectory(benchmarks)
endif()
