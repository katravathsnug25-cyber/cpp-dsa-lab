/**
 * @file n_queens.cpp
 * @brief N-Queens solution counter using bitmask constraint pruning in C++17.
 * Time Complexity : O(N!) upper bound with bitmask pruning
 * Space Complexity: O(N) recursion stack
 */

#include <cassert>
#include <iostream>
using namespace std;

void solveNQueens(int row, int n, unsigned int cols, unsigned int diag1, unsigned int diag2, long long& ways) {
    if (row == n) {
        ++ways;
        return;
    }
    unsigned int fullMask = (1U << n) - 1U;
    unsigned int avail = fullMask & ~(cols | diag1 | diag2);
    while (avail != 0U) {
        unsigned int bit = avail & -avail;
        avail ^= bit;
        solveNQueens(
            row + 1,
            n,
            cols | bit,
            ((diag1 | bit) << 1) & fullMask,
            (diag2 | bit) >> 1,
            ways
        );
    }
}

long long countNQueens(int n) {
    assert(n >= 1 && n <= 14);
    long long ways = 0;
    solveNQueens(0, n, 0U, 0U, 0U, ways);
    return ways;
}

int main() {
    assert(countNQueens(1) == 1);
    assert(countNQueens(4) == 2);
    assert(countNQueens(8) == 92);
    cout << "[PASS] N-Queens Bitmask OK\n";
    return 0;
}
