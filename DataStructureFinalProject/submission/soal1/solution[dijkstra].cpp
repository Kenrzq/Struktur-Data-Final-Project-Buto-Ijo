// solution_soal1_dijkstra.cpp — Dijkstra on 6x6 open grid
// Algorithm: Dijkstra's Shortest Path (min-heap priority queue)
// Map: 6x6, no walls, no weights — all cells are '.' with cost 1
// NOTE: On this uniform-cost map Dijkstra produces the same result as BFS
//       but with higher overhead due to the priority queue.
//       Included for direct algorithm comparison across all three maps.

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
    // On a uniform grid this behaves identically to BFS, expanding in cost rings
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
            // cellCost returns 1 for all '.' cells on this map
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