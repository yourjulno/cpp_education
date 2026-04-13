#include <algorithm>
#include <cassert>
#include <cmath>
#include <functional>
#include <iterator>
#include <numeric>
#include <random>
#include <ranges>
#include <tuple>
#include <vector>

template <std::ranges::input_range R>
auto make_vector(R&& range)
{
    using Value = std::ranges::range_value_t<R>;

    std::vector<Value> result;

    if constexpr (std::ranges::sized_range<R>)
    {
        result.reserve(std::ranges::size(range));
    }

    std::ranges::copy(range, std::back_inserter(result));
    return result;
}

// -----------------------------------------------------------------------------
// transform_if = copy_if + transform
// -----------------------------------------------------------------------------

template <std::ranges::input_range R, typename Pred, typename Func>
auto transform_if(R&& range, Pred pred, Func func)
{
    using In  = std::ranges::range_value_t<R>;
    using Out = std::decay_t<std::invoke_result_t<Func&, In>>;

    std::vector<In> filtered;
    std::ranges::copy_if(range, std::back_inserter(filtered), pred);

    std::vector<Out> result;
    result.reserve(filtered.size());
    std::ranges::transform(filtered, std::back_inserter(result), func);

    return result;
}

// -----------------------------------------------------------------------------
// MAE / MSE
// -----------------------------------------------------------------------------

double mae(const std::vector<double>& y_true, const std::vector<double>& y_pred)
{
    assert(y_true.size() == y_pred.size());
    assert(!y_true.empty());

    double sum = std::transform_reduce(
        y_true.begin(), y_true.end(),
        y_pred.begin(),
        0.0,
        std::plus<>(),
        [](const double a, const double b)
        {
            return std::abs(a - b);
        }
    );

    return sum / static_cast<double>(y_true.size());
}

double mse(std::vector<double> const& y_true, std::vector<double> const& y_pred)
{
    assert(y_true.size() == y_pred.size());
    assert(!y_true.empty());

    const double sum = std::transform_reduce(
        y_true.begin(), y_true.end(),
        y_pred.begin(),
        0.0,
        std::plus<>(),
        [](double a, double b)
        {
            double d = a - b;
            return d * d;
        }
    );

    return sum / static_cast<double>(y_true.size());
}

// -----------------------------------------------------------------------------
// Fibonacci view
// -----------------------------------------------------------------------------

class Fibonacci : public std::ranges::view_interface<Fibonacci>
{
public:
    using value_type = std::uint64_t;

    Fibonacci() = default;

    explicit Fibonacci(std::size_t count)
        : m_count(count)
    {
    }

    auto begin() const
    {
        return Iterator{0, m_count, 0, 1};
    }

    auto end() const
    {
        return Iterator{m_count, m_count, 0, 1};
    }

private:
    class Iterator
    {
    public:
        using iterator_concept  = std::input_iterator_tag;
        using iterator_category = std::input_iterator_tag;
        using value_type        = std::uint64_t;
        using difference_type   = std::ptrdiff_t;

        Iterator() = default;

        value_type operator*() const
        {
            return m_current;
        }

        Iterator& operator++()
        {
            ++m_index;
            auto old_next = m_next;
            m_next = m_current + m_next;
            m_current = old_next;
            return *this;
        }

        void operator++(int)
        {
            ++(*this);
        }

        bool operator==(const Iterator& other) const
        {
            return m_index == other.m_index;
        }

    private:
        friend class Fibonacci;

        Iterator(std::size_t index, std::size_t count, value_type current, value_type next)
            : m_index(index), m_count(count), m_current(current), m_next(next)
        {
        }

        std::size_t m_index   = 0;
        std::size_t m_count   = 0;
        value_type  m_current = 0;
        value_type  m_next    = 1;
    };

    std::size_t m_count = 0;
};


int main()
{
    // -------------------------------------------------------------------------
    // ranges::replace
    // -------------------------------------------------------------------------
    {
        std::vector<int> v = {1, 2, 1, 3, 1};
        std::ranges::replace(v, 1, 9);
        assert(v == std::vector<int>({9, 2, 9, 3, 9}));
    }

    // -------------------------------------------------------------------------
    // ranges::fill
    // -------------------------------------------------------------------------
    {
        std::vector<int> v(5);
        std::ranges::fill(v, 7);
        assert(v == std::vector<int>({7, 7, 7, 7, 7}));
    }

    // -------------------------------------------------------------------------
    // ranges::unique
    // -------------------------------------------------------------------------
    {
        std::vector<int> v = {1, 1, 2, 2, 2, 3, 3, 4, 4};
        auto tail = std::ranges::unique(v);
        v.erase(tail.begin(), v.end());
        assert(v == std::vector<int>({1, 2, 3, 4}));
    }

    // -------------------------------------------------------------------------
    // ranges::rotate
    // -------------------------------------------------------------------------
    {
        std::vector<int> v = {1, 2, 3, 4, 5};
        std::ranges::rotate(v, v.begin() + 2);
        assert(v == std::vector<int>({3, 4, 5, 1, 2}));
    }

    // -------------------------------------------------------------------------
    // ranges::sample
    // -------------------------------------------------------------------------
    {
        std::vector<int> v = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        std::vector<int> sample_result;

        std::mt19937 gen(42);
        std::ranges::sample(v, std::back_inserter(sample_result), 4, gen);

        assert(sample_result.size() == 4);

        for (int x : sample_result)
        {
            assert(std::ranges::find(v, x) != v.end());
        }

        auto sorted = sample_result;
        std::ranges::sort(sorted);
        assert(std::ranges::adjacent_find(sorted) == sorted.end());
    }

    // -------------------------------------------------------------------------
    // transform_if
    // -------------------------------------------------------------------------
    {
        std::vector<int> v = {1, 2, 3, 4, 5, 6};

        auto result = transform_if(
            v,
            [](int x) { return x % 2 == 0; },
            [](int x) { return x * x; }
        );

        assert(result == std::vector<int>({4, 16, 36}));
    }

    // -------------------------------------------------------------------------
    // MAE / MSE
    // -------------------------------------------------------------------------
    {
        std::vector<double> y_true = {1.0, 2.0, 3.0, 4.0};
        std::vector<double> y_pred = {1.5, 2.5, 2.0, 5.0};

        assert(std::abs(mae(y_true, y_pred) - 0.75) < 1e-12);
        assert(std::abs(mse(y_true, y_pred) - 0.625) < 1e-12);
    }

    // -------------------------------------------------------------------------
    // views::filter
    // -------------------------------------------------------------------------
    {
        std::vector<int> v = {1, 2, 3, 4, 5, 6};
        auto filtered = v | std::views::filter([](int x) { return x % 2 == 0; });

        assert(make_vector(filtered) == std::vector<int>({2, 4, 6}));
    }

    // -------------------------------------------------------------------------
    // views::drop
    // -------------------------------------------------------------------------
    {
        std::vector<int> v = {1, 2, 3, 4, 5};
        auto dropped = v | std::views::drop(2);

        assert(make_vector(dropped) == std::vector<int>({3, 4, 5}));
    }

    // -------------------------------------------------------------------------
    // views::join
    // -------------------------------------------------------------------------
    {
        std::vector<std::vector<int>> vv = {
            {1, 2},
            {3},
            {4, 5}
        };

        auto joined = vv | std::views::join;
        assert(make_vector(joined) == std::vector<int>({1, 2, 3, 4, 5}));
    }

    // -------------------------------------------------------------------------
    // views::zip
    // -------------------------------------------------------------------------
    {
        std::vector<int> numbers = {1, 2, 3};
        std::vector<char> letters = {'a', 'b', 'c'};

        auto zipped = std::views::zip(numbers, letters);

        std::vector<std::tuple<int, char>> result = {
                    {1, 'a'},
                    {2, 'b'},
                    {3, 'c'}
        };
        assert(
            make_vector(zipped) == result
        );
    }

    // -------------------------------------------------------------------------
    // views::stride
    // -------------------------------------------------------------------------
    {
        std::vector<int> v = {1, 2, 3, 4, 5, 6, 7};
        auto strided = v | std::views::stride(2);

        assert(make_vector(strided) == std::vector<int>({1, 3, 5, 7}));
    }

    // -------------------------------------------------------------------------
    // Fibonacci view
    // -------------------------------------------------------------------------
    {
        Fibonacci fib(10);

        assert(
            make_vector(fib) ==
            std::vector<std::uint64_t>({0, 1, 1, 2, 3, 5, 8, 13, 21, 34})
        );

        auto tail = fib | std::views::drop(5) | std::views::take(3);

        assert(
            make_vector(tail) ==
            std::vector<std::uint64_t>({5, 8, 13})
        );
    }

    return 0;
}