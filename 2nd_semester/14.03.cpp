#include <algorithm>
#include <cassert>
#include <future>
#include <iterator>
#include <ranges>
#include <type_traits>
#include <vector>
#include <numeric>

template <std::ranges::view V, typename F>
auto for_each(V view, F function) -> void
{
    auto begin = std::begin(view);
    auto end = std::end(view);

    if (auto size = std::distance(begin, end), half = size / 2; size > 16)
    {
        std::ranges::subrange range(begin, std::next(begin, half));

        auto future = std::async(std::launch::async, for_each<decltype(range), F>, range, function);

        for_each(std::ranges::subrange(std::end(range), end), function);

        future.get();
    }
    else
    {
        std::for_each(begin, end, function);
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
