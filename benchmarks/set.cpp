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
#include <unordered_set>

static void BM_std_unordered_set(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        std::unordered_set<i32> s{};

        for (i64 i = 0; i < n; i++) {
            s.insert(static_cast<i32>(i));
        }

        benchmark::DoNotOptimize(s);
        benchmark::ClobberMemory();
        state.PauseTiming();
        s = std::unordered_set<i32>();
        state.ResumeTiming();
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_std_unordered_set)->RangeMultiplier(4)->Range(8, 10 << 20);

static void BM_std_set(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        std::set<i32> s{};

        for (i64 i = 0; i < n; i++) {
            s.insert(static_cast<i32>(i));
        }

        benchmark::DoNotOptimize(s);
        benchmark::ClobberMemory();
        state.PauseTiming();
        s = std::set<i32>();
        state.ResumeTiming();
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_std_set)->RangeMultiplier(4)->Range(8, 10 << 20);

static void BM_cstl_set(benchmark::State& state) {
    const i64 n = state.range(0);
    for (auto _ : state) {
        cstl_set(i32) s = set_i32_create(std_allocator());

        for (i64 i = 0; i < n; i++) {
            set_i32_insert(&s, static_cast<i32>(i));
        }

        benchmark::DoNotOptimize(s);
        benchmark::ClobberMemory();
        state.PauseTiming();
        set_i32_free(&s);
        state.ResumeTiming();
    }

    state.SetItemsProcessed(state.iterations() * n);
}
BENCHMARK(BM_cstl_set)->RangeMultiplier(4)->Range(8, 10 << 20);
