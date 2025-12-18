#include <algorithm>
#include <cassert>

double max_from_packed(auto... xs)
{
    return std::max({xs...});
}

double min_from_packed(auto... xs)
{
    return std::min({xs...});
}

double sum(auto... xs)
{
    return (xs + ...     );
}

double average(auto... xs)
{
    // кол-во элементов, не байты
    std::size_t count = sizeof...(xs);
    auto summary = sum(xs...);
    return summary / count;
}

int main()
{
    assert(sum(1.0, 2.0, 3.0) == 6);

    assert(min_from_packed(1, 2, 3) == 1);

    assert(max_from_packed(1, 2, 3) == 3);

    assert(average(1.0, 2.0, 3.0) == 2);

}
