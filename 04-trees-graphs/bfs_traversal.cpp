/**
 * @file bfs_traversal.cpp
 * @brief Breadth-First Search (BFS) on a graph in C++17.
 * Time Complexity : O(V + E) | Space Complexity: O(V)
 */

#include <cassert>
#include <iostream>
#include <queue>
#include <vector>
using namespace std;

vector<int> bfsOrder(int n, const vector<vector<int>>& adj, int start) {
    vector<int> order;
    if (start < 0 || start >= n) return order;

    vector<bool> visited(n, false);
    queue<int> q;
    visited[start] = true;
    q.push(start);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        order.push_back(u);
        for (int v : adj[u]) {
            if (!visited[v]) {
                visited[v] = true;
                q.push(v);
            }
        }
    }
    return order;
}

int main() {
    int n = 5;
    vector<vector<int>> adj(n);
    adj[0] = {1, 2};
    adj[1] = {0, 3};
    adj[2] = {0, 4};
    adj[3] = {1};
    adj[4] = {2};

    assert(bfsOrder(n, adj, 0) == vector<int>({0, 1, 2, 3, 4}));
    cout << "[PASS] BFS Graph OK\n";
    return 0;
}
