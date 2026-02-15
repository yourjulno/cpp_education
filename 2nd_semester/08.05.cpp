// /src/main.cpp

#include <cassert>
#include <chrono>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

template <typename D = std::chrono::duration<double>>
class Timer
{
public:
    using clock_t = std::chrono::steady_clock;

    explicit Timer(std::string scope)
        : m_scope(std::move(scope)),
          m_started(false),
          m_start(clock_t::now()) {}

    void start()
    {
        if (m_started)
        {
            return;
        }
        m_start = clock_t::now();
        m_started = true;
    }

    void stop()
    {
        if (!m_started)
        {
            return;
        }
        const auto end = clock_t::now();
        const auto dur = std::chrono::duration_cast<D>(end - m_start);
        m_periods.push_back(std::chrono::duration<double>(dur));
        m_started = false;
    }

    double average() const
    {
        double acc = 0.0;
        if (m_periods.empty())
        {
            return acc;
        }

        for (const auto d : m_periods)
        {
            acc += d.count();
        }

        return acc / static_cast<double>(m_periods.size());
    }

    std::size_t count() const { return m_periods.size(); }

private:
    std::string m_scope{};
    bool m_started;
    clock_t::time_point m_start;
    std::vector<std::chrono::duration<double>> m_periods{};
};

double calculate(std::size_t size)
{
    double x = 0.0;
    for (std::size_t i = 0; i < size; ++i)
    {
        x += std::pow(std::sin(static_cast<double>(i)), 2.0)
           + std::pow(std::cos(static_cast<double>(i)), 2.0);
    }
    return x;
}

bool equal(double x, double y, double epsilon = 1e-6)
{
    return std::abs(x - y) < epsilon;
}

int main()
{
    Timer<> timer("main : timer");

    for (int k = 0; k < 5; ++k)
    {
        timer.start();
        assert(equal(calculate(1'000'000), 1'000'000));
        timer.stop();
    }

    std::cout << std::fixed << std::setprecision(6)
              << "average (s) = " << timer.average()
              << " | measurements = " << timer.count()
              << '\n';

    return 0;
}
