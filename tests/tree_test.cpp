// /tests/tree_test.cpp

#include <gtest/gtest.h>

#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "2nd_semester/09.02.hpp"

namespace {

class CoutCapture
{
public:
    CoutCapture() : old_(std::cout.rdbuf(buf_.rdbuf())) {}
    ~CoutCapture() { std::cout.rdbuf(old_); }

    std::string str() const { return buf_.str(); }

private:
    std::stringstream buf_;
    std::streambuf* old_;
};

semester_second::Tree build_tree_1_to_7()
{
    using namespace semester_second;

    Tree t;
    t.root = std::make_shared<Node>(1);

    auto n2 = std::make_shared<Node>(2);
    auto n3 = std::make_shared<Node>(3);
    link_left(t.root, n2);
    link_right(t.root, n3);

    auto n4 = std::make_shared<Node>(4);
    auto n5 = std::make_shared<Node>(5);
    link_left(n2, n4);
    link_right(n2, n5);

    auto n6 = std::make_shared<Node>(6);
    auto n7 = std::make_shared<Node>(7);
    link_left(n3, n6);
    link_right(n3, n7);

    return t;
}

int count_occurrences(const std::string& s, const std::string& needle) {
    int count = 0;
    std::size_t pos = 0;
    while ((pos = s.find(needle, pos)) != std::string::npos) {
        ++count;
        pos += needle.size();
    }
    return count;
}

}  // namespace

TEST(TreeTest, EmptyTree_BFS_PrintsEmpty)
{
    semester_second::Tree t;

    CoutCapture cap;
    t.traverse_v1();

    EXPECT_EQ(cap.str(), "(empty)\n");
}

TEST(TreeTest, EmptyTree_DFS_PrintsEmpty)
{
    semester_second::Tree t;

    CoutCapture cap;
    t.traverse_v2();

    EXPECT_EQ(cap.str(), "(empty)\n");
}

TEST(TreeTest, TraverseV1_BFS_Order)
{
    auto t = build_tree_1_to_7();

    CoutCapture cap;
    t.traverse_v1();

    EXPECT_EQ(cap.str(), "1 2 3 4 5 6 7 \n");
}


TEST(TreeTest, TraverseV2_DFS_PreorderOrder)
{
    auto t = build_tree_1_to_7();

    CoutCapture cap;
    t.traverse_v2();

    EXPECT_EQ(cap.str(), "1 2 4 5 3 6 7 \n");
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
