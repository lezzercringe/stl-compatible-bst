#pragma once

#include <utility>

template <typename Key>
struct Node {
  Node() = default;

  template <typename... Args>
  Node(Args&&... args) : key{std::forward<Args>(args)...} {}

  Node* left = nullptr;
  Node* right = nullptr;
  Node* parent = nullptr;

  Key key;
};
