/**
 * @file logic_and_mystery.cpp
 * @brief Josephus survivor puzzle and XOR odd-occurrence mystery solver in C++17.
 * Time Complexity : Josephus O(n), XOR Mystery O(n) | Space Complexity: O(1)
 */

#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

// 0-indexed Josephus problem: n people in a circle, every k-th eliminated
int josephusSurvivor(int n, int k) {
    assert(n > 0 && k > 0);
    int ans = 0;
    for (int i = 2; i <= n; ++i) {
        ans = (ans + k) % i;
    }
    return ans;
}

// Mystery: every element appears twice except one unique element
int findUniqueMystery(const vector<int>& nums) {
    int xr = 0;
    for (int x : nums) xr ^= x;
    return xr;
}

int main() {
    assert(josephusSurvivor(5, 2) == 2);
    assert(josephusSurvivor(1, 3) == 0);
    assert(findUniqueMystery({4, 1, 2, 1, 2}) == 4);
    assert(findUniqueMystery({99}) == 99);
    cout << "[PASS] Logical & Mystery OK\n";
    return 0;
}
