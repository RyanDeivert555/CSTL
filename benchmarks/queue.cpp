extern "C" {
#include "CSTL/queue.h"
#include "CSTL/allocator.h"
    QUEUE_DEFINE(i32)
    QUEUE_IMPL(i32)
}
#include "benchmark/benchmark.h"
#include <queue>

static void BM_std_queue(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        std::queue<int> q{};

        for (i64 i = 0; i < n; i++) {
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
BENCHMARK(BM_std_queue)->RangeMultiplier(4)->Range(8, 8 << 12)->Complexity();

static void BM_cstl_queue_no_prealloc(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        cstl_queue(i32) v = queue_i32_create(std_allocator());

        for (i64 i = 0; i < n; i++) {
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
BENCHMARK(BM_cstl_queue_no_prealloc)->RangeMultiplier(4)->Range(8, 8 << 12)->Complexity();

static void BM_cstl_queue_prealloc(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        cstl_queue(i32) v = queue_i32_create(std_allocator());
        queue_i32_reserve(&v, n);

        for (i64 i = 0; i < n; i++) {
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
BENCHMARK(BM_cstl_queue_prealloc)->RangeMultiplier(4)->Range(8, 8 << 12)->Complexity();
