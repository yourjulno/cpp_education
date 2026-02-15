#include <iterator>
#include <memory>
#include <utility>

template <typename T>
class List
{
private:
    struct Node
    {
        T x{};
        std::shared_ptr<Node> next{};
        std::weak_ptr<Node> prev{};

        Node() = default;

        explicit Node(T value)
            : x(std::move(value))
        {
        }
    };

public:
    class Iterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using difference_type = std::ptrdiff_t;
        using pointer = T*;
        using reference = T&;

        Iterator(std::shared_ptr<Node> node = nullptr, std::shared_ptr<Node> tail = nullptr)
            : m_node(std::move(node)),
              m_tail(std::move(tail))
        {
        }

        Iterator& operator++()
        {
            if (m_node)
            {
                m_node = m_node->next;
            }
            return *this;
        }

        Iterator operator++(int)
        {
            Iterator tmp = *this;
            ++(*this);
            return tmp;
        }

        Iterator& operator--()
        {
            if (!m_node)
            {
                // --end() -> last element
                m_node = m_tail;
                return *this;
            }

            m_node = m_node->prev.lock();
            return *this;
        }

        Iterator operator--(int)
        {
            Iterator tmp = *this;
            --(*this);
            return tmp;
        }

        reference operator*() const
        {
            return m_node->x;
        }

        pointer operator->() const
        {
            return &m_node->x;
        }

        friend bool operator==(const Iterator& lhs, const Iterator& rhs)
        {
            return lhs.m_node == rhs.m_node;
        }

        friend bool operator!=(const Iterator& lhs, const Iterator& rhs)
        {
            return !(lhs == rhs);
        }

    private:
        std::shared_ptr<Node> m_node;
        std::shared_ptr<Node> m_tail;
    };

    Iterator begin() const
    {
        return Iterator(m_head, m_tail);
    }

    Iterator end() const
    {
        return Iterator(nullptr, m_tail);
    }

    void push_back(T x)
    {
        auto node = std::make_shared<Node>(std::move(x));

        if (!m_head)
        {
            m_head = node;
            m_tail = node;
            return;
        }

        node->prev = m_tail;
        m_tail->next = node;
        m_tail = node;
    }

private:
    std::shared_ptr<Node> m_head{};
    std::shared_ptr<Node> m_tail{};
};

int main()
{
    List<int> list;

    list.push_back(1);
    list.push_back(2);
    list.push_back(3);

    for (auto it = std::begin(list); it != std::end(list); ++it)
    {
        (void)*it;
    }

    for ([[maybe_unused]] auto element : list)
    {
    }

    for (auto it = std::end(list); it != std::begin(list); )
    {
        --it;
        (void)*it;
    }

    return 0;
}
