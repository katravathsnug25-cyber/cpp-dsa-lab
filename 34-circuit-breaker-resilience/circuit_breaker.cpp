/**
 * @file circuit_breaker.cpp
 * @brief Circuit Breaker resilience state machine (Closed, Open, HalfOpen) in C++17.
 * Time Complexity : O(1) per call/transition
 * Space Complexity: O(1)
 */

#include <cassert>
#include <iostream>
#include <string>
using namespace std;

enum class BreakerState { Closed, Open, HalfOpen };

class CircuitBreaker {
    int failureThreshold;
    long long cooldownTicks;
    int consecutiveFailures = 0;
    long long openedAtTick = 0;
    BreakerState state = BreakerState::Closed;

public:
    CircuitBreaker(int failThreshold, long long cooldown)
        : failureThreshold(failThreshold), cooldownTicks(cooldown) {
        assert(failureThreshold > 0 && cooldownTicks >= 0);
    }

    BreakerState currentState(long long nowTick) {
        if (state == BreakerState::Open && (nowTick - openedAtTick) >= cooldownTicks) {
            state = BreakerState::HalfOpen;
        }
        return state;
    }

    bool allowRequest(long long nowTick) {
        BreakerState st = currentState(nowTick);
        return st != BreakerState::Open;
    }

    void recordResult(bool success, long long nowTick) {
        BreakerState st = currentState(nowTick);
        if (success) {
            consecutiveFailures = 0;
            state = BreakerState::Closed;
            return;
        }
        ++consecutiveFailures;
        if (st == BreakerState::HalfOpen || consecutiveFailures >= failureThreshold) {
            state = BreakerState::Open;
            openedAtTick = nowTick;
        }
    }
};

int main() {
    CircuitBreaker cb(2, 10);
    assert(cb.allowRequest(1));
    cb.recordResult(false, 1);
    assert(cb.currentState(1) == BreakerState::Closed);
    cb.recordResult(false, 2);
    assert(cb.currentState(2) == BreakerState::Open);
    assert(!cb.allowRequest(5));

    assert(cb.allowRequest(12)); // cooldown (10) elapsed -> HalfOpen
    assert(cb.currentState(12) == BreakerState::HalfOpen);
    cb.recordResult(true, 12);
    assert(cb.currentState(12) == BreakerState::Closed);

    cout << "[PASS] Circuit Breaker Resilience OK\n";
    return 0;
}
