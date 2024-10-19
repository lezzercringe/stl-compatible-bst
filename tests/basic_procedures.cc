#include <gtest/gtest.h>
#include <experimental/random>
#include <lib/set.hpp>
#include <ranges>

TEST(InsertionTest, BasicProcedures) {
  Set<int> tree;

  int key = 52;

  auto [success, it] = tree.insert(key);
  ASSERT_EQ(success, true);
  ASSERT_EQ(*it, key);
}

TEST(MultipleKeysInsertion, BasicProcedures) {
  Set<int> tree;

  int a = 52;
  int b = 42;

  auto [a_bool, a_it] = tree.insert(a);
  auto [b_bool, b_it] = tree.insert(b);

  ASSERT_EQ(a_bool, true);
  ASSERT_EQ(*a_it, a);

  ASSERT_EQ(b_bool, true);
  ASSERT_EQ(*b_it, b);
}

TEST(DoubleInsertionTest, BasicProcedures) {
  Set<int> tree;

  int key = 52;

  auto [success, it] = tree.insert(key);
  auto [other, it2] = tree.insert(key);

  ASSERT_EQ(success, true);
  ASSERT_EQ(*it, key);

  ASSERT_EQ(other, false);
  ASSERT_EQ(*it2, key);
}

TEST(KeyDeletionTest, BasicProcedures) {
  Set<int> set;

  constexpr int nodes_count = 500;
  std::vector<int> input_data;
  input_data.reserve(nodes_count);

  for (int i = 0; i < nodes_count; ++i) {
    int num = std::experimental::randint(-1000, 1000);
    input_data.push_back(num);
    set.emplace(num);
  }

  for (int i = 0; i < nodes_count; ++i) {
    if (set.empty())
      break;
    int index = std::experimental::randint(0, nodes_count - 1);
    set.erase(input_data[index]);

    for (int i : set) {
      ASSERT_NE(i, input_data[index]);
    }
  }
}
