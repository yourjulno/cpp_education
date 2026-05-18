#include "../2nd_semester/14.04.hpp"

#include <benchmark/benchmark.h>

#include <cmath>
#include <cstddef>
#include <ranges>
#include <string>
#include <thread>
#include <vector>

namespace
{
    auto heavy_function(double value) -> double
    {
        for (int i = 0; i < 8; ++i)
        {
            value =
                std::sin(value) * std::cos(value) +
                std::sqrt(value + 1.0) +
                std::log1p(value);
        }

        return value;
    }

    auto make_data() -> std::vector<double>
    {
        constexpr std::size_t size = 1u << 22;

        std::vector<double> data(size);

        for (std::size_t i = 0; i < data.size(); ++i)
        {
            data[i] = 0.001 * static_cast<double>(i + 1);
        }

        return data;
    }

    auto input_data() -> const std::vector<double>&
    {
        static const std::vector<double> data = make_data();
        return data;
    }

    auto reference_result() -> double
    {
        static const double value =
            parallel::fold(std::views::all(input_data()), 0.0, 1, heavy_function);

        return value;
    }

    auto nearly_equal(double left, double right) -> bool
    {
        const double scale = std::max(1.0, std::max(std::abs(left), std::abs(right)));
        return std::abs(left - right) <= 1e-7 * scale;
    }

    auto max_threads() -> int
    {
        const unsigned hardware = std::thread::hardware_concurrency();
        return static_cast<int>(hardware == 0 ? 8 : hardware * 2);
    }

    void BM_parallel_fold(benchmark::State& state)
    {
        const auto& data = input_data();
        const unsigned thread_count = static_cast<unsigned>(state.range(0));
        const double expected = reference_result();

        for (auto _ : state)
        {
            const double result =
                parallel::fold(std::views::all(data), 0.0, thread_count, heavy_function);

            if (!nearly_equal(result, expected))
            {
                state.SkipWithError("invalid result");
                break;
            }

            benchmark::DoNotOptimize(result);
            benchmark::ClobberMemory();
        }

        state.SetItemsProcessed(
            static_cast<int64_t>(state.iterations()) * static_cast<int64_t>(data.size()));
        state.SetLabel("threads=" + std::to_string(thread_count));
    }
}

BENCHMARK(BM_parallel_fold)
    ->DenseRange(1, max_threads())
    ->UseRealTime();

BENCHMARK_MAIN();