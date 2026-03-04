//

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <ranges>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace semester_second {

template <typename RandomIt>
void insertion_sort(RandomIt first, RandomIt last)
{
    for (auto it = std::next(first); it != last; ++it)
    {
        for (auto jt = it; jt != first; --jt)
        {
            auto prev = std::prev(jt);
            if (!(*jt < *prev))
            {
                break;
            }
            std::iter_swap(jt, prev);
        }
    }
}

template <typename RandomIt>
auto median_of_three_value(RandomIt first, RandomIt last) -> typename std::iterator_traits<RandomIt>::value_type
{
    auto mid = first;
    std::advance(mid, std::distance(first, last) / 2);

    auto a = *first;
    auto b = *mid;
    auto c = *std::prev(last);

    if (b < a) std::swap(a, b);
    if (c < a) std::swap(a, c);
    if (c < b) std::swap(b, c);
    return b;
}

template <typename RandomIt>
RandomIt hoare_partition(RandomIt first, RandomIt last)
{
    using T = typename std::iterator_traits<RandomIt>::value_type;

    const T pivot = median_of_three_value(first, last);

    auto i = first;
    auto j = std::prev(last);

    while (true)
    {
        while (*i < pivot)
        {
            ++i;
        }
        while (pivot < *j)
        {
            if (j == first) break;
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

template <typename RandomIt>
void hybrid_quicksort(RandomIt first, RandomIt last)
{
    constexpr std::ptrdiff_t kInsertionThreshold = 16;

    const auto n = std::distance(first, last);
    if (n <= 1)
    {
        return;
    }

    if (n <= kInsertionThreshold)
    {
        insertion_sort(first, last);
        return;
    }

    auto cut = hoare_partition(first, last);
    hybrid_quicksort(first, cut);
    hybrid_quicksort(cut, last);
}

template <typename RandomIt>
void sort(RandomIt first, RandomIt last)
{
    using Cat = typename std::iterator_traits<RandomIt>::iterator_category;
    static_assert(std::is_base_of_v<std::random_access_iterator_tag, Cat>,
                  "semester_second::sort requires random-access iterators");

    hybrid_quicksort(first, last);
}

template <typename Container>
void sort(Container& c)
{
    sort(std::begin(c), std::end(c));
}

} // namespace semester_second

int main()
{

    // integers
    {
        auto size = 1'000uz;
        std::vector<int> v(size, 0);

        for (auto i = 0uz; i < size; ++i) {
            v[i] = static_cast<int>(size - i);
        }

        semester_second::sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
        std::cout << "Test 1 passed: Integer sorting\n";
    }

    // doubles
    {
        std::vector<double> v = {3.14, 2.71, 1.41, 1.61, 0.57, 2.35};
        semester_second::sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
        std::cout << "Test 2 passed: Double sorting\n";
    }

    // strings
    {
        std::vector<std::string> v = {"banana", "apple", "cherry", "date", "elderberry"};
        semester_second::sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
        std::cout << "Test 3 passed: String sorting\n";
    }

    // already sorted
    {
        std::vector<int> v = {1,2,3,4,5,6,7,8,9,10};
        semester_second::sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
        std::cout << "Test 4 passed: Already sorted array\n";
    }

    // reverse sorted
    {
        std::vector<int> v = {10,9,8,7,6,5,4,3,2,1};
        semester_second::sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
        std::cout << "Test 5 passed: Reverse sorted array\n";
    }

    // duplicates
    {
        std::vector<int> v = {5, 2, 8, 2, 5, 8, 1, 1, 3};
        semester_second::sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
        std::cout << "Test 6 passed: Array with duplicates\n";
    }

    // empty
    {
        std::vector<int> v;
        semester_second::sort(v.begin(), v.end());
        assert(v.empty());
        std::cout << "Test 7 passed: Empty array\n";
    }

    // single element
    {
        std::vector<int> v = {42};
        semester_second::sort(v.begin(), v.end());
        assert(std::ranges::is_sorted(v));
        std::cout << "Test 8 passed: Single element array\n";
    }

    return 0;
}
