#pragma once

#include <gtest/gtest.h>
#include <algorithm>
#include <experimental/random>
#include <lib/set.hpp>

class TraversalsTest : public testing::Test {
protected:
  using preorder = Set<int>::preorder;
  using postorder = Set<int>::postorder;

  const int nodes_count = 500;
  Set<int> tree;
  std::vector<int> input_nodes{15, 10, 12, 11, 20};
  std::vector<int> preorder_expected{15, 10, 12, 11, 20};
  std::vector<int> postorder_expected{ 11, 12, 10, 20, 15 };

  void SetUp() override {
    std::ranges::for_each(input_nodes, [this](int n) { tree.emplace(n); });
  }

};
