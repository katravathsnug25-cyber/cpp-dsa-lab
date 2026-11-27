/**
 * @file sliding_window_limiter.cpp
 * @brief Sliding-window request rate limiter using timestamp deque in C++17.
 * Time Complexity : O(1) amortized per request check
 * Space Complexity: O(R) for up to R requests inside window
 */

#include <cassert>
#include <deque>
#include <iostream>
using namespace std;

class SlidingWindowRateLimiter {
    size_t maxRequests;
    long long windowMs;
    deque<long long> timestamps;

    void evictExpired(long long nowMs) {
        long long cutoff = nowMs - windowMs;
        while (!timestamps.empty() && timestamps.front() <= cutoff) {
            timestamps.pop_front();
        }
    }

public:
    SlidingWindowRateLimiter(size_t limit, long long windowDurationMs)
        : maxRequests(limit), windowMs(windowDurationMs) {
        assert(maxRequests > 0 && windowMs > 0);
    }

    bool allow(long long nowMs) {
        evictExpired(nowMs);
        if (timestamps.size() >= maxRequests) return false;
        timestamps.push_back(nowMs);
        return true;
    }
};

int main() {
    SlidingWindowRateLimiter rl(3, 1000);
    assert(rl.allow(100));
    assert(rl.allow(200));
    assert(rl.allow(500));
    assert(!rl.allow(900));  // 4th within 1000ms window
    assert(rl.allow(1101)); // 100ms expired at 1101
    cout << "[PASS] Sliding Window Rate Limiter OK\n";
    return 0;
}
