/**
 * @file binary_search.cpp
 * @brief Iterative Binary Search implementation in C++17.
 *
 * Time Complexity : O(log n)
 * Space Complexity: O(1)
 * Compile & Run   : g++ -std=c++17 -Wall -Wextra 01-arrays-strings/binary_search.cpp -o binary_search && ./binary_search
 */

#include <cassert>
#include <iostream>
#include <vector>

using namespace std;

int binarySearchIterative(const vector<int>& arr, int target) {
    int left = 0;
    int right = static_cast<int>(arr.size()) - 1;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return mid;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return -1;
}

int main() {
    vector<int> data = {2, 5, 8, 12, 16, 23, 38, 56, 72, 91};

    assert(binarySearchIterative(data, 23) == 5);
    assert(binarySearchIterative(data, 2) == 0);
    assert(binarySearchIterative(data, 91) == 9);
    assert(binarySearchIterative(data, 100) == -1);
    assert(binarySearchIterative({}, 10) == -1);

    cout << "[PASS] All Binary Search test cases verified.\n";
    cout << "Target 23 found at index: " << binarySearchIterative(data, 23) << "\n";
    return 0;
}
