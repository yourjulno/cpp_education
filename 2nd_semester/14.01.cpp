#include <algorithm>
#include <cassert>
#include <ranges>
#include <thread>
#include <type_traits>
#include <vector>
#include <numeric>

template <std::ranges::view V, typename F>
class Task
{
public:
    void operator()(V view, F function) const
    {
        std::for_each(std::begin(view), std::end(view), function);
    }
};

template <typename F>
void for_each(std::ranges::view auto view, F function)
{
    auto begin = std::begin(view);
    auto end = std::end(view);

    if (auto size = std::distance(begin, end); size > 0)
    {
        auto concurrency = std::thread::hardware_concurrency();

        if (concurrency == 0)
        {
            concurrency = 1;
        }

        if (static_cast<decltype(size)>(concurrency) > size)
        {
            concurrency = static_cast<unsigned>(size);
        }

        auto step = size / concurrency;

        {
            std::vector<std::jthread> threads(concurrency - 1);

            for (auto i = 0uz; i < std::size(threads); ++i)
            {
                auto range = std::ranges::subrange(begin, std::next(begin, step));

                threads[i] = std::jthread(Task<decltype(range), F>(), range, function);

                std::advance(begin, step);
            }

            auto range = std::ranges::subrange(begin, end);

            Task<decltype(range), F>()(range, function);
        }
    }
}

struct Increment
{
    void operator()(int& value) const
    {
        ++value;
    }
};

int main()
{
    std::vector<int> vector(1 << 10, 0);

//  ----------------------------------------------------

    std::iota(vector.begin(), vector.end(), 1);

//  ----------------------------------------------------

    static_assert(std::is_same_v<decltype(for_each(std::views::all(vector), Increment())), void>);
    static_assert(std::is_same_v<decltype(std::for_each(std::begin(vector), std::end(vector), Increment())), Increment>);

//  ----------------------------------------------------

    for_each(std::views::all(vector), Increment());

//  ----------------------------------------------------

    assert(vector.front() == 2);
    assert(vector.back() == 1025);
}