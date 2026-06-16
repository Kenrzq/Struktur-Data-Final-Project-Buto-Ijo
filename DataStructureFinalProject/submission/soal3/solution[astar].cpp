// solution_soal3_astar.cpp — A* on 16x32 weighted grid
// Algorithm: A* Search with Manhattan Distance heuristic
// Map: 16x32, walls (#) and weighted cells (2, 3, 9)
//
// Heuristic: h(n) = |row_n - row_goal| + |col_n - col_goal|
//   This is admissible (never overestimates true cost) because the minimum cost
//   per cell is 1, so the true cost to reach goal >= Manhattan distance.
//   It is also consistent: h(n) <= cost(n, n') + h(n') for all neighbors n'.
//
// A* is OPTIMAL and typically explores fewer cells than Dijkstra on large maps
// because the heuristic biases expansion toward the goal direction.
// On this specific map the improvement over Dijkstra may be moderate because
// the wall barrier in row 8 forces detours that the heuristic cannot anticipate.

#include "harness.h"
#include <queue>
#include <map>
#include <vector>

// Manhattan distance heuristic
// Provides a lower bound: min possible cost = Manhattan distance × min_cell_cost (= 1)
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

    // SECTOR 2: START - Seed with start cell; g=0, f=h(start)
    g_cost[start] = 0;
    came_from[start] = start;
    pq.push({heuristic(start, goal), 0, start});

    // SECTOR 3: A* LOOP - Always expand the cell with lowest f = g + h
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
            int new_g = g + cellCost(grid[nr][nc]);     // Actual cost including cell weight

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