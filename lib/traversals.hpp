#pragma once

#include <lib/node.hpp>
#include <cassert>

template<typename T>
struct PreOrder {
  static Node<T>* GetInitial(Node<T>* root) {
    return root->left;
  };

  static Node<T>* Successor(Node<T>* node) {
    assert(node != nullptr);
    if (node->left != nullptr) {
      return node->left;
    }

    if (node->right != nullptr) {
      return node->right;
    }

    if (node->parent->left == node && node->parent->right != nullptr) {
      return node->parent->right;
    }

    auto* parent = node->parent;

    while(parent != nullptr) {
      if (parent->left == node && parent->right != nullptr) {
        return parent->right;
      }

      node = node->parent;
      parent = node->parent;
    }

    return node;
  }

  static Node<T>* Predecessor(Node<T>* node) {
    if (node->parent == nullptr) {
      // called from "endian" node, so we have to manually find rightmost leaf of whole tree
      auto is_leaf = [](Node<T>* node) { return !node->left && !node->right; };
      auto* it = node->left;

      while (!is_leaf(it)) {
        if (it->right != nullptr) {
          it = it->right;
        } else {
          it = it->left;
        }
      }

      return it;
    }

  // If u has a left sibling, ls, then pred(u) is the rightmost descendent of ls
    auto* ls = node->parent->left;
    if (ls != node && ls != nullptr) {
      while (ls->right != nullptr || ls->left != nullptr) {
        if (ls->right != nullptr) {
          ls = ls->right;
        } else {
          ls = ls->left;
        }
      }

      return ls;
    }

  // Otherwise, pred(u) is parent(u).
    return node->parent;
  }

  static Node<T>* GetEnd(Node<T>* root) {
    return root;
  }
};

template<typename T>
struct InOrder {
  static Node<T>* GetEnd(Node<T>* root) {
    // InOrder traversal ends with the largest element,
    // in our case it's "end" node which is the fake root of the tree
    // and whole tree is it's left child
    return root;
  };

  static Node<T>* GetInitial(Node<T>* root) {
    // InOrder traversal start with the most left element

    auto* it = root;
    while (it->left != nullptr) {
      it = it->left;
    }

    return it;
  };
  static Node<T>* Successor(Node<T>* node) {
    // In inorder traversal the succers is either the most left element right subtree,
    // or node's parent

    Node<T>* it; 

    if (node->right != nullptr) {
      it = node->right;
      while (it->left != nullptr) {
        it = it->left;
      }
    } else {
      it = node->parent;

      while (it->left != node) {
        if (it->parent == nullptr) {
          return it;
        }

        node = it;
        it = it->parent;
      }
    }

    return it;
  }

  static Node<T>* Predecessor(Node<T>* node) {
    Node<T>* it; 

    if (node->left != nullptr) {
      it = node->left;
      while (it->right != nullptr) {
        it = it->right;
      }
    } else {
      it = node->parent;

      while (it->right != node) {
        /* real tree is just a subtree of fake "end" node,
         * so it's unnecessary to check for nullptrs */

        if (it->parent == nullptr) {
          return it;
        }

        node = it;
        it = it->parent;
      }
    }

    return it;
  }
};

template<typename T>
struct PostOrder {
  static Node<T>* GetEnd(Node<T>* root) {
    return root;
  }

  static Node<T>* GetInitial(Node<T>* root) {
    while (!IsLeaf(root)) {
      if (root->left != nullptr) {
        root = root->left;
      } else {
        root = root->right;
      }
    }

    return root;
  }

  static Node<T>* Successor(Node<T>* node) {
    if (node->parent == nullptr) return node; // reached "endian" node
    if (node->parent->right == node) {
      return node->parent;
    } 

    /* at this moment node can only be left child
       if node has a right sibling, r, succ(node) is the leftmost leaf in r’s subtree
       otherwise succ(node) is parent(node). */

    if (auto* r = node->parent->right) {
      while (!IsLeaf(r)) {
        if (r->left != nullptr) {
          r = r->left;
        } else {
          r = r->right;
        }
      }

      return r;
    }

    return node->parent;
  }; 

  static Node<T>* Predecessor(Node<T>* node) {
  // If u has a right child, r, then pred(u) is r
    if (node->right != nullptr) { 
      return node->right;
    }

  // Otherwise If u has a left child, l, then pred(u) is l.
    if (node->left != nullptr) {
      return node->left;
    }

  // Otherwise if u has a left sibling, ls, then pred(u) is ls
    if (node->parent->left != node && node->parent->left != nullptr) {
      return node->parent->left;
    }

  /* Otherwise if u has an ancestor, v, which is a right child and has a left sibling,
     vls, then pred(u) is vls */

    auto* v = node->parent;
    while (v->parent != nullptr && (v->parent->right != v || v->parent->left == nullptr)) {
      v = v->parent;
    }

    if (v->parent == nullptr) {
      return v;  // should check it because it's edge case 
    }

    return v->left;

  }; 

  static bool IsLeaf(Node<T>* node) {
    return node->left == nullptr && node->right == nullptr;
  };
};

