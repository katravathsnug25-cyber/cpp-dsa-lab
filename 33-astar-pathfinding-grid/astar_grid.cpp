/**
 * @file astar_grid.cpp
 * @brief A* shortest path algorithm on a 2D obstacle grid in C++17.
 * Time Complexity : O(R * C log(R * C)) worst-case
 * Space Complexity: O(R * C)
 */

#include <cassert>
#include <cstdlib>
#include <iostream>
#include <queue>
#include <utility>
#include <vector>
using namespace std;

struct AStarNode {
    int f, g, r, c;
    bool operator>(const AStarNode& o) const {
        if (f != o.f) return f > o.f;
        return g > o.g;
    }
};

// Returns shortest step count from start to goal on 0=free, 1=blocked grid, or -1 if unreachable.
int astarShortestPath(const vector<vector<int>>& grid,
                      pair<int, int> start,
                      pair<int, int> goal) {
    int rows = static_cast<int>(grid.size());
    assert(rows > 0 && !grid[0].empty());
    int cols = static_cast<int>(grid[0].size());

    auto inBounds = [&](int r, int c) {
        return r >= 0 && r < rows && c >= 0 && c < cols && grid[r][c] == 0;
    };
    assert(inBounds(start.first, start.second) && inBounds(goal.first, goal.second));

    if (start == goal) return 0;

    auto heuristic = [&](int r, int c) {
        return abs(r - goal.first) + abs(c - goal.second);
    };

    const int INF = 1e9;
    vector<vector<int>> dist(rows, vector<int>(cols, INF));
    priority_queue<AStarNode, vector<AStarNode>, greater<AStarNode>> pq;

    dist[start.first][start.second] = 0;
    pq.push(AStarNode{heuristic(start.first, start.second), 0, start.first, start.second});

    const int dr[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    while (!pq.empty()) {
        AStarNode cur = pq.top();
        pq.pop();
        if (cur.g > dist[cur.r][cur.c]) continue;
        if (cur.r == goal.first && cur.c == goal.second) return cur.g;

        for (int k = 0; k < 4; ++k) {
            int nr = cur.r + dr[k];
            int nc = cur.c + dc[k];
            if (!inBounds(nr, nc)) continue;
            int ng = cur.g + 1;
            if (ng < dist[nr][nc]) {
                dist[nr][nc] = ng;
                pq.push(AStarNode{ng + heuristic(nr, nc), ng, nr, nc});
            }
        }
    }
    return -1;
}

int main() {
    vector<vector<int>> g = {
        {0, 0, 0, 0},
        {1, 1, 0, 1},
        {0, 0, 0, 0},
        {0, 1, 1, 0}
    };
    assert(astarShortestPath(g, {0, 0}, {3, 3}) == 6);

    vector<vector<int>> blocked = {
        {0, 1},
        {1, 0}
    };
    assert(astarShortestPath(blocked, {0, 0}, {1, 1}) == -1);

    cout << "[PASS] A* Grid Pathfinding OK\n";
    return 0;
}
