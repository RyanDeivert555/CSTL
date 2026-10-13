extern "C" {
#include "CSTL/vec.h"
#include "CSTL/allocator.h"
    VEC_DEFINE(i32)
    VEC_IMPL(i32)
}
#include "benchmark/benchmark.h"
#include <vector>

// This doesn't test anything meanful
// I do not want to testing the underlying allocators, and testing pushing is trivial

static void BM_std_vector(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        std::vector<i32> v{};
        v.reserve(n);
        state.ResumeTiming();

        for (i64 i = 0; i < n; i++) {
            v.push_back(static_cast<i32>(i));
        }

        benchmark::DoNotOptimize(v);
        benchmark::ClobberMemory();
        state.PauseTiming();
        v = std::vector<i32>();
        state.ResumeTiming();
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_std_vector)->RangeMultiplier(4)->Range(8, 10 << 20);

static void BM_cstl_vec(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        state.PauseTiming();
        cstl_vec(i32) v = vec_i32_create(std_allocator());
        vec_i32_reserve(&v, n);
        state.ResumeTiming();

        for (i64 i = 0; i < n; i++) {
            vec_i32_push(&v, static_cast<i32>(i));
        }

        benchmark::DoNotOptimize(v);
        benchmark::ClobberMemory();
        state.PauseTiming();
        vec_i32_free(&v);
        state.ResumeTiming();
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_cstl_vec)->RangeMultiplier(4)->Range(8, 10 << 20);
