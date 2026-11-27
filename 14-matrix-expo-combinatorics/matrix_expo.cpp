/**
 * @file matrix_expo.cpp
 * @brief Binary Matrix Exponentiation for O(k^3 log N) linear recurrences in C++17.
 * Time Complexity : O(k^3 log N)
 * Space Complexity: O(k^2)
 */

#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

using Matrix = vector<vector<long long>>;

Matrix mulMat(const Matrix& A, const Matrix& B, long long mod) {
    int n = static_cast<int>(A.size());
    int m = static_cast<int>(B[0].size());
    int p = static_cast<int>(B.size());
    Matrix C(n, vector<long long>(m, 0));
    for (int i = 0; i < n; ++i) {
        for (int k = 0; k < p; ++k) {
            if (A[i][k] == 0) continue;
            long long aik = A[i][k] % mod;
            for (int j = 0; j < m; ++j) {
                C[i][j] = (C[i][j] + aik * (B[k][j] % mod)) % mod;
            }
        }
    }
    return C;
}

Matrix matPow(Matrix base, long long exp, long long mod) {
    int n = static_cast<int>(base.size());
    Matrix res(n, vector<long long>(n, 0));
    for (int i = 0; i < n; ++i) res[i][i] = 1 % mod;

    while (exp > 0) {
        if (exp & 1LL) res = mulMat(res, base, mod);
        base = mulMat(base, base, mod);
        exp >>= 1LL;
    }
    return res;
}

long long fibFast(long long n, long long mod = 1000000007LL) {
    assert(n >= 0);
    if (n == 0) return 0;
    if (n == 1) return 1 % mod;
    Matrix T = {{1, 1}, {1, 0}};
    Matrix Tn = matPow(T, n - 1, mod);
    return Tn[0][0] % mod;
}

int main() {
    assert(fibFast(0) == 0);
    assert(fibFast(1) == 1);
    assert(fibFast(10) == 55);
    assert(fibFast(50) == 12586269025LL % 1000000007LL);
    cout << "[PASS] Matrix Exponentiation OK\n";
    return 0;
}
