/**
 * @file modern_cpp_essentials.cpp
 * @brief RAII, unique_ptr, structured bindings, and generic lambda in C++17.
 * Time Complexity : O(n) | Space Complexity: O(n)
 */

#include <cassert>
#include <iostream>
#include <memory>
#include <tuple>
#include <vector>

using namespace std;

pair<int, int> minMaxArray(const vector<int>& arr) {
    assert(!arr.empty());
    int mn = arr[0], mx = arr[0];
    for (int x : arr) {
        if (x < mn) mn = x;
        if (x > mx) mx = x;
    }
    return {mn, mx};
}

int main() {
    auto ptr = make_unique<vector<int>>(initializer_list<int>{14, 3, 29, 8, 19});
    auto [low, high] = minMaxArray(*ptr);

    auto span = [](const auto& a, const auto& b) { return b - a; };
    assert(low == 3);
    assert(high == 29);
    assert(span(low, high) == 26);

    cout << "[PASS] C++17 Core OK (min=" << low << ", max=" << high << ", span=" << span(low, high) << ")\n";
    return 0;
}
