// solution_soal2_dfs.cpp — DFS on 8x8 grid with walls
// Algorithm: Depth-First Search (iterative, using explicit stack)
// Map: 8x8, walls present (#), all passable cells cost 1
// NOTE: DFS does NOT guarantee the shortest path. It finds A path, not the best path.
// Included for comparative analysis only.

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

    // SECTOR 3: DFS LOOP - Process cells depth-first (dive into one branch before backtracking)
    while (!stk.empty()) {
        Cell curr = stk.top();
        stk.pop();
        visited.push_back(curr);

        if (curr == goal) break;

        // SECTOR 4: CHECK NEIGHBORS - Skip out-of-bounds, walls, or already seen
        // Note: Stack reverses discovery order, so we push in reverse to explore
        // in a consistent direction (up, left, right, down = natural reading order)
        for (int i = 3; i >= 0; --i) {     // Reversed loop for intuitive traversal order
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];

            if (!inBounds(nr, nc)) continue;
            if (isWall(grid[nr][nc])) continue;         // Wall check

            Cell neighbor = {nr, nc};
            if (seen.count(neighbor)) continue;

            // SECTOR 5: STACK NEIGHBOR - Mark seen immediately to prevent duplicate pushes
            seen.insert(neighbor);
            came_from[neighbor] = curr;
            stk.push(neighbor);
        }
    }

    // SECTOR 6: RETURN - Output the complete exploration history
    return visited;
}