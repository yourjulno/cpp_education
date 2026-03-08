#include <cassert>
#include <stack>

template <typename T>
class Stack
{
public:
    void push(const T& x)
    {
        if (m_stack.empty())
        {
            m_stack.push(x);
            m_min = x;
            return;
        }

        if (x < m_min)
        {
            m_stack.push(static_cast<T>(T(2) * x - m_min));
            m_min = x;
        }
        else
        {
            m_stack.push(x);
        }
    }

    T top() const
    {
        assert(!m_stack.empty());
        const T t = m_stack.top();
        return (t < m_min) ? m_min : t;
    }

    void pop()
    {
        assert(!m_stack.empty());
        const T t = m_stack.top();

        if (t < m_min)
        {
            // restore previous min
            m_min = static_cast<T>(T(2) * m_min - t);
        }

        m_stack.pop();
    }

    T min() const
    {
        assert(!m_stack.empty());
        return m_min;
    }

    bool empty() const noexcept { return m_stack.empty(); }
    std::size_t size() const noexcept { return m_stack.size(); }

private:
    std::stack<T> m_stack;
    T m_min{};
};

int main()
{
    Stack<int> stack;

    stack.push(1); assert(stack.top() == 1 && stack.min() == 1);
    stack.push(3); assert(stack.top() == 3 && stack.min() == 1);
    stack.push(2); assert(stack.top() == 2 && stack.min() == 1);

    assert(stack.top() == 2 && stack.min() == 1);

    stack.pop();    assert(stack.top() == 3 && stack.min() == 1);
    stack.pop();    assert(stack.top() == 1 && stack.min() == 1);
    stack.pop();    assert(stack.empty());
}