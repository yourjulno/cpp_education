#include <algorithm>
#include <cassert>
#include <cstddef>
#include <numeric>
#include <utility>
#include <vector>
#include <iostream>

////////////////////////////////////////////////////////////////////////////////////

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

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
T median_of_three(std::vector<T> & vector, std::size_t left, std::size_t right)
{
    std::size_t mid = left + (right - left - 1) / 2;

    // Сортируем первый, средний и последний элементы
    if (vector[left] > vector[mid])
        std::swap(vector[left], vector[mid]);
    if (vector[left] > vector[right - 1])
        std::swap(vector[left], vector[right - 1]);
    if (vector[mid] > vector[right - 1])
        std::swap(vector[mid], vector[right - 1]);

    // Возвращаем медиану (средний элемент после сортировки)
    return vector[mid];
}

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
std::size_t hoare_partition(std::vector<T> & vector, std::size_t left, std::size_t right)
{
    T pivot = median_of_three(vector, left, right);

    std::size_t i = left - 1;
    std::size_t j = right;

    while (true) {
        // элемент слева, который больше или равен опорному
        do {
            ++i;
        } while (vector[i] < pivot);

        // элемент справа, который меньше или равен опорному
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

////////////////////////////////////////////////////////////////////////////////////

//  рекурсивная функция гибридной сортировки
template <typename T>
void split(std::vector<T> & vector, std::size_t left, std::size_t right)
{
    // сортировка вставкамм
    if (right - left <= 16)
    {
        order(vector, left, right);
    }
    else
    {
        // схема хоара
        auto partition_index = hoare_partition(vector, left, right);

        split(vector, left, partition_index);
        split(vector, partition_index, right);
    }
}

////////////////////////////////////////////////////////////////////////////////////

template <typename T>
void sort(std::vector<T> & vector)
{
    if (vector.size() > 1) {
        split(vector, 0, vector.size());
    }
}

////////////////////////////////////////////////////////////////////////////////////


int main()
{
    // цулые
    {
        auto size = 1'000uz;
        std::vector<int> vector(size, 0);

        for (auto i = 0uz; i < size; ++i)
        {
            vector[i] = size - i;
        }

        sort(vector);
        assert(std::ranges::is_sorted(vector));
        std::cout << "Test 1 passed: Integer sorting\n";
    }

    // дробные
    {
        std::vector<double> vector = {3.14, 2.71, 1.41, 1.61, 0.57, 2.35};

        sort(vector);
        assert(std::ranges::is_sorted(vector));
        std::cout << "Test 2 passed: Double sorting\n";
    }

    // строки
    {
        std::vector<std::string> vector = {"banana", "apple", "cherry", "date", "elderberry"};

        sort(vector);
        assert(std::ranges::is_sorted(vector));
        std::cout << "Test 3 passed: String sorting\n";
    }

    // уже по порядку
    {
        std::vector<int> vector = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        sort(vector);
        assert(std::ranges::is_sorted(vector));
        std::cout << "Test 4 passed: Already sorted array\n";
    }

    // обратный
    {
        std::vector<int> vector = {10, 9, 8, 7, 6, 5, 4, 3, 2, 1};

        sort(vector);
        assert(std::ranges::is_sorted(vector));
        std::cout << "Test 5 passed: Reverse sorted array\n";
    }

    // повторы
    {
        std::vector<int> vector = {5, 2, 8, 2, 5, 8, 1, 1, 3};

        sort(vector);
        assert(std::ranges::is_sorted(vector));
        std::cout << "Test 6 passed: Array with duplicates\n";
    }

    // пустой
    {
        std::vector<int> vector;

        sort(vector);
        assert(vector.empty());
        std::cout << "Test 7 passed: Empty array\n";
    }

    // один эл-т
    {
        std::vector<int> vector = {42};

        sort(vector);
        assert(std::ranges::is_sorted(vector));
        std::cout << "Test 8 passed: Single element array\n";
    }

    std::cout << "\nAll tests passed successfully!\n";
}

////////////////////////////////////////////////////////////////////////////////////

/*
Обоснование временной сложности гибридного алгоритма:

1. Быстрая сортировка Хоара:
   - Средняя временная сложность: O(n log n)
   - Худший случай: O(n²) - но выбор медианы из трех значительно снижает вероятность худшего случая

2. Сортировка вставками для маленьких массивов:
   - Временная сложность: O(k²) для k элементов
   - Эффективна для маленьких массивов (k ≤ 16)

3. Гибридный подход:
   - В среднем: O(n log n)
   - В худшем случае: O(n log n) с высокой вероятностью благодаря выбору медианы из трех
   - На практике работает быстрее чистой быстрой сортировки благодаря:
     * Устранению рекурсии для маленьких подмассивов
     * Улучшенному выбору опорного элемента
     * Лучшему использованию кэша процессора

Преимущества выбора медианы из трех:
- Снижает вероятность выбора наихудшего опорного элемента
- Улучшает баланс разделения массива
- В среднем дает лучшее время работы чем случайный выбор опорного элемента
*/