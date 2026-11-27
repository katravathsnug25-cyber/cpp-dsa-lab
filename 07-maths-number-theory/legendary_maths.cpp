/**
 * @file legendary_maths.cpp
 * @brief Modular exponentiation, Euclidean GCD/LCM, and Sieve of Eratosthenes in C++17.
 * Time Complexity : ModExp O(log exp), GCD O(log min(a,b)), Sieve O(n log log n)
 */

#include <cassert>
#include <iostream>
#include <numeric>
#include <vector>
using namespace std;

long long modPow(long long base, long long exp, long long mod) {
    long long res = 1 % mod;
    base %= mod;
    while (exp > 0) {
        if (exp & 1LL) res = (__int128)res * base % mod;
        base = (__int128)base * base % mod;
        exp >>= 1LL;
    }
    return res;
}

vector<int> sievePrimes(int n) {
    if (n < 2) return {};
    vector<bool> isPrime(n + 1, true);
    isPrime[0] = isPrime[1] = false;
    for (int i = 2; 1LL * i * i <= n; ++i) {
        if (isPrime[i]) {
            for (int j = i * i; j <= n; j += i) isPrime[j] = false;
        }
    }
    vector<int> primes;
    for (int i = 2; i <= n; ++i) {
        if (isPrime[i]) primes.push_back(i);
    }
    return primes;
}

int main() {
    const long long MOD = 1000000007LL;
    assert(modPow(2, 10, MOD) == 1024);
    assert(gcd(48, 18) == 6);
    assert(lcm(12, 18) == 36);
    assert(sievePrimes(15) == vector<int>({2, 3, 5, 7, 11, 13}));
    cout << "[PASS] Legendary Maths OK\n";
    return 0;
}
