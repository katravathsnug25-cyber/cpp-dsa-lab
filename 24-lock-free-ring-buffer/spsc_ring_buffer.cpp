/**
 * @file spsc_ring_buffer.cpp
 * @brief Lock-free Single-Producer Single-Consumer (SPSC) Ring Buffer in C++17.
 * Time Complexity : Push O(1), Pop O(1)
 * Space Complexity: O(N)
 */

#include <atomic>
#include <cassert>
#include <iostream>
#include <thread>
#include <vector>
using namespace std;

template <typename T>
class SpscRingBuffer {
    size_t cap; // usable slots = cap - 1
    vector<T> buf;
    atomic<size_t> head{0};
    atomic<size_t> tail{0};

    size_t nextIdx(size_t idx) const { return (idx + 1) % cap; }

public:
    explicit SpscRingBuffer(size_t capacity) : cap(capacity + 1), buf(capacity + 1) {
        assert(capacity > 0);
    }

    bool push(const T& item) {
        size_t h = head.load(memory_order_relaxed);
        size_t nxt = nextIdx(h);
        if (nxt == tail.load(memory_order_acquire)) return false;
        buf[h] = item;
        head.store(nxt, memory_order_release);
        return true;
    }

    bool pop(T& out) {
        size_t t = tail.load(memory_order_relaxed);
        if (t == head.load(memory_order_acquire)) return false;
        out = buf[t];
        tail.store(nextIdx(t), memory_order_release);
        return true;
    }
};

int main() {
    const int N = 1000;
    SpscRingBuffer<int> rb(64);
    long long sum = 0;

    thread consumer([&]() {
        int received = 0, val = 0;
        while (received < N) {
            if (rb.pop(val)) {
                sum += val;
                ++received;
            }
        }
    });

    for (int i = 1; i <= N; ++i) {
        while (!rb.push(i)) {
        }
    }
    consumer.join();

    long long expected = static_cast<long long>(N) * (N + 1) / 2;
    assert(sum == expected);
    cout << "[PASS] Lock-Free SPSC Ring Buffer OK\n";
    return 0;
}
