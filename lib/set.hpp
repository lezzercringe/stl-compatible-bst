#pragma once

#include <memory>
#include <functional>
#include <utility>

#include <lib/node.hpp>
#include <lib/iterator.hpp>
#include <lib/reverse_iterator.hpp>
#include <lib/traversals.hpp>

template<
  typename Key,
  typename Comparator = std::less<Key>,
  typename Alloc = std::allocator<Key>
>

class Set {
public:
  // tree-specific aliases's
  using preorder = PreOrder<Key>;
  using inorder = InOrder<Key>;
  using postorder = PostOrder<Key>;

  // named requirements tags (Container)
  using value_type = Key;
  using reference = Key&;
  using const_reference = const Key&;

  // allocator aware container requirments
  using allocator_type = std::allocator_traits<Alloc>::template rebind_alloc<Node<Key>>;

  // regular iterators
  using iterator = Iterator<Key>; 
  using const_iterator = iterator;
  using preorder_iterator = Iterator<Key, preorder>; 
  using preorder_const_iterator = Iterator<Key, preorder>;
  using postorder_iterator = Iterator<Key, postorder>; 
  using postorder_const_iterator = Iterator<Key, postorder>;

  // reverse iterators
  using reverse_iterator = ReverseIterator<Key>; 
  using const_reverse_iterator = iterator;
  using preorder_reverse_iterator = ReverseIterator<Key, preorder>; 
  using preorder_const_reverse_iterator = ReverseIterator<Key, preorder>;
  using postorder_reverse_iterator = ReverseIterator<Key, postorder>; 
  using postorder_const_reverse_iterator = ReverseIterator<Key, postorder>;

  using difference_type = long long;
  using size_type = size_t;


  // named requirements tags (AssociativeContainer)
  using key_type = Key;
  using key_compare = Comparator;
  using value_compare = Comparator;
  using node_type = Node<Key>;

  // destructor
  ~Set();

  // constructors
  Set();

  Set(const Set<Key, Comparator, Alloc>& other);
  Set(Set<Key, Comparator, Alloc>&& other) noexcept;

  Set& operator=(const Set<Key, Comparator, Alloc>& other);
  Set& operator=(Set<Key, Comparator, Alloc>&& other) noexcept;

  // iterator access
  [[nodiscard]] const_iterator cbegin() const;
  [[nodiscard]] const_iterator cend() const;

  template <typename Traversal = inorder>
  [[nodiscard]] Iterator<Key, Traversal> begin() const;

  template <typename Traversal = inorder>
  [[nodiscard]] Iterator<Key, Traversal> end() const;

  // reverse iterator access
  [[nodiscard]] const_iterator crbegin() const;
  [[nodiscard]] const_iterator crend() const;

  template <typename Traversal = inorder>
  [[nodiscard]] ReverseIterator<Key, Traversal> rbegin();

  template <typename Traversal = inorder>
  [[nodiscard]] ReverseIterator<Key, Traversal> rend();

  // comparison
  bool operator==(const Set<Key, Comparator, Alloc>& other) const;
  bool operator!=(const Set<Key, Comparator, Alloc>& other) const;

  // business methods
  template <typename... Args>
  std::pair<bool, iterator> emplace(Args&&... args);

  std::pair<bool, iterator> insert(Key key); 
  size_type erase(const Key& key);
  const_iterator erase(iterator it);
  const_iterator find(const Key& key);
  void clear();

  // size & utility
  [[nodiscard]] size_type size() const;
  [[nodiscard]] bool empty() const;
  [[nodiscard]] bool contains(const Key& key) const;

private:
  Node<Key>* ConstructEmptyNode();

  template<typename... Args>
  Node<Key>* ConstructNodeWithKey(Args&&... args);
  void DropTree();
  void DropNode(Node<Key>* ptr);

  void EraseNodeByPointer(Node<Key>* ptr);

  Node<Key>* root_ = nullptr;
  size_type size_ = 0;

  Comparator comparator_;
  allocator_type allocator_;
};


template<typename Key, typename Comparator, typename Alloc>
Set<Key, Comparator, Alloc>::Set()
  : root_{ConstructEmptyNode()},
    size_{0} {
}

template<typename Key, typename Comparator, typename Alloc>
Set<Key, Comparator, Alloc>::~Set() {
  DropTree();
}

template<typename Key, typename Comparator, typename Alloc>
void Set<Key, Comparator, Alloc>::DropTree() {
  auto postorder = [this](Node<Key>* node, auto& this_closure) { 
    if (node == nullptr) return;

    this_closure(node->left, this_closure);
    this_closure(node->right, this_closure);
    DropNode(node);
  };

  // TODO: probably get rid of recursion

  postorder(root_, postorder);
}

template<typename Key, typename Comparator, typename Alloc>
void Set<Key, Comparator, Alloc>::DropNode(Node<Key>* ptr) {
  std::allocator_traits<allocator_type>::destroy(allocator_, ptr);
  allocator_.deallocate(ptr, 1);
};

template<typename Key, typename Comparator, typename Alloc>
Node<Key>* Set<Key, Comparator, Alloc>::ConstructEmptyNode() {
  auto* ptr = allocator_.allocate(1);
  std::allocator_traits<allocator_type>::construct(allocator_, ptr);
  return ptr;
};

template<typename Key, typename Comparator, typename Alloc>
template<typename... Args>
Node<Key>* Set<Key, Comparator, Alloc>::ConstructNodeWithKey(Args&&... args) {
  auto* ptr = allocator_.allocate(1);
  std::allocator_traits<allocator_type>::construct(allocator_, ptr, std::forward<Args>(args)...);
  return ptr;
};

template<typename Key, typename Comparator, typename Alloc>
template<typename... Args>
std::pair<bool, typename Set<Key, Comparator, Alloc>::iterator> Set<Key, Comparator, Alloc>::emplace(Args&&... args) {
  using namespace std::placeholders;
  auto deleter = std::bind(&Set::DropNode, this, _1);
  auto new_node = std::unique_ptr<Node<Key>, decltype(deleter)>(ConstructNodeWithKey(std::forward<Args>(args)...), deleter);
  auto* it = root_->left;
  auto* parent = root_;
  bool is_last_move_left = true;

  while (it != nullptr) {
    parent = it;

    if (std::equal_to<Key>{}(it->key, new_node->key)) {
      return { false, Iterator<Key>(it) }; // key already exists 
    } else if (comparator_(new_node->key, it->key)) {
      is_last_move_left = true;
      it = it->left;
    } else {
      is_last_move_left = false;
      it = it->right;
    } 
  }


  new_node->parent = parent;
  auto* raw_ptr_ = new_node.release();
  if (is_last_move_left) {
    parent->left = raw_ptr_;
  } else {
    parent->right = raw_ptr_; 
  }

  ++size_;
  return { true, Iterator<Key>{raw_ptr_}}; 
}

template<typename Key, typename Comparator, typename Alloc>
std::pair<bool, typename Set<Key, Comparator, Alloc>::iterator> Set<Key, Comparator, Alloc>::insert(Key key) {
  return emplace(std::forward<Key>(key));
}; 

template<typename Key, typename Comparator, typename Alloc>
template <typename Traversal>
[[nodiscard]] Iterator<Key, Traversal> Set<Key, Comparator, Alloc>::
begin() const {
    return Iterator<Key, Traversal>::GetBegin(root_);
};

template<typename Key, typename Comparator, typename Alloc>
template <typename Traversal>
[[nodiscard]] Iterator<Key, Traversal> Set<Key, Comparator, Alloc>::
end() const {
    return Iterator<Key, Traversal>::GetEnd(root_);
};

template<typename Key, typename Comparator, typename Alloc>
template <typename Traversal>
[[nodiscard]] ReverseIterator<Key, Traversal> Set<Key, Comparator, Alloc>::
rend() {
    return ReverseIterator(Iterator<Key, Traversal>(root_));
};

template<typename Key, typename Comparator, typename Alloc>
template <typename Traversal>
[[nodiscard]] ReverseIterator<Key, Traversal> Set<Key, Comparator, Alloc>::
rbegin() {
    return ReverseIterator(--Iterator<Key, Traversal>::GetEnd(root_));
};

template<typename Key, typename Comparator, typename Alloc>
Set<Key, Comparator, Alloc>::Set(const Set<Key, Comparator, Alloc>& other) {
  // TODO: probably get rid of recursion here (pohuy)
  allocator_ = std::allocator_traits<allocator_type>::select_on_container_copy_construction(other.allocator_);
  
  auto preorder_copy = [this](Node<Key>* node, auto& this_closure) -> void {
    if (node == nullptr) return;
    insert(node->key);
    this_closure(node->left, this_closure);
    this_closure(node->right, this_closure);
  };

  preorder_copy(other.root_, preorder_copy);
};

template<typename Key, typename Comparator, typename Alloc>
Set<Key, Comparator, Alloc>& Set<Key, Comparator, Alloc>::operator=(const Set<Key, Comparator, Alloc>& other) {
  if (this == &other) {
    return *this;
  }

  DropTree();

  for (auto i : other) {
    insert(i);
  }

  return *this;
};

template<typename Key, typename Comparator, typename Alloc>
Set<Key, Comparator, Alloc>::Set(Set<Key, Comparator, Alloc>&& other) noexcept {
  root_ = std::exchange(other.root_, ConstructEmptyNode());
  size_ = std::exchange(other.size_, 0);
};


template<typename Key, typename Comparator, typename Alloc>
Set<Key, Comparator, Alloc>::size_type Set<Key, Comparator, Alloc>::erase(const Key& key) {
  auto it = find(key);
  if (it == end()) return 0; // key was not found
  erase(it);

  return 1; 
};

template<typename Key, typename Comparator, typename Alloc>
Set<Key, Comparator, Alloc>::const_iterator Set<Key, Comparator, Alloc>::find(const Key& key) {
  auto* it = root_->left;

  while (it != nullptr) {
    if (std::equal_to<Key>{}(key, it->key)) {
      return Iterator<Key>(it);
    }

    if (comparator_(it->key, key)) {
      it = it->right;
    } else {
      it = it->left;
    } 
  }

  return end(); 
};

template<typename Key, typename Comparator, typename Alloc>
void Set<Key, Comparator, Alloc>::EraseNodeByPointer(Node<Key>* node) {
  auto get_parents_pointer = [](Node<Key>* ptr) -> Node<Key>*& {
    return ptr->parent->right == ptr ? ptr->parent->right : ptr->parent->left;
  };

  if (node->left == nullptr && node->right == nullptr) {
    get_parents_pointer(node) = nullptr;
    DropNode(node);
    return;
  }

  if (node->left != nullptr && node->right != nullptr) {
    auto* successor = InOrder<Key>::Successor(node);
    node->key = successor->key;

    EraseNodeByPointer(successor);
    return;
  }

  if (node->left != nullptr) { // only single left child
    get_parents_pointer(node) = node->left;
    node->left->parent = node->parent;
    DropNode(node);
  } else { // only single right child
    get_parents_pointer(node) = node->right;
    node->right->parent = node->parent;
    DropNode(node);
  }
};

template<typename Key, typename Comparator, typename Alloc>
Set<Key, Comparator, Alloc>::const_iterator Set<Key, Comparator, Alloc>::erase(iterator it) {
  --size_; // erasure should occure anyway
  auto successor = ++Iterator(it);
  EraseNodeByPointer(it.node_ptr());
  return successor;
};

template<typename Key, typename Comparator, typename Alloc>
Set<Key, Comparator, Alloc>& Set<Key, Comparator, Alloc>::operator=(Set<Key, Comparator, Alloc>&& other) noexcept {
  if (this == &other) {
    return *this;
  }
  
  DropTree(); // this drops everything including "endian" root node
  root_ = std::exchange(other.root_, ConstructEmptyNode());
  size_ = std::exchange(other.size_, 0);

  if constexpr (std::allocator_traits<allocator_type>::propagate_on_container_move_assignment::value) {
    allocator_ =  std::exchange(other.allocator_, allocator_type());
  }

  return *this;
};

template<typename Key, typename Comparator, typename Alloc>
[[nodiscard]] Set<Key, Comparator, Alloc>::const_iterator Set<Key, Comparator, Alloc>::cbegin() const {
  return begin();
};

template<typename Key, typename Comparator, typename Alloc>
[[nodiscard]] Set<Key, Comparator, Alloc>::const_iterator Set<Key, Comparator, Alloc>::cend() const {
  return end();
};

template<typename Key, typename Comparator, typename Alloc>
[[nodiscard]] Set<Key, Comparator, Alloc>::const_iterator Set<Key, Comparator, Alloc>::crbegin() const {
  return rbegin();
};

template<typename Key, typename Comparator, typename Alloc>
[[nodiscard]] Set<Key, Comparator, Alloc>::const_iterator Set<Key, Comparator, Alloc>::crend() const {
  return rend();
};

template<typename Key, typename Comparator, typename Alloc>
[[nodiscard]] Set<Key, Comparator, Alloc>::size_type Set<Key, Comparator, Alloc>::size() const {
  return size_;
};

template<typename Key, typename Comparator, typename Alloc>
[[nodiscard]] bool Set<Key, Comparator, Alloc>::empty() const {
  return size_ == 0;
};

template<typename Key, typename Comparator, typename Alloc>
[[nodiscard]] bool Set<Key, Comparator, Alloc>::contains(const Key& key) const {
  return find(key) == cend();
};

template<typename Key, typename Comparator, typename Alloc>
void Set<Key, Comparator, Alloc>::clear() {
  DropTree();
  root_ = ConstructEmptyNode();
};
