/**
 * @file parallel_mapreduce.cpp
 * @brief Multi-threaded MapReduce word-count / score aggregation in C++17.
 * Time Complexity : O(N / T + M log M) for N items, T threads, M unique keys
 * Space Complexity: O(M + T)
 */

#include <cassert>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>
#include <vector>
using namespace std;

map<string, long long> parallelCount(const vector<string>& items, size_t numThreads) {
    assert(numThreads > 0);
    if (items.empty()) return {};

    size_t actualThreads = min(numThreads, items.size());
    vector<map<string, long long>> partials(actualThreads);
    vector<thread> workers;
    workers.reserve(actualThreads);

    size_t chunk = (items.size() + actualThreads - 1) / actualThreads;
    for (size_t t = 0; t < actualThreads; ++t) {
        size_t begin = t * chunk;
        size_t end = min(items.size(), begin + chunk);
        workers.emplace_back([&, t, begin, end]() {
            for (size_t i = begin; i < end; ++i) {
                partials[t][items[i]] += 1;
            }
        });
    }
    for (thread& w : workers) {
        if (w.joinable()) w.join();
    }

    map<string, long long> reduced;
    for (const auto& pm : partials) {
        for (const auto& kv : pm) {
            reduced[kv.first] += kv.second;
        }
    }
    return reduced;
}

int main() {
    vector<string> tokens = {"cpp", "ai", "cpp", "rust", "ai", "cpp", "go", "ai"};
    auto res = parallelCount(tokens, 3);
    assert(res["cpp"] == 3);
    assert(res["ai"] == 3);
    assert(res["rust"] == 1);
    assert(res["go"] == 1);
    cout << "[PASS] Parallel MapReduce OK\n";
    return 0;
}
