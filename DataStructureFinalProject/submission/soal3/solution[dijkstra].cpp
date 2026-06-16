// solution_soal3_dijkstra.cpp — Dijkstra on 16x32 weighted grid
// Algorithm: Dijkstra's Shortest Path (min-heap priority queue)
// Map: 16x32, walls (#) and weighted cells (2, 3, 9)
// This is the CORRECT algorithm for this map. Dijkstra handles variable
// cell costs and guarantees the minimum-cost path from S to G.
//
// Key trade-off on this map:
//   Option A: Cross the row of 9s (row 7) — short path, very high cost
//   Option B: Detour through the single gap at far right of wall (row 8) — longer path, lower cost
//   Dijkstra correctly evaluates both and picks globally cheapest route.

#include "harness.h"
#include <queue>
#include <map>
#include <vector>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal) {

    // SECTOR 1: INIT - Prep tracking structures, priority queue, and movement vectors
    vector<Cell> visited;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = { 0, 0, -1, 1};

    using Entry = pair<int, Cell>;      // (cumulative_cost, cell)
    priority_queue<Entry, vector<Entry>, greater<Entry>> pq;  // min-heap: cheapest first
    map<Cell, int> best_cost;

    // SECTOR 2: START - Set initial cost zero, map parent root, seed priority queue
    best_cost[start] = 0;
    came_from[start] = start;
    pq.push({0, start});

    // SECTOR 3: DIJKSTRA LOOP - Always expand the globally cheapest known cell
    while (!pq.empty()) {
        auto [cost, current] = pq.top();
        pq.pop();

        // Stale entry guard: a cell may be pushed multiple times with different costs.
        // Only process it when we pop with the BEST (lowest) known cost.
        if (cost > best_cost[current]) continue;

        visited.push_back(current);

        if (current == goal) break;

        // SECTOR 4: CHECK NEIGHBORS - Skip out-of-bounds or walls
        for (int k = 0; k < 4; k++) {
            int nr = current.first + dr[k];
            int nc = current.second + dc[k];

            if (!inBounds(nr, nc)) continue;
            if (isWall(grid[nr][nc])) continue;

            Cell neighbor = {nr, nc};
            // cellCost returns the weight: '.' = 1, '2'= 2, '3'= 3, '9'= 9
            int new_cost = cost + cellCost(grid[nr][nc]);

            // SECTOR 5: EVALUATE & QUEUE - Update if this path is strictly cheaper
            if (!best_cost.count(neighbor) || new_cost < best_cost[neighbor]) {
                best_cost[neighbor] = new_cost;
                came_from[neighbor] = current;
                pq.push({new_cost, neighbor});
            }
        }
    }

    // SECTOR 6: RETURN - Output the complete exploration history
    return visited;
}