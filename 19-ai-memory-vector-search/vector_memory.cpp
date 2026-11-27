/**
 * @file vector_memory.cpp
 * @brief Cosine similarity search and Top-K retrieval for AI memory embeddings in C++17.
 * Time Complexity : Insert O(1), Top-K Query O(M * D + K log M) for M memories of dim D
 * Space Complexity: O(M * D)
 */

#include <algorithm>
#include <cassert>
#include <cmath>
#include <iostream>
#include <queue>
#include <string>
#include <utility>
#include <vector>
using namespace std;

class VectorMemory {
    struct Entry {
        string id;
        vector<double> vec;
        double norm;
    };
    vector<Entry> items;

    static double computeNorm(const vector<double>& v) {
        double sumSq = 0.0;
        for (double x : v) sumSq += x * x;
        return sqrt(sumSq);
    }

public:
    void add(const string& id, const vector<double>& emb) {
        assert(!emb.empty());
        double nrm = computeNorm(emb);
        assert(nrm > 0.0);
        items.push_back(Entry{id, emb, nrm});
    }

    static double cosineSimilarity(const vector<double>& a, double normA,
                                   const vector<double>& b, double normB) {
        assert(a.size() == b.size() && normA > 0.0 && normB > 0.0);
        double dot = 0.0;
        for (size_t i = 0; i < a.size(); ++i) dot += a[i] * b[i];
        return dot / (normA * normB);
    }

    // Returns top-K {similarity, id} sorted descending by similarity
    vector<pair<double, string>> topK(const vector<double>& query, int k) const {
        assert(k >= 0 && !query.empty());
        if (k == 0 || items.empty()) return {};
        double qNorm = computeNorm(query);
        assert(qNorm > 0.0);

        using Item = pair<double, string>;
        priority_queue<Item, vector<Item>, greater<>> minHeap;
        for (const auto& entry : items) {
            if (entry.vec.size() != query.size()) continue;
            double sim = cosineSimilarity(query, qNorm, entry.vec, entry.norm);
            Item cand{sim, entry.id};
            if (static_cast<int>(minHeap.size()) < k) {
                minHeap.push(std::move(cand));
            } else if (cand.first > minHeap.top().first) {
                minHeap.pop();
                minHeap.push(std::move(cand));
            }
        }

        vector<Item> res;
        res.reserve(minHeap.size());
        while (!minHeap.empty()) {
            res.push_back(minHeap.top());
            minHeap.pop();
        }
        sort(res.begin(), res.end(), [](const Item& x, const Item& y) {
            return x.first > y.first;
        });
        return res;
    }
};

int main() {
    VectorMemory mem;
    mem.add("cpp_threads", {1.0, 0.2, 0.0});
    mem.add("sql_indexing", {0.0, 0.1, 1.0});
    mem.add("lock_free_queue", {0.9, 0.3, 0.1});

    auto top = mem.topK({1.0, 0.25, 0.0}, 2);
    assert(top.size() == 2);
    assert(top[0].second == "cpp_threads");
    assert(top[1].second == "lock_free_queue");

    cout << "[PASS] AI Memory Vector Search OK\n";
    return 0;
}
