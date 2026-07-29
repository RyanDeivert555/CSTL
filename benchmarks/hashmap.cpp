extern "C" {
#include "CSTL/hashmap.h"
#include "CSTL/allocator.h"

    static bool i32_eql(i32 a, i32 b) {
        return a == b;
    }

    static u64 i32_hash(i32 n) {
        return (u64)n;
    }

    HASHMAP_DEFINE(i32, i32)
    HASHMAP_IMPL(i32, i32, i32_eql, i32_hash)
}
#include "benchmark/benchmark.h"
#include <map>
#include <unordered_map>

static void BM_std_unordered_map(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        std::unordered_map<i32, i32> m{};

        for (i64 i = 0; i < n; i++) {
            m.insert({i, i});
        }

        benchmark::ClobberMemory();
        state.PauseTiming();
        m = std::unordered_map<i32, i32>{};
        state.ResumeTiming();
    }

    state.SetComplexityN(n);
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_std_unordered_map)->RangeMultiplier(4)->Range(8, 8 << 12)->Complexity();

static void BM_std_map(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        std::map<i32, i32> m{};

        for (i64 i = 0; i < n; i++) {
            m.insert({i, i});
        }

        benchmark::ClobberMemory();
        state.PauseTiming();
        m = std::map<i32, i32>{};
        state.ResumeTiming();
    }

    state.SetComplexityN(n);
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_std_map)->RangeMultiplier(4)->Range(8, 8 << 12)->Complexity();

static void BM_cstl(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        cstl_hashmap(i32, i32) m = hashmap_i32_i32_create(std_allocator());

        for (i64 i = 0; i < n; i++) {
            hashmap_i32_i32_set(&m, i, i);
        }

        benchmark::ClobberMemory();
        state.PauseTiming();
        hashmap_i32_i32_free(&m);
        state.ResumeTiming();
    }

    state.SetComplexityN(n);
    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_cstl)->RangeMultiplier(4)->Range(8, 8 << 12)->Complexity();

BENCHMARK_MAIN();
