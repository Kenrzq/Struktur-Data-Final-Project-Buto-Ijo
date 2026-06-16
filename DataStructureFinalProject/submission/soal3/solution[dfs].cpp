// solution_soal3_dfs.cpp — DFS on 16x32 weighted grid
// Algorithm: Depth-First Search (iterative, using explicit stack)
// Map: 16x32, walls (#) and weighted cells (2, 3, 9)
// WARNING: DFS is SUBOPTIMAL here. It neither minimizes steps nor cost.
//          On a large grid like this, DFS may explore a very long winding path
//          before reaching the goal, and the result depends entirely on direction order.
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

    stack<Cell> stk;
    set<Cell> seen;

    // SECTOR 2: START - Push start cell, mark seen, set path root
    stk.push(start);
    seen.insert(start);
    came_from[start] = start;

    // SECTOR 3: DFS LOOP - Dive deep into one branch before exploring others
    // On a 16x32 grid DFS may explore far reaches of the map before finding goal
    while (!stk.empty()) {
        Cell curr = stk.top();
        stk.pop();
        visited.push_back(curr);

        if (curr == goal) break;

        // SECTOR 4: CHECK NEIGHBORS - Skip out-of-bounds, walls, or already seen
        // DFS ignores weights entirely — purely structural traversal
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