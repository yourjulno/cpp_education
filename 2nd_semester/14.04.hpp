#pragma once

#include <cstddef>
#include <functional>
#include <future>
#include <iterator>
#include <ranges>
#include <thread>
#include <utility>
#include <vector>

namespace parallel::detail
{
    template <std::ranges::view V, typename T, typename F>
    class Task
    {
    public:
        auto operator()(V view, T init, F function) const -> T
        {
            for (auto&& value : view)
            {
                init += std::invoke(function, value);
            }

            return init;
        }
    };
}

namespace parallel
{
    template <typename T, std::ranges::view V, typename F>
    auto fold(V view, T init, unsigned thread_count, F function) -> T
    {
        auto begin = std::begin(view);
        auto end = std::end(view);

        const auto size = std::distance(begin, end);

        if (size <= 0)
        {
            return init;
        }

        if (thread_count == 0)
        {
            thread_count = 1;
        }

        if (thread_count > static_cast<unsigned>(size))
        {
            thread_count = static_cast<unsigned>(size);
        }

        std::vector<std::pair<std::future<T>, std::jthread>> futures(thread_count - 1);

        const auto base_step = size / thread_count;
        const auto remainder = size % thread_count;

        for (std::size_t i = 0; i < futures.size(); ++i)
        {
            const auto step = base_step + (i < static_cast<std::size_t>(remainder) ? 1 : 0);
            auto range = std::ranges::subrange(begin, std::next(begin, step));

            std::packaged_task<T(decltype(range), T, F)> task{detail::Task<decltype(range), T, F>()};

            futures[i].first = task.get_future();
            futures[i].second = std::jthread(std::move(task), range, T(), function);

            std::advance(begin, step);
        }

        auto range = std::ranges::subrange(begin, end);

        init += detail::Task<decltype(range), T, F>()(range, T(), function);

        for (auto& [future, thread] : futures)
        {
            init += future.get();
        }

        return init;
    }
}
