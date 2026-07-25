extern "C" {
#include "CSTL/set.h"
#include "CSTL/allocator.h"

    static bool i32_eql(i32 a, i32 b) {
        return a == b;
    }

    static u64 i32_hash(i32 n) {
        return (u64)n;
    }

    SET_DEFINE(i32)
    SET_IMPL(i32, i32_eql, i32_hash)
}
#include "benchmark/benchmark.h"
#include <set>

static void BM_std(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        cstl_set(i32) s = set_i32_create(std_allocator());

        for (i64 i = 0; i < n; ++i) {
            set_i32_insert(&s, static_cast<i32>(i));
        }

        benchmark::ClobberMemory();
        state.PauseTiming();
        set_i32_free(&s);
        state.ResumeTiming();
    }

    state.SetComplexityN(n);
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_std)->RangeMultiplier(4)->Range(8, 8 << 12)->Complexity();

static void BM_cstl(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        std::set<int> s = {0};

        for (int64_t i = 0; i < n; ++i) {
            s.insert(static_cast<i32>(i));
        }

        benchmark::ClobberMemory();
        state.PauseTiming();
        s = std::set<i32>();
        state.ResumeTiming();
    }

    state.SetComplexityN(n);
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_cstl)->RangeMultiplier(4)->Range(8, 8 << 12)->Complexity();

BENCHMARK_MAIN();
