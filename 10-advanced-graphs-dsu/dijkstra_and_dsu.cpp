/**
 * @file dijkstra_and_dsu.cpp
 * @brief Dijkstra's shortest path and Disjoint Set Union (Union-Find) in C++17.
 * Time Complexity : Dijkstra O((V + E) log V), DSU O(alpha(N)) amortized
 * Space Complexity: O(V + E)
 */

#include <cassert>
#include <iostream>
#include <limits>
#include <numeric>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

class DSU {
    vector<int> parent, rnk;
public:
    explicit DSU(int n) : parent(n), rnk(n, 0) {
        iota(parent.begin(), parent.end(), 0);
    }
    int find(int x) {
        if (parent[x] != x) parent[x] = find(parent[x]);
        return parent[x];
    }
    bool unite(int a, int b) {
        a = find(a);
        b = find(b);
        if (a == b) return false;
        if (rnk[a] < rnk[b]) swap(a, b);
        parent[b] = a;
        if (rnk[a] == rnk[b]) ++rnk[a];
        return true;
    }
};

vector<int> dijkstra(int n, const vector<vector<pair<int, int>>>& adj, int src) {
    const int INF = numeric_limits<int>::max();
    vector<int> dist(n, INF);
    priority_queue<pair<int, int>, vector<pair<int, int>>, greater<>> pq;

    dist[src] = 0;
    pq.push({0, src});

    while (!pq.empty()) {
        auto [d, u] = pq.top();
        pq.pop();
        if (d > dist[u]) continue;
        for (auto [v, w] : adj[u]) {
            if (dist[u] != INF && dist[u] + w < dist[v]) {
                dist[v] = dist[u] + w;
                pq.push({dist[v], v});
            }
        }
    }
    return dist;
}

int main() {
    DSU dsu(5);
    assert(dsu.unite(0, 1));
    assert(dsu.unite(1, 2));
    assert(!dsu.unite(0, 2));
    assert(dsu.find(0) != dsu.find(3));

    vector<vector<pair<int, int>>> adj(4);
    adj[0] = {{1, 4}, {2, 1}};
    adj[2] = {{1, 2}, {3, 5}};
    adj[1] = {{3, 1}};

    assert(dijkstra(4, adj, 0) == vector<int>({0, 3, 1, 4}));
    cout << "[PASS] Dijkstra & DSU OK\n";
    return 0;
}
