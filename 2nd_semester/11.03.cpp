#include <algorithm>
#include <cassert>
#include <functional>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace semester_second {

template <typename RandomIt, typename Compare>
void insertion_sort(RandomIt first, RandomIt last, Compare comp)
{
    for (auto it = std::next(first); it != last; ++it)
    {
        for (auto jt = it; jt != first; --jt)
        {
            auto prev = std::prev(jt);
            if (!comp(*jt, *prev))
            {
                break;
            }
            std::iter_swap(jt, prev);
        }
    }
}

template <typename RandomIt, typename Compare>
auto median_of_three_value(RandomIt first, RandomIt last, Compare comp)
    -> typename std::iterator_traits<RandomIt>::value_type
{
    auto mid = first;
    std::advance(mid, std::distance(first, last) / 2);

    auto a = *first;
    auto b = *mid;
    auto c = *std::prev(last);

    if (comp(b, a)) std::swap(a, b);
    if (comp(c, a)) std::swap(a, c);
    if (comp(c, b)) std::swap(b, c);

    return b;
}

template <typename RandomIt, typename Compare>
RandomIt hoare_partition(RandomIt first, RandomIt last, Compare comp)
{
    using T = typename std::iterator_traits<RandomIt>::value_type;

    const T pivot = median_of_three_value(first, last, comp);

    auto i = first;
    auto j = std::prev(last);

    while (true)
    {
        while (comp(*i, pivot))
        {
            ++i;
        }

        while (comp(pivot, *j))
        {
            if (j == first)
            {
                break;
            }
            --j;
        }

        if (i >= j)
        {
            return std::next(j);
        }

        std::iter_swap(i, j);
        ++i;

        if (j == first)
        {
            return std::next(j);
        }
        --j;
    }
}

template <typename RandomIt, typename Compare>
void hybrid_quicksort(RandomIt first, RandomIt last, Compare comp)
{
    constexpr std::ptrdiff_t kInsertionThreshold = 16;

    const auto n = std::distance(first, last);
    if (n <= 1)
    {
        return;
    }

    if (n <= kInsertionThreshold)
    {
        insertion_sort(first, last, comp);
        return;
    }

    auto cut = hoare_partition(first, last, comp);
    hybrid_quicksort(first, cut, comp);
    hybrid_quicksort(cut, last, comp);
}

template <typename RandomIt, typename Compare>
void sort(RandomIt first, RandomIt last, Compare comp)
{
    using Cat = typename std::iterator_traits<RandomIt>::iterator_category;
    static_assert(std::is_base_of_v<std::random_access_iterator_tag, Cat>,
                  "semester_second::sort requires random-access iterators");

    hybrid_quicksort(first, last, comp);
}


template <typename RandomIt>
void sort(RandomIt first, RandomIt last)
{
    using Value = typename std::iterator_traits<RandomIt>::value_type;
    semester_second::sort(first, last, std::less<Value>{});
}

template <typename Container, typename Compare>
void sort(Container& c, Compare comp)
{
    sort(std::begin(c), std::end(c), comp);
}

template <typename Container>
void sort(Container& c)
{
    semester_second::sort(std::begin(c), std::end(c));
}

} // namespace semester_second


bool greater_int(int a, int b)
{
    return a > b;
}

int main()
{
    {
        std::vector<int> v = {5, 4, 3, 2, 1};
        semester_second::sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
        std::cout << "Test 1 passed: default ascending sort\n";
    }

    {
        std::vector<int> v = {1, 5, 2, 4, 3};
        semester_second::sort(v.begin(), v.end(), greater_int);
        assert(std::ranges::is_sorted(v, std::greater<int>{}));
        std::cout << "Test 2 passed: custom free function comparator\n";
    }

    {
        std::vector<std::string> v = {"banana", "apple", "cherry", "date"};
        semester_second::sort(v.begin(), v.end(), std::less<std::string>{});
        assert(std::ranges::is_sorted(v, std::less<std::string>{}));
        std::cout << "Test 3 passed: std::less comparator\n";
    }

    {
        std::vector<int> v = {10, 3, 7, 1, 9, 2};

        semester_second::sort(v.begin(), v.end(),
            [](int a, int b)
            {
                return a > b; // сортировка по убыванию
            });

        assert(std::ranges::is_sorted(v, std::greater<int>{}));
        std::cout << "Test 4 passed: lambda comparator\n";
    }

    {
        std::vector<double> v = {3.14, 2.71, 1.41, 1.61, 0.57, 2.35};
        semester_second::sort(v);
        assert(std::ranges::is_sorted(v));
        std::cout << "Test 5 passed: double sorting\n";
    }

    {
        std::vector<int> v = {1, 2, 3, 4, 5};
        semester_second::sort(v);
        assert(std::ranges::is_sorted(v));
        std::cout << "Test 6 passed: already sorted\n";
    }

    {
        std::vector<int> v = {5, 4, 3, 2, 1};
        semester_second::sort(v);
        assert(std::ranges::is_sorted(v));
        std::cout << "Test 7 passed: reverse sorted\n";
    }

    {
        std::vector<int> v = {5, 2, 8, 2, 5, 8, 1, 1, 3};
        semester_second::sort(v);
        assert(std::ranges::is_sorted(v));
        std::cout << "Test 8 passed: duplicates\n";
    }

    {
        std::vector<int> v;
        semester_second::sort(v);
        assert(v.empty());
        std::cout << "Test 9 passed: empty vector\n";
    }

    {
        std::vector<int> v = {42};
        semester_second::sort(v);
        assert(std::ranges::is_sorted(v));
        std::cout << "Test 10 passed: single element\n";
    }

    return 0;
}