#include <benchmark/benchmark.h>
#include <vector>
#include <cstddef>
#include "hybrid_sort.hpp"


static std::vector<double> make_reverse_100k()
{
    constexpr std::size_t N = 100000;
    std::vector<double> v(N);
    for (std::size_t i = 0; i < N; ++i) {
        v[i] = static_cast<double>(N - i);
    }
    return v;
}

static void BM_HybridSort_Threshold(benchmark::State& state)
{
    std::size_t threshold = static_cast<std::size_t>(state.range(0));

    static const std::vector<double> base_data = make_reverse_100k();

    for (auto _ : state)
    {

        std::vector<double> data = base_data;

        sort_with_threshold(data, threshold);

        benchmark::DoNotOptimize(data.data());
    }
}

BENCHMARK(BM_HybridSort_Threshold)
    ->Name("HybridSort/ThresholdBenchmark")
    ->Unit(benchmark::kMillisecond)
    ->Args({4})
    ->Args({8})
    ->Args({16})
    ->Args({64})
    ->Args({128})
    ->Args({256})
    ->Args({512});


BENCHMARK_MAIN();