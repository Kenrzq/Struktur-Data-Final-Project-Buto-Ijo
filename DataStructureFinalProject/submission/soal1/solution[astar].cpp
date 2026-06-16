#include "harness.h"
#include <queue>
#include <map>
#include <vector>

inline int heuristic(Cell a, Cell b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal) {

    vector<Cell> visited;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = { 0, 0, -1, 1};

    using Entry = tuple<int, int, Cell>;
    priority_queue<Entry, vector<Entry>, greater<Entry>> pq;

    map<Cell, int> g_cost;

    g_cost[start] = 0;
    came_from[start] = start;
    pq.push({heuristic(start, goal), 0, start});

    while (!pq.empty()) {
        auto [f, g, current] = pq.top();
        pq.pop();

        if (g > g_cost[current]) continue;

        visited.push_back(current);

        if (current == goal) break;

        for (int k = 0; k < 4; k++) {
            int nr = current.first + dr[k];
            int nc = current.second + dc[k];

            if (!inBounds(nr, nc)) continue;

            Cell neighbor = {nr, nc};
            int new_g = g + cellCost(grid[nr][nc]);

            if (!g_cost.count(neighbor) || new_g < g_cost[neighbor]) {
                g_cost[neighbor] = new_g;
                came_from[neighbor] = current;
                int h = heuristic(neighbor, goal);
                pq.push({new_g + h, new_g, neighbor});
            }
        }
    }
    return visited;
}
