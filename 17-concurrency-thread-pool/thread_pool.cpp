/**
 * @file thread_pool.cpp
 * @brief Fixed-size C++17 Thread Pool with task queue and graceful shutdown.
 * Time Complexity : Enqueue O(1) amortized, Worker dispatch O(1)
 * Space Complexity: O(T + W) for T queued tasks and W worker threads
 */

#include <atomic>
#include <cassert>
#include <condition_variable>
#include <functional>
#include <iostream>
#include <mutex>
#include <queue>
#include <thread>
#include <utility>
#include <vector>
using namespace std;

class ThreadPool {
    vector<thread> workers;
    queue<function<void()>> tasks;
    mutex mtx;
    condition_variable cv;
    atomic<bool> stop{false};

public:
    explicit ThreadPool(size_t numThreads) {
        assert(numThreads > 0);
        workers.reserve(numThreads);
        for (size_t i = 0; i < numThreads; ++i) {
            workers.emplace_back([this]() {
                while (true) {
                    function<void()> job;
                    {
                        unique_lock<mutex> lock(mtx);
                        cv.wait(lock, [this]() { return stop.load() || !tasks.empty(); });
                        if (stop.load() && tasks.empty()) return;
                        job = std::move(tasks.front());
                        tasks.pop();
                    }
                    job();
                }
            });
        }
    }

    void enqueue(function<void()> task) {
        {
            lock_guard<mutex> lock(mtx);
            assert(!stop.load());
            tasks.push(std::move(task));
        }
        cv.notify_one();
    }

    ~ThreadPool() {
        stop.store(true);
        cv.notify_all();
        for (thread& w : workers) {
            if (w.joinable()) w.join();
        }
    }
};

int main() {
    atomic<int> counter{0};
    {
        ThreadPool pool(4);
        for (int i = 0; i < 20; ++i) {
            pool.enqueue([&counter]() {
                counter.fetch_add(1, memory_order_relaxed);
            });
        }
    }
    assert(counter.load() == 20);
    cout << "[PASS] C++17 ThreadPool OK\n";
    return 0;
}
