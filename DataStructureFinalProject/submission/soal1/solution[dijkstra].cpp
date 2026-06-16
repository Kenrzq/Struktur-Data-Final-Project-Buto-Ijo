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

    map<Cell, int> dist;

    dist[start] = 0;
    came_from[start] = start;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [cost, current] = pq.top();
        pq.pop();

        if (cost > dist[current]) continue;

        visited.push_back(current);

        if (current == goal) break;

        for (int k = 0; k < 4; k++) {
            int nr = current.first + dr[k];
            int nc = current.second + dc[k];

            if (!inBounds(nr, nc)) continue;

            Cell neighbor = {nr, nc};
            int new_cost = cost + 1;

            if (!dist.count(neighbor) || new_cost < dist[neighbor]) {
                dist[neighbor] = new_cost;
                came_from[neighbor] = current;
                pq.push({new_cost, neighbor});
            }
        }
    }

    return visited;
}