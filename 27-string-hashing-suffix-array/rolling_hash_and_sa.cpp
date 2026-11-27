/**
 * @file rolling_hash_and_sa.cpp
 * @brief Rolling string hashing (O(1) substring query) and Suffix Array construction in C++17.
 * Time Complexity : Hash build O(N), Substring hash O(1), Suffix Array O(N log^2 N)
 * Space Complexity: O(N)
 */

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class RollingHasher {
    static constexpr uint64_t MOD = 1000000007ULL;
    static constexpr uint64_t BASE = 131ULL;
    vector<uint64_t> pref, powBase;

public:
    explicit RollingHasher(const string& s) {
        size_t n = s.size();
        pref.assign(n + 1, 0ULL);
        powBase.assign(n + 1, 1ULL);
        for (size_t i = 0; i < n; ++i) {
            uint64_t ch = static_cast<unsigned char>(s[i]) + 1ULL;
            pref[i + 1] = (pref[i] * BASE + ch) % MOD;
            powBase[i + 1] = (powBase[i] * BASE) % MOD;
        }
    }

    // Hash of s[l .. r) where 0 <= l <= r <= s.size()
    uint64_t substrHash(size_t l, size_t r) const {
        assert(l <= r && r + 1 < powBase.size());
        uint64_t raw = (pref[r] + MOD - ((pref[l] * powBase[r - l]) % MOD)) % MOD;
        return raw;
    }
};

vector<int> buildSuffixArray(const string& s) {
    int n = static_cast<int>(s.size());
    vector<int> sa(n), rank(n), tmp(n);
    for (int i = 0; i < n; ++i) {
        sa[i] = i;
        rank[i] = static_cast<unsigned char>(s[i]);
    }
    for (int k = 1; k < n; k <<= 1) {
        auto cmp = [&](int a, int b) {
            if (rank[a] != rank[b]) return rank[a] < rank[b];
            int ra = (a + k < n) ? rank[a + k] : -1;
            int rb = (b + k < n) ? rank[b + k] : -1;
            return ra < rb;
        };
        sort(sa.begin(), sa.end(), cmp);
        tmp[sa[0]] = 0;
        for (int i = 1; i < n; ++i) {
            tmp[sa[i]] = tmp[sa[i - 1]] + (cmp(sa[i - 1], sa[i]) ? 1 : 0);
        }
        rank = tmp;
        if (rank[sa[n - 1]] == n - 1) break;
    }
    return sa;
}

int main() {
    string s = "banana";
    RollingHasher rh(s);
    assert(rh.substrHash(1, 3) == rh.substrHash(3, 5)); // "an" == "an"
    assert(rh.substrHash(0, 3) != rh.substrHash(1, 4)); // "ban" != "ana"

    auto sa = buildSuffixArray(s);
    assert(static_cast<int>(sa.size()) == 6);
    assert(sa[0] == 5); // "a" is lexicographically smallest suffix
    cout << "[PASS] Rolling Hash & Suffix Array OK\n";
    return 0;
}
