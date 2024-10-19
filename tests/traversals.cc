#include <gtest/gtest.h>
#include <algorithm>
#include <experimental/random>
#include <lib/set.hpp>
#include <tests/test_fixture.hpp>

TEST_F(TraversalsTest, InOrderSuccessor) {
  std::ranges::sort(input_nodes);
  std::vector<int> result;
  result.reserve(input_nodes.size());
  for (int i: tree) {
    result.push_back(i);
  }

  ASSERT_EQ(result, input_nodes);
}

TEST_F(TraversalsTest, InOrderReverse) {
  std::ranges::sort(input_nodes, std::greater{});
  std::vector<int> result;
  result.reserve(input_nodes.size());

  for (auto it = tree.rbegin(); it != tree.rend(); ++it) {
    result.push_back(*it);
  }

  ASSERT_EQ(result, input_nodes);
}

TEST_F(TraversalsTest, PreorderSuccessor) {
  std::vector<int> result;
  result.reserve(input_nodes.size());

  for (auto it = tree.begin<preorder>(); it != tree.end<preorder>(); ++it) {
    result.push_back(*it);
  }

  ASSERT_EQ(result, preorder_expected);
}

TEST_F(TraversalsTest, PreorderReverse) {
  std::vector<int> result;
  result.reserve(input_nodes.size());

  std::ranges::reverse(preorder_expected);

  for (auto it = tree.rbegin<preorder>(); it != tree.rend<preorder>(); ++it) {
    result.push_back(*it);
  }

  ASSERT_EQ(result, preorder_expected);
}

TEST_F(TraversalsTest, PostOrderSuccessor) {
  std::vector<int> result;
  result.reserve(input_nodes.size());

  for (auto it = tree.begin<postorder>(); it != tree.end<postorder>(); ++it) {
    result.push_back(*it);
  }

  ASSERT_EQ(result, postorder_expected);
}

TEST_F(TraversalsTest, PostOrderReverse) {
  std::vector<int> result;
  result.reserve(input_nodes.size());

  std::ranges::reverse(postorder_expected);

  for (auto it = tree.rbegin<postorder>(); it != tree.rend<postorder>(); ++it) {
    result.push_back(*it);
  }

  ASSERT_EQ(result, postorder_expected);
}
