/**
 * @file range_queries.cpp
 * @brief Fenwick Tree (Binary Indexed Tree) for O(log N) point update & range sum in C++17.
 * Time Complexity : Build O(N), Point Update O(log N), Range Query O(log N)
 * Space Complexity: O(N)
 */

#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

class FenwickTree {
    int n;
    vector<long long> bit;

public:
    explicit FenwickTree(int n) : n(n), bit(n + 1, 0) {}

    explicit FenwickTree(const vector<int>& arr) : n(static_cast<int>(arr.size())), bit(n + 1, 0) {
        for (int i = 0; i < n; ++i) {
            add(i, arr[i]);
        }
    }

    // 0-indexed point add: arr[idx] += delta
    void add(int idx, long long delta) {
        assert(idx >= 0 && idx < n);
        for (int i = idx + 1; i <= n; i += i & -i) {
            bit[i] += delta;
        }
    }

    // Prefix sum [0..idx] inclusive (0-indexed)
    long long prefixSum(int idx) const {
        if (idx < 0) return 0;
        assert(idx < n);
        long long sum = 0;
        for (int i = idx + 1; i > 0; i -= i & -i) {
            sum += bit[i];
        }
        return sum;
    }

    // Range sum [l..r] inclusive (0-indexed)
    long long rangeSum(int l, int r) const {
        assert(l >= 0 && r < n && l <= r);
        return prefixSum(r) - prefixSum(l - 1);
    }
};

int main() {
    FenwickTree ft({2, 4, 5, 7, 8});
    assert(ft.rangeSum(0, 2) == 11);
    assert(ft.rangeSum(1, 3) == 16);

    ft.add(2, 5); // index 2 becomes 10
    assert(ft.rangeSum(1, 3) == 21);
    assert(ft.prefixSum(4) == 31);

    cout << "[PASS] Fenwick Tree OK\n";
    return 0;
}
