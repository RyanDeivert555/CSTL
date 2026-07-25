set(CSTL_BENCHMARKS
    vec
    hashmap
    queue
    set
)

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

foreach(BENCH_NAME IN LISTS CSTL_BENCHMARKS)
    add_executable(bench_${BENCH_NAME} ${BENCH_NAME}.cpp)
    if(NOT MSVC)
        target_compile_options(bench_${BENCH_NAME} PRIVATE -O3)
    else()
        target_compile_options(bench_${BENCH_NAME} PRIVATE /O2)
    endif()
    target_link_libraries(bench_${BENCH_NAME} PRIVATE cstl benchmark::benchmark)
endforeach()
