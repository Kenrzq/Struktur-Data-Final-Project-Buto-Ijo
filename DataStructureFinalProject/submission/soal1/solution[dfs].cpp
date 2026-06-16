// solution_soal1_dfs.cpp — DFS on 6x6 open grid
// Algorithm: Depth-First Search (iterative, using explicit stack)
// Map: 6x6, no walls, no weights — all cells are '.' with cost 1
// WARNING: DFS does NOT guarantee the shortest path. It finds A path, not the best path.
//          On this open grid DFS will tunnel to the boundary before backtracking,
//          producing a longer winding path compared to BFS or Dijkstra.
//          Included for comparative analysis only.

#include "harness.h"
#include <stack>
#include <set>
#include <vector>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal) {

    // SECTOR 1: INIT - Prep tracking list, 4-way movement vectors, stack, and seen set
    vector<Cell> visited;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = { 0, 0, -1, 1};

    stack<Cell> stk;      // DFS uses a LIFO stack instead of FIFO queue
    set<Cell> seen;

    // SECTOR 2: START - Push start cell, mark seen, set path root
    stk.push(start);
    seen.insert(start);
    came_from[start] = start;

    // SECTOR 3: DFS LOOP - Dive deep into one branch before backtracking
    // On this open 6x6 grid DFS will likely hug one edge before finding G
    while (!stk.empty()) {
        Cell curr = stk.top();
        stk.pop();
        visited.push_back(curr);

        if (curr == goal) break;

        // SECTOR 4: CHECK NEIGHBORS - Skip out-of-bounds, walls, or already seen
        // Reversed loop so stack pops in consistent order (up explored first)
        for (int i = 3; i >= 0; --i) {     // Reversed to maintain consistent priority order
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];

            if (!inBounds(nr, nc)) continue;
            if (isWall(grid[nr][nc])) continue;

            Cell neighbor = {nr, nc};
            if (seen.count(neighbor)) continue;

            // SECTOR 5: STACK NEIGHBOR - Mark seen immediately, save parent, push
            seen.insert(neighbor);
            came_from[neighbor] = curr;
            stk.push(neighbor);
        }
    }

    // SECTOR 6: RETURN - Output the complete exploration history
    return visited;
}