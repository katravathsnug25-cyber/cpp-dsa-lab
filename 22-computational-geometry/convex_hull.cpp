/**
 * @file convex_hull.cpp
 * @brief Monotone Chain 2D Convex Hull in C++17.
 * Time Complexity : O(N log N)
 * Space Complexity: O(N)
 */

#include <algorithm>
#include <cassert>
#include <iostream>
#include <vector>
using namespace std;

struct Pt {
    long long x, y;
    bool operator<(const Pt& o) const {
        return x < o.x || (x == o.x && y < o.y);
    }
    bool operator==(const Pt& o) const {
        return x == o.x && y == o.y;
    }
};

long long cross(const Pt& o, const Pt& a, const Pt& b) {
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}

vector<Pt> convexHull(vector<Pt> pts) {
    sort(pts.begin(), pts.end());
    pts.erase(unique(pts.begin(), pts.end()), pts.end());
    int n = static_cast<int>(pts.size());
    if (n <= 1) return pts;

    vector<Pt> hull;
    hull.reserve(2 * n);
    for (int i = 0; i < n; ++i) {
        while (hull.size() >= 2 && cross(hull[hull.size() - 2], hull.back(), pts[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(pts[i]);
    }
    size_t lowerSize = hull.size();
    for (int i = n - 2; i >= 0; --i) {
        while (hull.size() > lowerSize && cross(hull[hull.size() - 2], hull.back(), pts[i]) <= 0) {
            hull.pop_back();
        }
        hull.push_back(pts[i]);
    }
    hull.pop_back();
    return hull;
}

int main() {
    vector<Pt> pts = {{0, 0}, {4, 0}, {4, 4}, {0, 4}, {2, 2}, {1, 3}};
    auto h = convexHull(pts);
    assert(h.size() == 4);
    cout << "[PASS] 2D Convex Hull OK\n";
    return 0;
}
