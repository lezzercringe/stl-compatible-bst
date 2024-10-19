#pragma once

#include <lib/iterator.hpp>

template <typename T, typename Traversal = InOrder<T>>
struct ReverseIterator {
  ReverseIterator(Iterator<T, Traversal> it) : it_{it} {};

  using ref_type = const T&;
  using ptr_type = const T*;

  ptr_type operator->();
  ref_type operator*();

  ReverseIterator& operator++();
  ReverseIterator& operator--();

  bool operator==(const ReverseIterator<T, Traversal>& other);
  bool operator!=(const ReverseIterator<T, Traversal>& other);

private:
  Iterator<T, Traversal> it_;
};

template <typename T, typename Traversal>
bool ReverseIterator<T, Traversal>::operator==(const ReverseIterator<T, Traversal>& other) {
  return other.it_ == it_;
}

template <typename T, typename Traversal>
bool ReverseIterator<T, Traversal>::operator!=(const ReverseIterator<T, Traversal>& other) {
  return other.it_ != it_;
}

template <typename T, typename Traversal>
ReverseIterator<T, Traversal>& ReverseIterator<T, Traversal>::operator++() {
  --it_;
  return *this;
}; 

template <typename T, typename Traversal>
ReverseIterator<T, Traversal>& ReverseIterator<T, Traversal>::operator--() {
  ++it_;
  return *this;
}; 

template <typename T, typename Traversal>
ReverseIterator<T, Traversal>::ptr_type ReverseIterator<T, Traversal>::operator->() {
  return &*it_;
}

template <typename T, typename Traversal>
ReverseIterator<T, Traversal>::ref_type ReverseIterator<T, Traversal>::operator*() {
  return *it_;
}
