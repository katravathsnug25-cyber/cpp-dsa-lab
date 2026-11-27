/**
 * @file bloom_filter.cpp
 * @brief Bloom Filter probabilistic membership structure in C++17.
 * Time Complexity : Insert O(K), Query O(K) for K hash functions
 * Space Complexity: O(M) bits
 */

#include <cassert>
#include <cstdint>
#include <functional>
#include <iostream>
#include <string>
#include <vector>
using namespace std;

class BloomFilter {
    size_t numBits;
    int numHashes;
    vector<uint64_t> bits;

    uint64_t hashAt(const string& item, int idx) const {
        uint64_t h1 = static_cast<uint64_t>(hash<string>{}(item));
        uint64_t h2 = static_cast<uint64_t>(hash<string>{}(to_string(idx) + "#" + item));
        uint64_t mixed = h1 + static_cast<uint64_t>(idx) * (h2 | 1ULL);
        return mixed % numBits;
    }

public:
    BloomFilter(size_t mBits, int kHashes)
        : numBits(mBits), numHashes(kHashes), bits((mBits + 63) / 64, 0ULL) {
        assert(numBits > 0 && numHashes > 0);
    }

    void add(const string& item) {
        for (int i = 0; i < numHashes; ++i) {
            uint64_t pos = hashAt(item, i);
            bits[pos / 64] |= (1ULL << (pos % 64));
        }
    }

    bool possiblyContains(const string& item) const {
        for (int i = 0; i < numHashes; ++i) {
            uint64_t pos = hashAt(item, i);
            if ((bits[pos / 64] & (1ULL << (pos % 64))) == 0ULL) return false;
        }
        return true;
    }
};

int main() {
    BloomFilter bf(1024, 3);
    bf.add("mastermax");
    bf.add("nit-silchar");
    assert(bf.possiblyContains("mastermax"));
    assert(bf.possiblyContains("nit-silchar"));
    cout << "[PASS] Bloom Filter OK\n";
    return 0;
}
