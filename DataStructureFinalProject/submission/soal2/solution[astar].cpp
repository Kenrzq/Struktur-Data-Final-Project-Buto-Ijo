// solution_soal2_astar.cpp — A* on 8x8 grid with walls
// Algorithm: A* Search with Manhattan Distance heuristic
// Map: 8x8, walls present (#), all passable cells cost 1
// Heuristic: h(n) = |row_n - row_goal| + |col_n - col_goal|  (Manhattan distance)
// The heuristic is ADMISSIBLE (never overestimates) and CONSISTENT on this 4-directional grid,
// guaranteeing A* finds the optimal path while exploring fewer cells than BFS or Dijkstra.

#include "harness.h"
#include <queue>
#include <map>
#include <vector>

// Manhattan distance heuristic: lower bound on steps remaining to goal
// Admissible because on a grid with cost >= 1, the true distance >= Manhattan distance
inline int heuristic(Cell a, Cell b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal) {

    // SECTOR 1: INIT - Prep tracking structures, priority queue, and movement vectors
    vector<Cell> visited;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = { 0, 0, -1, 1};

    // Priority queue entry: (f_score, g_score, cell)
    // f = g + h  where g = cost so far, h = heuristic estimate to goal
    // Using f as primary sort key, g as tiebreaker (prefer lower g when f is equal)
    using Entry = tuple<int, int, Cell>;    // (f, g, cell)
    priority_queue<Entry, vector<Entry>, greater<Entry>> pq;

    map<Cell, int> g_cost;   // Best known cost to reach each cell

    // SECTOR 2: START - Set initial costs, map parent root, seed priority queue
    g_cost[start] = 0;
    came_from[start] = start;
    int h_start = heuristic(start, goal);
    pq.push({h_start, 0, start});     // f = 0 + h, g = 0

    // SECTOR 3: A* LOOP - Process cell with lowest f = g + h score
    while (!pq.empty()) {
        auto [f, g, current] = pq.top();
        pq.pop();

        // Stale entry guard: skip if we already found a cheaper path
        if (g > g_cost[current]) continue;

        visited.push_back(current);

        if (current == goal) break;

        // SECTOR 4: CHECK NEIGHBORS - Skip out-of-bounds or walls
        for (int k = 0; k < 4; k++) {
            int nr = current.first + dr[k];
            int nc = current.second + dc[k];

            if (!inBounds(nr, nc)) continue;
            if (isWall(grid[nr][nc])) continue;         // Wall check

            Cell neighbor = {nr, nc};
            int new_g = g + cellCost(grid[nr][nc]);     // g(neighbor) = g(current) + step cost

            // SECTOR 5: EVALUATE & QUEUE - If this path to neighbor is cheaper, update
            if (!g_cost.count(neighbor) || new_g < g_cost[neighbor]) {
                g_cost[neighbor] = new_g;
                came_from[neighbor] = current;
                int h = heuristic(neighbor, goal);
                int new_f = new_g + h;                  // f = g + h
                pq.push({new_f, new_g, neighbor});
            }
        }
    }

    // SECTOR 6: RETURN - Output the complete exploration history
    return visited;
}