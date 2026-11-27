/**
 * @file lfu_cache.cpp
 * @brief O(1) Least Frequently Used (LFU) cache with LRU tie-breaking in C++17.
 * Time Complexity : Get O(1), Put O(1)
 * Space Complexity: O(C) for capacity C
 */

#include <cassert>
#include <iostream>
#include <list>
#include <unordered_map>
#include <utility>
using namespace std;

class LfuCache {
    struct Entry {
        int val;
        int freq;
        list<int>::iterator pos;
    };

    size_t cap;
    int minFreq = 0;
    unordered_map<int, Entry> keyMap;
    unordered_map<int, list<int>> freqLists;

    void touch(int key) {
        auto it = keyMap.find(key);
        assert(it != keyMap.end());
        int f = it->second.freq;
        auto& oldList = freqLists[f];
        oldList.erase(it->second.pos);
        if (oldList.empty()) {
            freqLists.erase(f);
            if (minFreq == f) ++minFreq;
        }
        int nf = f + 1;
        auto& newList = freqLists[nf];
        newList.push_front(key);
        it->second.freq = nf;
        it->second.pos = newList.begin();
    }

public:
    explicit LfuCache(size_t capacity) : cap(capacity) { assert(cap > 0); }

    bool get(int key, int& outVal) {
        auto it = keyMap.find(key);
        if (it == keyMap.end()) return false;
        outVal = it->second.val;
        touch(key);
        return true;
    }

    void put(int key, int val) {
        auto it = keyMap.find(key);
        if (it != keyMap.end()) {
            it->second.val = val;
            touch(key);
            return;
        }
        if (keyMap.size() == cap) {
            auto& evictList = freqLists[minFreq];
            int evictKey = evictList.back();
            evictList.pop_back();
            if (evictList.empty()) freqLists.erase(minFreq);
            keyMap.erase(evictKey);
        }
        minFreq = 1;
        auto& f1 = freqLists[1];
        f1.push_front(key);
        keyMap[key] = Entry{val, 1, f1.begin()};
    }
};

int main() {
    LfuCache c(2);
    c.put(1, 10);
    c.put(2, 20);
    int v = 0;
    assert(c.get(1, v) && v == 10); // key 1 freq=2, key 2 freq=1
    c.put(3, 30);                   // evicts key 2
    assert(!c.get(2, v));
    assert(c.get(3, v) && v == 30);
    cout << "[PASS] LFU Cache Engine OK\n";
    return 0;
}
