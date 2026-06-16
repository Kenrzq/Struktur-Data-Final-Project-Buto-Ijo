// solution_soal1_astar.cpp — A* on 6x6 open grid
// Algorithm: A* Search with Manhattan Distance heuristic
// Map: 6x6, no walls, no weights — all cells are '.' with cost 1
//
// Heuristic: h(n) = |row_n - row_goal| + |col_n - col_goal|  (Manhattan distance)
//   Admissible: min cost per cell is 1, so true cost >= Manhattan distance. Never overestimates.
//   Consistent: h(n) <= cost(n, n') + h(n') holds for all neighbors on a 4-dir grid.
//
// On this open uniform grid A* is OPTIMAL and explores the FEWEST cells of all four algorithms.
// The heuristic guides expansion directly toward G (bottom-right), so very few off-path
// cells are ever visited — a stark contrast to BFS/Dijkstra which expand in all directions.

#include "harness.h"
#include <queue>
#include <map>
#include <vector>

// Manhattan distance heuristic
// Provides a tight lower bound on remaining cost (cell cost = 1, so distance = cost)
inline int heuristic(Cell a, Cell b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal) {

    // SECTOR 1: INIT - Prep tracking structures, priority queue, and movement vectors
    vector<Cell> visited;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = { 0, 0, -1, 1};

    // Entry: (f_score, g_score, cell)
    //   f = g + h  (total estimated cost: actual so far + estimated remaining)
    //   g = actual cumulative cost to reach this cell
    //   h = heuristic estimate of remaining cost to goal
    using Entry = tuple<int, int, Cell>;
    priority_queue<Entry, vector<Entry>, greater<Entry>> pq;

    map<Cell, int> g_cost;   // Best known actual cost to each cell

    // SECTOR 2: START - Seed with start cell; g=0, f=h(start, goal)
    g_cost[start] = 0;
    came_from[start] = start;
    pq.push({heuristic(start, goal), 0, start});

    // SECTOR 3: A* LOOP - Always expand the cell with lowest f = g + h
    // On this open grid the heuristic is near-perfect, so A* walks almost straight to G
    while (!pq.empty()) {
        auto [f, g, current] = pq.top();
        pq.pop();

        // Stale entry guard: skip if a cheaper g was already found for this cell
        if (g > g_cost[current]) continue;

        visited.push_back(current);

        if (current == goal) break;

        // SECTOR 4: CHECK NEIGHBORS - Skip out-of-bounds or walls
        for (int k = 0; k < 4; k++) {
            int nr = current.first + dr[k];
            int nc = current.second + dc[k];

            if (!inBounds(nr, nc)) continue;
            if (isWall(grid[nr][nc])) continue;

            Cell neighbor = {nr, nc};
            int new_g = g + cellCost(grid[nr][nc]);     // All cells cost 1 on this map

            // SECTOR 5: EVALUATE & QUEUE - Update if cheaper g found; push with new f
            if (!g_cost.count(neighbor) || new_g < g_cost[neighbor]) {
                g_cost[neighbor] = new_g;
                came_from[neighbor] = current;
                int h = heuristic(neighbor, goal);
                pq.push({new_g + h, new_g, neighbor}); // f = g + h
            }
        }
    }

    // SECTOR 6: RETURN - Output the complete exploration history
    return visited;
}