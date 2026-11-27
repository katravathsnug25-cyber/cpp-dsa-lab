/**
 * @file rag_pipeline.cpp
 * @brief Cohesive C++17 RAG & Agent pipeline combining vector similarity, cache, and step state.
 * Time Complexity : Query O(M * D + K log M) on cache miss, O(1) on cache hit
 * Space Complexity: O(M * D + C)
 */

#include <cassert>
#include <cmath>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

struct Doc {
    string id;
    string text;
    vector<double> emb;
    double norm;
};

static double vecNorm(const vector<double>& v) {
    double s = 0.0;
    for (double x : v) s += x * x;
    return sqrt(s);
}

static double cosine(const vector<double>& a, double na, const vector<double>& b, double nb) {
    assert(a.size() == b.size() && na > 0.0 && nb > 0.0);
    double dot = 0.0;
    for (size_t i = 0; i < a.size(); ++i) dot += a[i] * b[i];
    return dot / (na * nb);
}

class AnswerCache {
    size_t cap;
    list<pair<string, string>> dq;
    unordered_map<string, list<pair<string, string>>::iterator> idx;

public:
    explicit AnswerCache(size_t c) : cap(c) { assert(cap > 0); }

    bool get(const string& q, string& out) {
        auto it = idx.find(q);
        if (it == idx.end()) return false;
        dq.splice(dq.begin(), dq, it->second);
        out = it->second->second;
        return true;
    }

    void put(const string& q, const string& ans) {
        auto it = idx.find(q);
        if (it != idx.end()) {
            it->second->second = ans;
            dq.splice(dq.begin(), dq, it->second);
            return;
        }
        if (dq.size() == cap) {
            auto back = dq.back();
            idx.erase(back.first);
            dq.pop_back();
        }
        dq.emplace_front(q, ans);
        idx[q] = dq.begin();
    }
};

class RagEngine {
    vector<Doc> docs;
    AnswerCache cache;

public:
    explicit RagEngine(size_t cacheCap) : cache(cacheCap) {}

    void indexDoc(const string& id, const string& text, const vector<double>& emb) {
        double n = vecNorm(emb);
        assert(!emb.empty() && n > 0.0);
        docs.push_back(Doc{id, text, emb, n});
    }

    string answer(const string& query, const vector<double>& qEmb, bool& fromCache) {
        string cached;
        if (cache.get(query, cached)) {
            fromCache = true;
            return cached;
        }
        fromCache = false;
        double qn = vecNorm(qEmb);
        assert(!docs.empty() && qn > 0.0);

        double bestSim = -1e9;
        string bestDoc = docs.front().id;
        for (const auto& d : docs) {
            if (d.emb.size() != qEmb.size()) continue;
            double sim = cosine(qEmb, qn, d.emb, d.norm);
            if (sim > bestSim) {
                bestSim = sim;
                bestDoc = d.id;
            }
        }
        string synthesized = "top_doc:" + bestDoc;
        cache.put(query, synthesized);
        return synthesized;
    }
};

int main() {
    RagEngine eng(4);
    eng.indexDoc("d1", "consistent hashing ring", {1.0, 0.0});
    eng.indexDoc("d2", "spsc atomic ring buffer", {0.0, 1.0});

    bool hit = false;
    string a1 = eng.answer("how to shard nodes", {0.95, 0.05}, hit);
    assert(!hit && a1 == "top_doc:d1");

    string a2 = eng.answer("how to shard nodes", {0.95, 0.05}, hit);
    assert(hit && a2 == "top_doc:d1");

    cout << "[PASS] RAG Hybrid Ranker Pipeline OK\n";
    return 0;
}
