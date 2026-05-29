// Copyright 2022 NNTU-CS
#include "tree.h"

#include <cstdlib>
#include <chrono>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <vector>

int main() {
  // Example usage (required)
  {
    std::vector<char> in = {'1', '2', '3'};
    PMTree tree(in);
    auto perms = getAllPerms(tree);
    std::cout << "All permutations for 1,2,3:\n";
    for (auto& p : perms) {
      for (char c : p) std::cout << c;
      std::cout << " ";
    }
    std::cout << "\n";

    std::cout << "getPerm2(2): ";
    auto p2 = getPerm2(tree, 2);
    for (char c : p2) std::cout << c;
    std::cout << "\n\n";
  }

  // Experiment
  const int MAX_N = 9;
  const int REPEATS = 500;

  // Create result directory (ignore return value)
  (void)system("mkdir -p result");

  std::ofstream out("result/data.txt");
  if (!out.is_open()) {
    std::cerr << "Error: cannot create result/data.txt\n";
    return 1;
  }
  out << "n\tall_ms\tperm1_avg_us\tperm2_avg_us\n";

  for (int n = 1; n <= MAX_N; ++n) {
    // Alphabet: '1','2',... for n<=9 else 'a','b',...
    std::vector<char> symbols(n);
    for (int i = 0; i < n; ++i) symbols[i] = static_cast<char>('1' + i);
    if (n > 9) {
      for (int i = 0; i < n; ++i) symbols[i] = static_cast<char>('a' + i);
    }

    PMTree tree(symbols);
    int64_t totalPerms = tree.getFactorials()[n];

    // getAllPerms
    auto start = std::chrono::high_resolution_clock::now();
    auto allPerms = getAllPerms(tree);
    auto end = std::chrono::high_resolution_clock::now();
    double timeAllMs = std::chrono::duration_cast<std::chrono::milliseconds>(
        end - start).count();

    // getPerm1 (average over REPEATS random queries)
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int64_t> dist(1, totalPerms);
    int64_t sumTimePerm1 = 0;
    for (int rep = 0; rep < REPEATS; ++rep) {
      int64_t num = dist(gen);
      auto s = std::chrono::high_resolution_clock::now();
      auto p = getPerm1(tree, static_cast<int>(num));
      auto e = std::chrono::high_resolution_clock::now();
      sumTimePerm1 += std::chrono::duration_cast<std::chrono::microseconds>(
          e - s).count();
    }
    double avgPerm1Us = static_cast<double>(sumTimePerm1) / REPEATS;

    // getPerm2 (average)
    int64_t sumTimePerm2 = 0;
    for (int rep = 0; rep < REPEATS; ++rep) {
      int64_t num = dist(gen);
      auto s = std::chrono::high_resolution_clock::now();
      auto p = getPerm2(tree, static_cast<int>(num));
      auto e = std::chrono::high_resolution_clock::now();
      sumTimePerm2 += std::chrono::duration_cast<std::chrono::microseconds>(
          e - s).count();
    }
    double avgPerm2Us = static_cast<double>(sumTimePerm2) / REPEATS;

    out << n << "\t" << timeAllMs << "\t" << avgPerm1Us << "\t"
        << avgPerm2Us << "\n";
    std::cout << "n = " << n << " done\n";
  }
  out.close();

  std::cout << "\nExperiment finished. Data saved to result/data.txt\n";
  std::cout << "Run 'python3 plot.py' to generate graph.\n";

  return 0;
}
