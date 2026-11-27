/**
 * @file agent_cloud_primitives.cpp
 * @brief LRU Cache, Token Bucket Rate Limiter, and Top-K Scheduler in C++17.
 * Time Complexity : LRU O(1) avg, RateLimiter O(1), Top-K O(N log K)
 * Space Complexity: LRU O(C), RateLimiter O(1), Top-K O(K)
 */

#include <cassert>
#include <iostream>
#include <list>
#include <queue>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>
using namespace std;

class LRUCache {
    size_t cap;
    list<pair<string, string>> items; // front = most recent
    unordered_map<string, list<pair<string, string>>::iterator> pos;

public:
    explicit LRUCache(size_t capacity) : cap(capacity) { assert(cap > 0); }

    bool get(const string& key, string& outVal) {
        auto it = pos.find(key);
        if (it == pos.end()) return false;
        items.splice(items.begin(), items, it->second);
        outVal = it->second->second;
        return true;
    }

    void put(const string& key, const string& val) {
        auto it = pos.find(key);
        if (it != pos.end()) {
            it->second->second = val;
            items.splice(items.begin(), items, it->second);
            return;
        }
        if (items.size() == cap) {
            auto last = items.back();
            pos.erase(last.first);
            items.pop_back();
        }
        items.emplace_front(key, val);
        pos[key] = items.begin();
    }
};

class TokenBucket {
    double capacity;
    double tokens;
    double refillPerSec;

public:
    TokenBucket(double cap, double refillRate)
        : capacity(cap), tokens(cap), refillPerSec(refillRate) {
        assert(capacity > 0.0 && refillPerSec >= 0.0);
    }

    bool allow(double cost = 1.0, double elapsedSec = 0.0) {
        if (elapsedSec > 0.0) {
            tokens = min(capacity, tokens + elapsedSec * refillPerSec);
        }
        if (tokens + 1e-12 < cost) return false;
        tokens -= cost;
        return true;
    }
};

vector<pair<int, string>> topKTasks(const vector<pair<int, string>>& tasks, int k) {
    assert(k >= 0);
    if (k == 0 || tasks.empty()) return {};
    priority_queue<pair<int, string>, vector<pair<int, string>>, greater<>> pq;
    for (const auto& t : tasks) {
        if (static_cast<int>(pq.size()) < k) {
            pq.push(t);
        } else if (t.first > pq.top().first) {
            pq.pop();
            pq.push(t);
        }
    }
    vector<pair<int, string>> res;
    res.reserve(pq.size());
    while (!pq.empty()) {
        res.push_back(pq.top());
        pq.pop();
    }
    return res;
}

int main() {
    LRUCache cache(2);
    cache.put("prompt:1", "ok");
    cache.put("prompt:2", "cached");
    string val;
    assert(cache.get("prompt:1", val) && val == "ok");
    cache.put("prompt:3", "new"); // evicts "prompt:2"
    assert(!cache.get("prompt:2", val));

    TokenBucket limiter(2.0, 1.0);
    assert(limiter.allow(1.0, 0.0));
    assert(limiter.allow(1.0, 0.0));
    assert(!limiter.allow(1.0, 0.0));
    assert(limiter.allow(1.0, 1.0));

    auto top2 = topKTasks({{10, "search"}, {50, "code_exec"}, {30, "rag_rank"}}, 2);
    assert(top2.size() == 2);
    cout << "[PASS] AI & Cloud Primitives OK\n";
    return 0;
}
