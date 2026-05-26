// Copyright 2022 NNTU-CS
#include "tree.h"
#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <cmath>
#include <cstdlib>

using namespace std::chrono;

int main() {
    {
        std::vector<char> in = {'1','2','3'};
        PMTree tree(in);
        auto perms = getAllPerms(tree);
        std::cout << "Все перестановки для 1,2,3:\n";
        for (auto& p : perms) {
            for (char c : p) std::cout << c;
            std::cout << " ";
        }
        std::cout << "\n";

        std::cout << "getPerm2(2): ";
        auto p2 = getPerm2(tree, 2);
        for (char c : p2) std::cout << c;
        std::cout << "\n";
    }

    const int MAX_N = 9;
    const int REPEATS = 500;

    std::ofstream out("result/data.txt");
    out << "n\tall_ms\tperm1_avg_us\tperm2_avg_us\n";

    for (int n = 1; n <= MAX_N; ++n) {
        std::vector<char> symbols(n);
        for (int i = 0; i < n; ++i) symbols[i] = static_cast<char>('1' + i);
        if (n > 9) {
            for (int i = 0; i < n; ++i) symbols[i] = static_cast<char>('a' + i);
        }

        PMTree tree(symbols);
        long long totalPerms = tree.getFactorials()[n];

        auto start = high_resolution_clock::now();
        auto allPerms = getAllPerms(tree);
        auto end = high_resolution_clock::now();
        double timeAllMs = duration_cast<milliseconds>(end - start).count();

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<long long> dist(1, totalPerms);
        long long sumTimePerm1 = 0;
        for (int rep = 0; rep < REPEATS; ++rep) {
            long long num = dist(gen);
            auto s = high_resolution_clock::now();
            auto p = getPerm1(tree, static_cast<int>(num));
            auto e = high_resolution_clock::now();
            sumTimePerm1 += duration_cast<microseconds>(e - s).count();
        }
        double avgPerm1Us = static_cast<double>(sumTimePerm1) / REPEATS;

        long long sumTimePerm2 = 0;
        for (int rep = 0; rep < REPEATS; ++rep) {
            long long num = dist(gen);
            auto s = high_resolution_clock::now();
            auto p = getPerm2(tree, static_cast<int>(num));
            auto e = high_resolution_clock::now();
            sumTimePerm2 += duration_cast<microseconds>(e - s).count();
        }
        double avgPerm2Us = static_cast<double>(sumTimePerm2) / REPEATS;

        out << n << "\t" << timeAllMs << "\t" << avgPerm1Us << "\t" << avgPerm2Us << "\n";
        std::cout << "n=" << n << " готово\n";
    }
    out.close();

    std::ofstream script("plot.py");
    script << R"(
import matplotlib.pyplot as plt
import numpy as np

data = np.loadtxt('result/data.txt', skiprows=1)
n = data[:, 0]
all_ms = data[:, 1]
perm1_us = data[:, 2]
perm2_us = data[:, 3]

plt.figure(figsize=(10,6))
plt.plot(n, all_ms, 'o-', label='getAllPerms (мс)')
plt.plot(n, perm1_us, 's-', label='getPerm1 (мкс, среднее)')
plt.plot(n, perm2_us, 'd-', label='getPerm2 (мкс, среднее)')
plt.yscale('log')
plt.xlabel('Размер алфавита n')
plt.ylabel('Время (логарифмическая шкала)')
plt.title('Зависимость времени работы от n')
plt.legend()
plt.grid(True, which='both', linestyle='--', linewidth=0.5)
plt.savefig('result/plot.png', dpi=150)
plt.close()
)";
    script.close();

    int ret = system("python plot.py");
    if (ret != 0) {
        std::cerr << "Не удалось выполнить Python-скрипт. Убедитесь, что установлен Python и matplotlib.\n";
        std::cerr << "Данные сохранены в result/data.txt. Вы можете построить график вручную.\n";
    } else {
        std::cout << "График сохранён в result/plot.png\n";
    }

    return 0;
}
