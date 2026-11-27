/**
 * @file skip_list.cpp
 * @brief Probabilistic Skip List ordered set in C++17.
 * Time Complexity : Insert/Erase/Contains O(log N) expected
 * Space Complexity: O(N) expected
 */

#include <cassert>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

class SkipList {
    static constexpr int MAX_LEVEL = 16;

    struct Node {
        int key;
        vector<Node*> forward;
        Node(int k, int lvl) : key(k), forward(lvl, nullptr) {}
    };

    Node* head;
    int level = 1;
    mt19937 rng{12345u};

    int randomLevel() {
        int lvl = 1;
        while (lvl < MAX_LEVEL && (rng() & 1u) == 0u) ++lvl;
        return lvl;
    }

public:
    SkipList() : head(new Node(-1, MAX_LEVEL)) {}

    ~SkipList() {
        Node* cur = head;
        while (cur) {
            Node* nxt = cur->forward[0];
            delete cur;
            cur = nxt;
        }
    }

    bool contains(int key) const {
        Node* cur = head;
        for (int i = level - 1; i >= 0; --i) {
            while (cur->forward[i] && cur->forward[i]->key < key) {
                cur = cur->forward[i];
            }
        }
        cur = cur->forward[0];
        return cur != nullptr && cur->key == key;
    }

    void insert(int key) {
        vector<Node*> update(MAX_LEVEL, head);
        Node* cur = head;
        for (int i = level - 1; i >= 0; --i) {
            while (cur->forward[i] && cur->forward[i]->key < key) {
                cur = cur->forward[i];
            }
            update[i] = cur;
        }
        cur = cur->forward[0];
        if (cur && cur->key == key) return;

        int newLvl = randomLevel();
        if (newLvl > level) {
            for (int i = level; i < newLvl; ++i) update[i] = head;
            level = newLvl;
        }
        Node* node = new Node(key, newLvl);
        for (int i = 0; i < newLvl; ++i) {
            node->forward[i] = update[i]->forward[i];
            update[i]->forward[i] = node;
        }
    }

    bool erase(int key) {
        vector<Node*> update(MAX_LEVEL, nullptr);
        Node* cur = head;
        for (int i = level - 1; i >= 0; --i) {
            while (cur->forward[i] && cur->forward[i]->key < key) {
                cur = cur->forward[i];
            }
            update[i] = cur;
        }
        cur = cur->forward[0];
        if (!cur || cur->key != key) return false;

        for (int i = 0; i < level; ++i) {
            if (update[i]->forward[i] != cur) break;
            update[i]->forward[i] = cur->forward[i];
        }
        delete cur;
        while (level > 1 && head->forward[level - 1] == nullptr) --level;
        return true;
    }
};

int main() {
    SkipList sl;
    for (int x : {15, 3, 25, 10, 7}) sl.insert(x);
    assert(sl.contains(10));
    assert(!sl.contains(99));
    assert(sl.erase(10));
    assert(!sl.contains(10));
    assert(!sl.erase(10));
    cout << "[PASS] Skip List Map OK\n";
    return 0;
}
