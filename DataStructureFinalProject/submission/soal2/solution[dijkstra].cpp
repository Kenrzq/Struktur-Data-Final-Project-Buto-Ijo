// solution_soal2_dijkstra.cpp — Dijkstra on 8x8 grid with walls
// Algorithm: Dijkstra's Shortest Path
// Map: 8x8, walls present (#), all passable cells cost 1
// NOTE: On uniform-cost maps, Dijkstra produces the same result as BFS
// but with higher overhead. Included for direct algorithm comparison.

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
    priority_queue<Entry, vector<Entry>, greater<Entry>> pq;  // min-heap
    map<Cell, int> best_cost;

    // SECTOR 2: START - Set initial cost, map parent root, and seed priority queue
    best_cost[start] = 0;
    came_from[start] = start;
    pq.push({0, start});

    // SECTOR 3: DIJKSTRA LOOP - Always process the cheapest known cell next
    while (!pq.empty()) {
        auto [cost, current] = pq.top();
        pq.pop();

        // Stale entry guard: skip if we already found a cheaper path to this cell
        if (cost > best_cost[current]) continue;

        visited.push_back(current);

        if (current == goal) break;

        // SECTOR 4: CHECK NEIGHBORS - Skip out-of-bounds or walls
        for (int k = 0; k < 4; k++) {
            int nr = current.first + dr[k];
            int nc = current.second + dc[k];

            if (!inBounds(nr, nc)) continue;
            if (isWall(grid[nr][nc])) continue;         // Wall check

            Cell neighbor = {nr, nc};
            int new_cost = cost + cellCost(grid[nr][nc]);  // All costs = 1 on this map

            // SECTOR 5: EVALUATE COST & QUEUE - Update if this path is cheaper
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