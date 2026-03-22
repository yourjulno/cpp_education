#include <benchmark/benchmark.h>

#include <functional>

#if defined(_MSC_VER)
  #define NOINLINE __declspec(noinline)
#elif defined(__GNUC__) || defined(__clang__)
  #define NOINLINE __attribute__((noinline))
#else
  #define NOINLINE
#endif

static volatile std::int32_t g_sink = 0;

NOINLINE std::int32_t free_fn(std::int32_t x)
{
    return x + 1;
}

struct Member
{
    NOINLINE std::int32_t call(std::int32_t x) const { return x + 1; }
};

struct Base
{
    virtual ~Base() = default;
    virtual std::int32_t call(std::int32_t x) const = 0;
};
struct Derived final : Base
{
    NOINLINE std::int32_t call(std::int32_t x) const override { return x + 1; }
};

struct Functor
{
    NOINLINE std::int32_t operator()(std::int32_t x) const { return x + 1; }
};

static void BM_FreeFunction(benchmark::State& state)
{
    std::int32_t x = 123;
    for (auto _ : state)
    {
        auto r = free_fn(x);
        benchmark::DoNotOptimize(r);
        g_sink = r;
        benchmark::ClobberMemory();
        x = r;
    }
}
BENCHMARK(BM_FreeFunction);

static void BM_MemberFunction(benchmark::State& state)
{
    Member obj{};
    std::int32_t x = 123;
    for (auto _ : state)
    {
        auto r = obj.call(x);
        benchmark::DoNotOptimize(r);
        g_sink = r;
        benchmark::ClobberMemory();
        x = r;
    }
}
BENCHMARK(BM_MemberFunction);

static void BM_VirtualFunction(benchmark::State& state)
{
    Derived d{};
    const Base* p = &d;
    std::int32_t x = 123;
    for (auto _ : state)
    {
        auto r = p->call(x);
        benchmark::DoNotOptimize(r);
        g_sink = r;
        benchmark::ClobberMemory();
        x = r;
    }
}
BENCHMARK(BM_VirtualFunction);

static void BM_Functor(benchmark::State& state)
{
    Functor f{};
    std::int32_t x = 123;
    for (auto _ : state)
    {
        auto r = f(x);
        benchmark::DoNotOptimize(r);
        g_sink = r;
        benchmark::ClobberMemory();
        x = r;
    }
}
BENCHMARK(BM_Functor);

static void BM_LambdaAuto(benchmark::State& state)
{
    auto lam = [](std::int32_t x) NOINLINE -> std::int32_t { return x + 1; };

    std::int32_t x = 123;
    for (auto _ : state)
    {
        auto r = lam(x);
        benchmark::DoNotOptimize(r);
        g_sink = r;
        benchmark::ClobberMemory();
        x = r;
    }
}
BENCHMARK(BM_LambdaAuto);

static void BM_StdFunction(benchmark::State& state)
{
    auto lam = [](std::int32_t x) NOINLINE -> std::int32_t { return x + 1; };

    std::function<std::int32_t(std::int32_t)> fn = lam;

    std::int32_t x = 123;
    for (auto _ : state)
    {
        auto r = fn(x);
        benchmark::DoNotOptimize(r);
        g_sink = r;
        benchmark::ClobberMemory();
        x = r;
    }
}
BENCHMARK(BM_StdFunction);

BENCHMARK_MAIN();