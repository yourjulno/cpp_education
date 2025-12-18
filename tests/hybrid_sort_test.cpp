#include <gtest/gtest.h>
#include "../test_3/hybrid_sort.hpp"
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <numeric>

TEST(HybridSortTest, IntegerSorting) {
    auto size = 1000u;
    std::vector<int> vector(size, 0);

    for (auto i = 0u; i < size; ++i) {
        vector[i] = size - i;
    }

    sort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(HybridSortTest, DoubleSorting) {
    std::vector<double> vector = {3.14, 2.71, 1.41, 1.61, 0.57, 2.35};
    sort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(HybridSortTest, StringSorting) {
    std::vector<std::string> vector = {"banana", "apple", "cherry", "date", "elderberry"};
    sort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(HybridSortTest, AlreadySorted) {
    std::vector<int> vector = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    sort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(HybridSortTest, ReverseSorted) {
    std::vector<int> vector = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};
    sort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(HybridSortTest, WithDuplicates) {
    std::vector<int> vector = {5, 2, 8, 2, 5, 8, 1, 1, 3};
    sort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(HybridSortTest, EmptyArray) {
    std::vector<int> vector;
    sort(vector);
    EXPECT_TRUE(vector.empty());
}

TEST(HybridSortTest, SingleElement) {
    std::vector<int> vector = {42};
    sort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

TEST(HybridSortTest, LargeRandomArray) {
    std::vector<int> vector(10000);
    std::iota(vector.begin(), vector.end(), 0);

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(vector.begin(), vector.end(), g);

    sort(vector);
    EXPECT_TRUE(std::ranges::is_sorted(vector));
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}