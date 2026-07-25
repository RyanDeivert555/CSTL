extern "C" {
#include "CSTL/queue.h"
#include "CSTL/allocator.h"
    QUEUE_DEFINE(i32)
    QUEUE_IMPL(i32)
}
#include "benchmark/benchmark.h"
#include <queue>

// This tests shows the preallocation abilities of cstl_queue

static void BM_std(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        // std::queue cannot pre-allocate
        std::queue<int> q{};

        for (i64 i = 0; i < n; ++i) {
            q.push(static_cast<i32>(i));
        }
        benchmark::ClobberMemory();

        state.PauseTiming();
        q = std::queue<int>();
        state.ResumeTiming();
    }

    state.SetComplexityN(n);
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_std)->RangeMultiplier(4)->Range(8, 8 << 12)->Complexity();

static void BM_cstl(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        cstl_queue(i32) v = queue_i32_create(std_allocator());
        queue_i32_reserve(&v, n);
        state.ResumeTiming();

        for (i64 i = 0; i < n; ++i) {
            queue_i32_push(&v, static_cast<i32>(i));
        }
        benchmark::ClobberMemory();

        state.PauseTiming();
        queue_i32_free(&v);
        state.ResumeTiming();
    }

    state.SetComplexityN(n);
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_cstl)->RangeMultiplier(4)->Range(8, 8 << 12)->Complexity();

BENCHMARK_MAIN();
