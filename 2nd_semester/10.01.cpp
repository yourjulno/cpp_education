#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

namespace vector_growth::exp 
{
    struct VectorGrowth
    {
        std::size_t old_c = 0;
        std::size_t new_c = 0;
        double ratio = 0;
    };

    static std::vector<VectorGrowth> track_vector_growth(std::size_t n) 
    {
        std::vector<int> v;
        std::vector<VectorGrowth> events;

        std::size_t last_cap = v.capacity();

        for (std::size_t i = 0; i < n; ++i) 
        {
            const std::size_t old_cap = v.capacity();
            const std::size_t old_size = v.size();

            v.push_back(static_cast<int>(i));

            const std::size_t new_cap = v.capacity();
            const std::size_t new_size = v.size();

            if (new_cap != old_cap) 
            {

                double ratio = 0.0;
                if (old_cap == 0) 
                {
                    ratio = 0.0;
                } else {
                    ratio = static_cast<double>(new_cap) / static_cast<double>(old_cap);
                }

                events.push_back(VectorGrowth
                {
                    .old_c = old_cap,
                    .new_c = new_cap,
                    .ratio = ratio,
                });
            }

            last_cap = new_cap;
        }

        return events;
    }

    struct RatioStats 
    {
        double min = 0.0;
        double max = 0.0;
        double mean = 0.0;
        double median = 0.0;
    };

    static RatioStats compute_ratio_stats(std::vector<double> ratios) 
    {
        RatioStats s{};
        if (ratios.empty()) return s;

        std::sort(ratios.begin(), ratios.end());
        s.min = ratios.front();
        s.max = ratios.back();
        s.mean = std::accumulate(ratios.begin(), ratios.end(), 0.0) / static_cast<double>(ratios.size());

        const std::size_t mid = ratios.size() / 2;
        if (ratios.size() % 2 == 1) {
            s.median = ratios[mid];
        } else {
            s.median = (ratios[mid - 1] + ratios[mid]) / 2.0;
        }
        return s;
    }

    void print_events(const std::vector<VectorGrowth>& events)
    {

        std::cout << std::left
                << std::setw(12) << "old_cap"
                << std::setw(12) << "new_cap"
                << std::setw(12) << "ratio"
                << "\n";

        std::cout << std::string(48, '-') << "\n";

        for (const auto& [old_c, new_c, ratio] : events)
        {
            std::cout << std::left
                    << std::setw(12) << old_c
                    << std::setw(12) << new_c;

            if (old_c == 0)
            {
                std::cout << std::setw(12) << "N/A";
            } 
            else 
            {
                std::cout << std::setw(12) << std::fixed << std::setprecision(6) << ratio;
            }
            std::cout << "\n";
        }
        std::cout << "\n";
    }

    void run_basic_tests(const std::vector<VectorGrowth>& events, std::size_t n)
    {
        if (n >= 64) 
        {
            assert(!events.empty());
        }

        for (const auto& e : events) 
        {
            if (e.old_c != 0) 
            {
                assert(e.ratio > 1.0);
            }
        }
    }
}

int main(int argc, char** argv) 
{

    std::size_t n = 2048;
    if (argc >= 2) {
        try {
            n = static_cast<std::size_t>(std::stoull(argv[1]));
        } catch (...)
        {
            std::cerr << "Usage: " << argv[0] << " [N]\n";
            return 2;
        }
    }

    const auto events = vector_growth::exp::track_vector_growth(n);
    run_basic_tests(events, n);
    print_events(events);

    std::vector<double> ratios;
    ratios.reserve(events.size());
    for (const auto& e : events) 
    {
        if (e.old_c != 0) ratios.push_back(e.ratio);
    }

    const auto [min, max, mean, median] = vector_growth::exp::compute_ratio_stats(ratios);
    if (ratios.empty()) 
    {
        std::cout << "  Not enough data.\n";
        return 0;
    }

    std::cout << "  min    = " << std::fixed << std::setprecision(6) << min << "\n";
    std::cout << "  max    = " << std::fixed << std::setprecision(6) << max << "\n";
    std::cout << "  mean   = " << std::fixed << std::setprecision(6) << mean << "\n";
    std::cout << "  median = " << std::fixed << std::setprecision(6) << median << "\n\n";

    return 0;
}
