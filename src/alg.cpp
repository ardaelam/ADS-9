// Copyright 2022 NNTU-CS
#include "tree.h"  // NOLINT(build/include_order)

#include <algorithm>  // NOLINT(build/include_order)
#include <cstdint>    // NOLINT(build/include_order)
#include <vector>     // NOLINT(build/include_order)

std::vector<PMTree::Node*> PMTree::buildLevel(const std::vector<char>& elems) {
  std::vector<Node*> nodes;
  for (size_t i = 0; i < elems.size(); ++i) {
    Node* node = new Node(elems[i]);
    std::vector<char> rest;
    rest.reserve(elems.size() - 1);
    for (size_t j = 0; j < elems.size(); ++j) {
      if (j != i) rest.push_back(elems[j]);
    }
    node->children = buildLevel(rest);
    nodes.push_back(node);
  }
  return nodes;
}

PMTree::PMTree(const std::vector<char>& elements) {
  std::vector<char> sorted = elements;
  std::sort(sorted.begin(), sorted.end());
  n = static_cast<int>(sorted.size());
  factorials.resize(n + 1);
  factorials[0] = 1;
  for (int i = 1; i <= n; ++i) {
    factorials[i] = factorials[i - 1] * i;
  }
  root = new Node('\0');
  root->children = buildLevel(sorted);
}

static void collectPerms(PMTree::Node* node, std::vector<char>& current,
                         std::vector<std::vector<char>>& result) {
  if (node->value != '\0') current.push_back(node->value);
  if (node->children.empty()) {
    result.push_back(current);
  } else {
    for (auto child : node->children) {
      collectPerms(child, current, result);
    }
  }
  if (node->value != '\0') current.pop_back();
}

std::vector<std::vector<char>> getAllPerms(PMTree& tree) {
  std::vector<std::vector<char>> result;
  std::vector<char> current;
  for (auto child : tree.getRoot()->children) {
    collectPerms(child, current, result);
  }
  return result;
}

std::vector<char> getPerm1(PMTree& tree, int num) {
  auto all = getAllPerms(tree);
  if (num < 1 || static_cast<size_t>(num) > all.size()) return {};
  return all[num - 1];
}

std::vector<char> getPerm2(PMTree& tree, int num) {
  int n = tree.getN();
  if (num < 1 || static_cast<int64_t>(num) > tree.getFactorials()[n]) {
    return {};
  }

  std::vector<char> result;
  const auto& fact = tree.getFactorials();
  std::vector<PMTree::Node*> currentLevel = tree.getRoot()->children;
  int currentNum = num;

  for (int pos = 0; pos < n; ++pos) {
    int64_t blockSize = fact[n - pos - 1];
    int idx = (currentNum - 1) / blockSize;
    result.push_back(currentLevel[idx]->value);
    currentNum = (currentNum - 1) % blockSize + 1;
    currentLevel = currentLevel[idx]->children;
  }
  return result;
}
