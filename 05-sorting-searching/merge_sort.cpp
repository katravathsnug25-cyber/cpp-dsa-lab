/**
 * @file merge_sort.cpp
 * @brief Merge Sort implementation in C++17.
 * Time Complexity : O(n log n) | Space Complexity: O(n)
 */

#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

void mergeRanges(vector<int>& arr, int left, int mid, int right) {
    vector<int> temp;
    temp.reserve(static_cast<size_t>(right - left + 1));
    int i = left, j = mid + 1;

    while (i <= mid && j <= right) {
        if (arr[i] <= arr[j]) temp.push_back(arr[i++]);
        else temp.push_back(arr[j++]);
    }
    while (i <= mid) temp.push_back(arr[i++]);
    while (j <= right) temp.push_back(arr[j++]);

    for (size_t k = 0; k < temp.size(); ++k) {
        arr[left + static_cast<int>(k)] = temp[k];
    }
}

void mergeSort(vector<int>& arr, int left, int right) {
    if (left >= right) return;
    int mid = left + (right - left) / 2;
    mergeSort(arr, left, mid);
    mergeSort(arr, mid + 1, right);
    mergeRanges(arr, left, mid, right);
}

int main() {
    vector<int> arr = {38, 27, 43, 3, 9, 82, 10};
    mergeSort(arr, 0, static_cast<int>(arr.size()) - 1);
    assert(arr == vector<int>({3, 9, 10, 27, 38, 43, 82}));
    cout << "[PASS] Merge Sort OK\n";
    return 0;
}
