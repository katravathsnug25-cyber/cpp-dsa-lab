/**
 * @file nim_and_bitmask_dp.cpp
 * @brief Nim game XOR invariant and Bitmask DP (minimum Hamiltonian tour cost) in C++17.
 * Time Complexity : Nim O(N), Bitmask TSP O(N^2 * 2^N)
 * Space Complexity: Nim O(1), Bitmask TSP O(N * 2^N)
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <vector>
using namespace std;

// Returns true if first player has a guaranteed winning move in standard Nim
bool nimFirstPlayerWins(const vector<int>& piles) {
    int nimSum = 0;
    for (int stones : piles) {
        assert(stones >= 0);
        nimSum ^= stones;
    }
    return nimSum != 0;
}

// Minimum cost starting at node 0, visiting every node once, and returning to 0
int minHamiltonianCycle(const vector<vector<int>>& cost) {
    int n = static_cast<int>(cost.size());
    assert(n >= 1 && n <= 16);
    if (n == 1) return 0;

    const int INF = numeric_limits<int>::max() / 4;
    int fullMask = (1 << n) - 1;
    vector<vector<int>> dp(1 << n, vector<int>(n, INF));
    dp[1][0] = 0;

    for (int mask = 1; mask <= fullMask; ++mask) {
        if ((mask & 1) == 0) continue;
        for (int u = 0; u < n; ++u) {
            if (((mask >> u) & 1) == 0 || dp[mask][u] == INF) continue;
            for (int v = 0; v < n; ++v) {
                if (((mask >> v) & 1) != 0) continue;
                int nextMask = mask | (1 << v);
                dp[nextMask][v] = min(dp[nextMask][v], dp[mask][u] + cost[u][v]);
            }
        }
    }

    int ans = INF;
    for (int u = 1; u < n; ++u) {
        if (dp[fullMask][u] != INF) {
            ans = min(ans, dp[fullMask][u] + cost[u][0]);
        }
    }
    return ans;
}

int main() {
    assert(nimFirstPlayerWins({3, 4, 5}));
    assert(!nimFirstPlayerWins({1, 1}));

    vector<vector<int>> g = {
        {0, 10, 15, 20},
        {10, 0, 35, 25},
        {15, 35, 0, 30},
        {20, 25, 30, 0}
    };
    assert(minHamiltonianCycle(g) == 80);
    cout << "[PASS] Game Theory & Bitmask DP OK\n";
    return 0;
}
