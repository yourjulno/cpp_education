#pragma once

#include <vector>
#include <cstddef>
#include <algorithm>
#include <cassert>
#include <utility>

template <typename T>
void order(std::vector<T> & vector, std::size_t left, std::size_t right)
{
    for (auto i = left + 1; i < right; ++i)
    {
        for (auto j = i; j > left; --j)
        {
            if (vector[j - 1] > vector[j])
            {
                std::swap(vector[j], vector[j - 1]);
            }
        }
    }
}

template <typename T>
T median_of_three(std::vector<T> & vector, std::size_t left, std::size_t right)
{
    std::size_t mid = left + (right - left - 1) / 2;

    if (vector[left] > vector[mid])
        std::swap(vector[left], vector[mid]);
    if (vector[left] > vector[right - 1])
        std::swap(vector[left], vector[right - 1]);
    if (vector[mid] > vector[right - 1])
        std::swap(vector[mid], vector[right - 1]);

    return vector[mid];
}

template <typename T>
std::size_t hoare_partition(std::vector<T> & vector, std::size_t left, std::size_t right)
{
    T pivot = median_of_three(vector, left, right);

    std::size_t i = left - 1;
    std::size_t j = right;

    while (true) {
        do {
            ++i;
        } while (vector[i] < pivot);

        do {
            --j;
        } while (vector[j] > pivot && j > left);

        if (i >= j) {
            break;
        }

        std::swap(vector[i], vector[j]);
    }

    return j + 1;
}

template <typename T>
void split_with_threshold(std::vector<T> & vector, std::size_t left, std::size_t right, std::size_t threshold)
{
    if (right - left <= threshold)
    {
        order(vector, left, right);
    }
    else
    {
        auto partition_index = hoare_partition(vector, left, right);

        split_with_threshold(vector, left, partition_index, threshold);
        split_with_threshold(vector, partition_index, right, threshold);
    }
}

template <typename T>
void sort(std::vector<T> & vector)
{
    if (vector.size() > 1) {
        constexpr std::size_t DEFAULT_THRESHOLD = 16;
        split_with_threshold(vector, 0, vector.size(), DEFAULT_THRESHOLD);
    }
}

template <typename T>
void sort_with_threshold(std::vector<T> & vector, std::size_t threshold)
{
    if (vector.size() > 1) {
        split_with_threshold(vector, 0, vector.size(), threshold);
    }
}