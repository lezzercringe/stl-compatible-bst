#pragma once

#include <lib/node.hpp>
#include <lib/traversals.hpp>

template <typename T, typename Traversal = InOrder<T>>
struct Iterator {
  Iterator(Node<T>* ptr) : ptr_{ptr} {};

  static Iterator GetBegin(Node<T>* root_);
  static Iterator GetEnd(Node<T>* root_);

  using ref_type = const T&;
  using ptr_type = const T*;

  ptr_type operator->();
  ref_type operator*();


  Iterator& operator++();
  Iterator& operator--();

  Node<T>* node_ptr(); // haha

  bool operator==(const Iterator<T, Traversal>& other) const;
  bool operator!=(const Iterator<T, Traversal>& other) const;

private:
  
  Node<T>* ptr_; 
};

template <typename T, typename Traversal>
Node<T>* Iterator<T, Traversal>::node_ptr() {
  return ptr_;
}; 

template <typename T, typename Traversal>
Iterator<T, Traversal> Iterator<T, Traversal>::GetBegin(Node<T>* root_) {
  return Iterator(Traversal::GetInitial(root_));
}

template <typename T, typename Traversal>
Iterator<T, Traversal> Iterator<T, Traversal>::GetEnd(Node<T>* root_) {
  return Iterator(Traversal::GetEnd(root_));
}

template <typename T, typename Traversal>
bool Iterator<T, Traversal>::operator==(const Iterator<T, Traversal>& other) const {
  return ptr_ == other.ptr_;
}

template <typename T, typename Traversal>
bool Iterator<T, Traversal>::operator!=(const Iterator<T, Traversal>& other) const {
  return ptr_ != other.ptr_;
}

template <typename T, typename Traversal>
Iterator<T, Traversal>& Iterator<T, Traversal>::operator++() {
  ptr_ = Traversal::Successor(ptr_);
  return *this;
}

template <typename T, typename Traversal>
Iterator<T, Traversal>& Iterator<T, Traversal>::operator--() {
  ptr_ = Traversal::Predecessor(ptr_);
  return *this;
}

template <typename T, typename Traversal>
Iterator<T, Traversal>::ref_type Iterator<T, Traversal>::operator*() {
  return ptr_->key;
}

template <typename T, typename Traversal>
Iterator<T, Traversal>::ptr_type Iterator<T, Traversal>::operator->() {
  return &ptr_->key;
}
