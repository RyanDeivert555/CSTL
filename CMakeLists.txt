cmake_minimum_required(VERSION 3.13)
project(
    cstl 
    VERSION 0.0.1
    DESCRIPTION "A library with generic implementations of common data stuctures"
    LANGUAGES C
)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(RUN_TESTS "Run tests" ON)
option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(
    SRC
    src/allocator.c
    src/fba.c
    src/untyped_vec.c
    src/untyped_hashmap.c
    src/intrusive_list.c
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
    include/untyped_hashmap.h
    include/intrusive_list.h
)

if(BUILD_SHARED_LIBS)
    add_library(${PROJECT_NAME} SHARED ${INCLUDE} ${SRC})
else()
    add_library(${PROJECT_NAME} STATIC ${INCLUDE} ${SRC})
endif()
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror)

target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:-g3 -fsanitize=address,undefined>)
target_link_libraries(${PROJECT_NAME} PRIVATE $<$<CONFIG:Debug>:asan ubsan>)
target_compile_options(${PROJECT_NAME} PRIVATE $<$<CONFIG:Release>:-O3>)

if(RUN_TESTS)
    add_executable(cstl_tests src/main.c)
    target_compile_options(cstl_tests PRIVATE $<$<CONFIG:Debug>:-g3 -fsanitize=address,undefined>)
    target_link_libraries(cstl_tests PRIVATE $<$<CONFIG:Debug>:asan ubsan>)
    target_compile_options(cstl_tests PRIVATE $<$<CONFIG:Release>:-O3>)
    target_link_libraries(cstl_tests PRIVATE ${PROJECT_NAME})
    enable_testing()
    add_test(cstl_tests cstl_tests)
endif()

