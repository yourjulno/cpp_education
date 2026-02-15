#include <iostream>
#include <memory>
#include <queue>
#include <stack>

namespace semester_second {

struct Node
{
    int value{};
    std::shared_ptr<Node> left{};
    std::shared_ptr<Node> right{};
    std::weak_ptr<Node> parent{};

    explicit Node(int v) : value(v) {}

    ~Node()
    {
        std::cout << "Node(" << value << ") destroyed\n";
    }
};

class Tree
{
public:

    std::shared_ptr<Node> root{};

    void traverse_v1() const
    {
        if (!root)
        {
            std::cout << "(empty)\n";
            return;
        }

        std::queue<std::shared_ptr<Node>> q;
        q.push(root);

        while (!q.empty())
        {
            auto n = q.front();
            q.pop();

            std::cout << n->value << ' ';

            if (n->left) q.push(n->left);
            if (n->right) q.push(n->right);
        }

        std::cout << '\n';
    }

    void traverse_v2() const
    {
        if (!root)
        {
            std::cout << "(empty)\n";
            return;
        }

        std::stack<std::shared_ptr<Node>> st;
        st.push(root);

        while (!st.empty()) {
            auto n = st.top();
            st.pop();

            std::cout << n->value << ' ';

            if (n->right) st.push(n->right);
            if (n->left) st.push(n->left);
        }

        std::cout << '\n';
    }
};

void link_left(const std::shared_ptr<Node>& parent, const std::shared_ptr<Node>& child)
{
    parent->left = child;
    child->parent = parent;
}

void link_right(const std::shared_ptr<Node>& parent, const std::shared_ptr<Node>& child)
{
    parent->right = child;
    child->parent = parent;
}

} // namespace semester_second

