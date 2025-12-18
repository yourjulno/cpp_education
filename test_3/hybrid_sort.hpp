#pragma once

#include <vector>
#include <cstddef>

#include <algorithm>
#include <cassert>
#include <numeric>
#include <utility>
#include <vector>
#include <iostream>


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
void split(std::vector<T> & vector, std::size_t left, std::size_t right)
{
    if (right - left <= 16)
    {
        order(vector, left, right);
    }
    else
    {
        auto partition_index = hoare_partition(vector, left, right);

        split(vector, left, partition_index);
        split(vector, partition_index, right);
    }
}

template <typename T>
void sort(std::vector<T> & vector)
{
    if (vector.size() > 1) {
        split(vector, 0, vector.size());
    }
}
