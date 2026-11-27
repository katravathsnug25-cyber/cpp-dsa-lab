/**
 * @file dp_classics.cpp
 * @brief Space-optimized 0/1 Knapsack and Coin Change (min coins) in C++17.
 * Time Complexity : Knapsack O(n * W), Coin Change O(amount * n)
 * Space Complexity: Knapsack O(W), Coin Change O(amount)
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

int knapsack01(int capacity, const vector<int>& wt, const vector<int>& val) {
    assert(wt.size() == val.size() && capacity >= 0);
    vector<int> dp(capacity + 1, 0);
    for (size_t i = 0; i < wt.size(); ++i) {
        for (int w = capacity; w >= wt[i]; --w) {
            dp[w] = max(dp[w], dp[w - wt[i]] + val[i]);
        }
    }
    return dp[capacity];
}

int coinChangeMin(const vector<int>& coins, int amount) {
    assert(amount >= 0);
    const int INF = amount + 1;
    vector<int> dp(amount + 1, INF);
    dp[0] = 0;
    for (int a = 1; a <= amount; ++a) {
        for (int c : coins) {
            if (c > 0 && a >= c && dp[a - c] != INF) {
                dp[a] = min(dp[a], dp[a - c] + 1);
            }
        }
    }
    return dp[amount] == INF ? -1 : dp[amount];
}

int main() {
    assert(knapsack01(50, {10, 20, 30}, {60, 100, 120}) == 220);
    assert(knapsack01(0, {10}, {60}) == 0);
    assert(coinChangeMin({1, 2, 5}, 11) == 3);
    assert(coinChangeMin({2}, 3) == -1);
    assert(coinChangeMin({1, 5}, 0) == 0);
    cout << "[PASS] Dynamic Programming OK\n";
    return 0;
}
