include(FetchContent)

set(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_GTEST_TESTS OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_INSTALL OFF CACHE BOOL "" FORCE)
set(BENCHMARK_ENABLE_WERROR OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    googlebenchmark
    GIT_REPOSITORY https://github.com/google/benchmark.git
    GIT_TAG v1.9.5
)
FetchContent_MakeAvailable(googlebenchmark)

add_executable(benchmarks 
    vec.cpp
    queue.cpp
    hashmap.cpp
    set.cpp
)
if(NOT MSVC)
    target_compile_options(benchmarks PRIVATE -O3)
else()
    target_compile_options(benchmarks PRIVATE /O2)
endif()
target_link_libraries(benchmarks PRIVATE cstl benchmark::benchmark_main)
