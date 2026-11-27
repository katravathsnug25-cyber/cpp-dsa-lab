/**
 * @file max_flow.cpp
 * @brief Edmonds-Karp Maximum Flow algorithm (BFS augmenting paths) in C++17.
 * Time Complexity : O(V * E^2)
 * Space Complexity: O(V + E)
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <limits>
#include <queue>
#include <vector>
using namespace std;

class EdmondsKarp {
    struct Edge {
        int to;
        int rev;
        long long cap;
    };
    int n;
    vector<vector<Edge>> adj;

public:
    explicit EdmondsKarp(int nodes) : n(nodes), adj(nodes) { assert(n > 0); }

    void addEdge(int u, int v, long long cap) {
        assert(u >= 0 && u < n && v >= 0 && v < n && cap >= 0);
        Edge a{v, static_cast<int>(adj[v].size()), cap};
        Edge b{u, static_cast<int>(adj[u].size()), 0};
        adj[u].push_back(a);
        adj[v].push_back(b);
    }

    long long maxFlow(int s, int t) {
        assert(s >= 0 && s < n && t >= 0 && t < n && s != t);
        long long flow = 0;
        const long long INF = numeric_limits<long long>::max();

        while (true) {
            vector<int> parentNode(n, -1), parentEdge(n, -1);
            queue<int> q;
            q.push(s);
            parentNode[s] = s;

            while (!q.empty() && parentNode[t] == -1) {
                int u = q.front();
                q.pop();
                for (int i = 0; i < static_cast<int>(adj[u].size()); ++i) {
                    const Edge& e = adj[u][i];
                    if (parentNode[e.to] == -1 && e.cap > 0) {
                        parentNode[e.to] = u;
                        parentEdge[e.to] = i;
                        q.push(e.to);
                    }
                }
            }

            if (parentNode[t] == -1) break;

            long long push = INF;
            for (int v = t; v != s; v = parentNode[v]) {
                int u = parentNode[v];
                push = min(push, adj[u][parentEdge[v]].cap);
            }
            for (int v = t; v != s; v = parentNode[v]) {
                int u = parentNode[v];
                int idx = parentEdge[v];
                adj[u][idx].cap -= push;
                int revIdx = adj[u][idx].rev;
                adj[v][revIdx].cap += push;
            }
            flow += push;
        }
        return flow;
    }
};

int main() {
    EdmondsKarp mf(6);
    mf.addEdge(0, 1, 16);
    mf.addEdge(0, 2, 13);
    mf.addEdge(1, 2, 10);
    mf.addEdge(1, 3, 12);
    mf.addEdge(2, 1, 4);
    mf.addEdge(2, 4, 14);
    mf.addEdge(3, 2, 9);
    mf.addEdge(3, 5, 20);
    mf.addEdge(4, 3, 7);
    mf.addEdge(4, 5, 4);

    assert(mf.maxFlow(0, 5) == 23);
    cout << "[PASS] Edmonds-Karp Max Flow OK\n";
    return 0;
}
