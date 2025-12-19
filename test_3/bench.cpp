#include <benchmark/benchmark.h>
#include <vector>
#include <cstddef>
#include "hybrid_sort.hpp"

static std::vector<double> make_reverse(std::size_t n)
{
    std::vector<double> v(n);
    for (std::size_t i = 0; i < n; ++i) v[i] = static_cast<double>(n - i);
    return v;
}

static void BM_ExistingHybridSort_Reverse100k(benchmark::State& state)
{
    constexpr std::size_t N = 100000;
    static const std::vector<double> base = make_reverse(N);
    for (auto _ : state)
    {
        std::vector<double> data = base;
        ::sort(data);
        benchmark::DoNotOptimize(data);
    }
    state.SetLabel("existing-threshold=16, N=100000");
}

BENCHMARK(BM_ExistingHybridSort_Reverse100k)->Iterations(10);
BENCHMARK_MAIN();
