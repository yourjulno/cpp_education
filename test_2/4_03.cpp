#include <algorithm>
#include <cassert>
#include <vector>

template<typename Container, typename T>
void handle(Container& container, T&& value)
{

}

template<typename Container>
void handle(Container& container, int value) {
    container.push_back(value);
}


template<typename Container, typename... Args>
void push_to_container(Container& container, Args&&... args)
{

    (handle(container, std::forward<Args>(args)), ...);
}

template<typename Container>
concept HasPushBack = requires(Container c, typename Container::value_type value) {
    c.push_back(value);
};

template<HasPushBack Container, typename... Args>
void push_to_container2(Container & container, Args... int_elems)
{
    if constexpr ((std::is_same_v<Args, int> && ...))
    {
        (container.push_back(int_elems), ...);
    }
}

int main()
{
    std::vector<int> int_vector;
    push_to_container(int_vector, 1, 2, 3);

    assert(int_vector[0] == 1);
    assert(int_vector[1] == 2);
    assert(int_vector[2] == 3);

    return 0;
}