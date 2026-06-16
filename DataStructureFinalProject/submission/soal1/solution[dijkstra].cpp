#include "harness.h"
#include <queue>
#include <map>
#include <vector>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal) {

    vector<Cell> visited;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = { 0, 0, -1, 1};

    using Entry = pair<int, Cell>;
    priority_queue<Entry, vector<Entry>, greater<Entry>> pq;
    map<Cell, int> best_cost;

    best_cost[start] = 0;
    came_from[start] = start;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [cost, current] = pq.top();
        pq.pop();

        if (cost > best_cost[current]) continue;

        visited.push_back(current);

        if (current == goal) break;

        for (int k = 0; k < 4; k++) {
            int nr = current.first + dr[k];
            int nc = current.second + dc[k];

            if (!inBounds(nr, nc)) continue;

            Cell neighbor = {nr, nc};

            int new_cost = cost + cellCost(grid[nr][nc]);

            if (!best_cost.count(neighbor) || new_cost < best_cost[neighbor]) {
                best_cost[neighbor] = new_cost;
                came_from[neighbor] = current;
                pq.push({new_cost, neighbor});
            }
        }
    }

    return visited;
}
