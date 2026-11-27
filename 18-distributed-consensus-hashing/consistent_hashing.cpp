/**
 * @file consistent_hashing.cpp
 * @brief Consistent Hashing ring with virtual nodes for distributed routing in C++17.
 * Time Complexity : Lookup O(log(N * V)), Add/Remove Node O(V log(N * V))
 * Space Complexity: O(N * V) for N physical nodes and V virtual replicas
 */

#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>
#include <map>
#include <string>
using namespace std;

class ConsistentHashRing {
    int replicas;
    map<size_t, string> ring;

    static size_t hashKey(const string& key) {
        return hash<string>{}(key);
    }

public:
    explicit ConsistentHashRing(int virtualReplicas = 16) : replicas(virtualReplicas) {
        assert(replicas > 0);
    }

    void addNode(const string& node) {
        assert(!node.empty());
        for (int i = 0; i < replicas; ++i) {
            ring[hashKey(node + "#" + to_string(i))] = node;
        }
    }

    void removeNode(const string& node) {
        for (int i = 0; i < replicas; ++i) {
            ring.erase(hashKey(node + "#" + to_string(i)));
        }
    }

    string getNode(const string& key) const {
        assert(!ring.empty());
        size_t h = hashKey(key);
        auto it = ring.lower_bound(h);
        if (it == ring.end()) return ring.begin()->second;
        return it->second;
    }

    bool empty() const { return ring.empty(); }
};

int main() {
    ConsistentHashRing ch(32);
    ch.addNode("node-a");
    ch.addNode("node-b");

    string assigned = ch.getNode("session:999");
    assert(!assigned.empty());

    ch.removeNode("node-a");
    assert(ch.getNode("session:999") == "node-b");

    cout << "[PASS] Consistent Hashing OK\n";
    return 0;
}
