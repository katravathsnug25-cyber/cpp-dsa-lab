/**
 * @file order_statistic_tree.cpp
 * @brief Order Statistic Tree (Treap-based randomized BST with subtree sizes) in C++17.
 * Time Complexity : Insert/Erase/Kth/Rank O(log N) expected
 * Space Complexity: O(N)
 */

#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>
using namespace std;

class OrderStatisticTree {
    struct Node {
        int key;
        uint64_t prio;
        int sz;
        Node* left;
        Node* right;
        Node(int k, uint64_t p) : key(k), prio(p), sz(1), left(nullptr), right(nullptr) {}
    };

    Node* root = nullptr;
    mt19937_64 rng{42ULL};

    static int getSize(Node* n) { return n ? n->sz : 0; }
    static void pull(Node* n) {
        if (n) n->sz = 1 + getSize(n->left) + getSize(n->right);
    }

    // Split nodes with key < target into L, and >= target into R
    void split(Node* cur, int target, Node*& L, Node*& R) {
        if (!cur) {
            L = R = nullptr;
            return;
        }
        if (cur->key < target) {
            split(cur->right, target, cur->right, R);
            L = cur;
        } else {
            split(cur->left, target, L, cur->left);
            R = cur;
        }
        pull(cur);
    }

    Node* merge(Node* L, Node* R) {
        if (!L || !R) return L ? L : R;
        if (L->prio > R->prio) {
            L->right = merge(L->right, R);
            pull(L);
            return L;
        }
        R->left = merge(L, R->left);
        pull(R);
        return R;
    }

    bool containsNode(Node* cur, int key) const {
        while (cur) {
            if (key == cur->key) return true;
            cur = (key < cur->key) ? cur->left : cur->right;
        }
        return false;
    }

    void freeTree(Node* cur) {
        if (!cur) return;
        freeTree(cur->left);
        freeTree(cur->right);
        delete cur;
    }

public:
    ~OrderStatisticTree() { freeTree(root); }

    void insert(int key) {
        if (containsNode(root, key)) return;
        Node *L = nullptr, *R = nullptr;
        split(root, key, L, R);
        Node* item = new Node(key, rng());
        root = merge(merge(L, item), R);
    }

    void erase(int key) {
        Node *L = nullptr, *mid = nullptr, *R = nullptr;
        split(root, key, L, R);
        split(R, key + 1, mid, R);
        freeTree(mid);
        root = merge(L, R);
    }

    // 0-indexed kth smallest element
    int kthSmallest(int k) const {
        assert(k >= 0 && k < getSize(root));
        Node* cur = root;
        while (cur) {
            int lsz = getSize(cur->left);
            if (k < lsz) {
                cur = cur->left;
            } else if (k == lsz) {
                return cur->key;
            } else {
                k -= (lsz + 1);
                cur = cur->right;
            }
        }
        assert(false);
        return -1;
    }

    // Count of elements strictly less than key
    int orderOfKey(int key) const {
        int cnt = 0;
        Node* cur = root;
        while (cur) {
            if (key <= cur->key) {
                cur = cur->left;
            } else {
                cnt += 1 + getSize(cur->left);
                cur = cur->right;
            }
        }
        return cnt;
    }

    int size() const { return getSize(root); }
};

int main() {
    OrderStatisticTree ost;
    for (int x : {50, 20, 70, 10, 30, 60, 80}) ost.insert(x);
    assert(ost.size() == 7);
    assert(ost.kthSmallest(0) == 10);
    assert(ost.kthSmallest(3) == 50);
    assert(ost.orderOfKey(60) == 4);
    ost.erase(50);
    assert(ost.size() == 6);
    assert(ost.kthSmallest(3) == 60);
    cout << "[PASS] Order Statistic Tree OK\n";
    return 0;
}
