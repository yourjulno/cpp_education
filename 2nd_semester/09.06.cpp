#include <cassert>
#include <iostream>

namespace semester_second
{

class Iterator
{
public:
    Iterator()
        : m_prev(0),
          m_curr(1)
    {
    }

    Iterator& operator++()
    {
        const int next = m_prev + m_curr;
        m_prev = m_curr;
        m_curr = next;
        return *this;
    }

    Iterator operator++(int)
    {
        Iterator tmp = *this;
        ++(*this);
        return tmp;
    }

    int operator*() const
    {
        return m_prev;
    }

    bool operator==(const Iterator& other) const
    {
        return m_prev == other.m_prev && m_curr == other.m_curr;
    }

    bool operator!=(const Iterator& other) const
    {
        return !(*this == other);
    }

private:
    int m_prev;
    int m_curr;
};

} // namespace semester_second

int main()
{
    using semester_second::Iterator;

    Iterator it;

    // (0 1 1 2 3 5 8 13 21 34)
    for (int i = 0; i < 10; ++i)
    {
        std::cout << *it << ' ';
        ++it;
    }
    std::cout << '\n';

    Iterator a;
    assert(*a == 0);
    ++a;
    assert(*a == 1);
    a++;
    assert(*a == 1);

    return 0;
}
