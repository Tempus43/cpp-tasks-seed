#include <gtest/gtest.h>
#include <vector>
#include <algorithm>
#include <random>
#include "sorting.h"

// Вспомогательная функция: проверяет, что вектор отсортирован
template <typename T>
bool is_sorted(const std::vector<T>& vec)
{
    return std::is_sorted(vec.begin(), vec.end());
}

// ---------- Тесты пузырьковой сортировки ----------
TEST(BubbleSort, Empty)
{
    std::vector<int> v;
    bubble_sort(v.begin(), v.end());
    EXPECT_TRUE(is_sorted(v));
}

TEST(BubbleSort, SingleElement)
{
    std::vector<int> v = {5};
    bubble_sort(v.begin(), v.end());
    EXPECT_TRUE(is_sorted(v));
}

TEST(BubbleSort, Sorted)
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    bubble_sort(v.begin(), v.end());
    EXPECT_TRUE(is_sorted(v));
}

TEST(BubbleSort, Reversed)
{
    std::vector<int> v = {5, 4, 3, 2, 1};
    bubble_sort(v.begin(), v.end());
    EXPECT_TRUE(is_sorted(v));
}

TEST(BubbleSort, Duplicates)
{
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    bubble_sort(v.begin(), v.end());
    EXPECT_TRUE(is_sorted(v));
}

TEST(BubbleSort, Random)
{
    std::vector<int> v(100);
    std::iota(v.begin(), v.end(), 0);
    std::shuffle(v.begin(), v.end(), std::mt19937{42});
    bubble_sort(v.begin(), v.end());
    EXPECT_TRUE(is_sorted(v));
}

// ---------- Тесты быстрой сортировки ----------
TEST(QuickSort, Empty)
{
    std::vector<int> v;
    quick_sort(v.begin(), v.end());
    EXPECT_TRUE(is_sorted(v));
}

TEST(QuickSort, SingleElement)
{
    std::vector<int> v = {42};
    quick_sort(v.begin(), v.end());
    EXPECT_TRUE(is_sorted(v));
}

TEST(QuickSort, Sorted)
{
    std::vector<int> v = {1, 2, 3, 4, 5};
    quick_sort(v.begin(), v.end());
    EXPECT_TRUE(is_sorted(v));
}

TEST(QuickSort, Reversed)
{
    std::vector<int> v = {5, 4, 3, 2, 1};
    quick_sort(v.begin(), v.end());
    EXPECT_TRUE(is_sorted(v));
}

TEST(QuickSort, Duplicates)
{
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    quick_sort(v.begin(), v.end());
    EXPECT_TRUE(is_sorted(v));
}

TEST(QuickSort, Random)
{
    std::vector<int> v(1000);
    std::iota(v.begin(), v.end(), 0);
    std::shuffle(v.begin(), v.end(), std::mt19937{123});
    quick_sort(v.begin(), v.end());
    EXPECT_TRUE(is_sorted(v));
}