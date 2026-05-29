// Copyright 2022 NNTU-CS
#ifndef INCLUDE_TREE_H_
#define INCLUDE_TREE_H_

#include <cstdint>
#include <vector>

class PMTree {
 public:
  struct Node {
    char value;
    std::vector<Node*> children;
    explicit Node(char val) : value(val) {}
    ~Node() {
      for (auto child : children) delete child;
    }
  };

  explicit PMTree(const std::vector<char>& elements);
  ~PMTree() { delete root; }

  Node* getRoot() const { return root; }
  int getN() const { return n; }
  const std::vector<int64_t>& getFactorials() const { return factorials; }

 private:
  Node* root;
  int n;
  std::vector<int64_t> factorials;

  static std::vector<Node*> buildLevel(const std::vector<char>& elems);
};

std::vector<std::vector<char>> getAllPerms(PMTree& tree);
std::vector<char> getPerm1(PMTree& tree, int num);
std::vector<char> getPerm2(PMTree& tree, int num);

inline std::vector<char> getPerm(PMTree& tree, int num) {
  return getPerm2(tree, num);
}

#endif  // INCLUDE_TREE_H_
