// solution_soal3_bfs.cpp — BFS on 16x32 weighted grid
// Algorithm: Breadth-First Search
// Map: 16x32, walls (#) and weighted cells (2, 3, 9)
// WARNING: BFS is SUBOPTIMAL here. It finds shortest path by STEPS, not by COST.
//          It may cross the row of 9s (cheap in steps, very expensive in cost).
//          Included for comparative analysis to demonstrate why Dijkstra is needed.

#include "harness.h"
#include <queue>
#include <set>
#include <vector>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal) {

    // SECTOR 1: INIT - Prep tracking list, 4-way movement vectors, queue, and seen set
    vector<Cell> visited;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = { 0, 0, -1, 1};

    queue<Cell> q;
    set<Cell> seen;

    // SECTOR 2: START - Queue start cell, mark seen, set path root
    q.push(start);
    seen.insert(start);
    came_from[start] = start;

    // SECTOR 3: BFS LOOP - Process cells level-by-level (ignores cell cost weights)
    while (!q.empty()) {
        Cell curr = q.front();
        q.pop();
        visited.push_back(curr);

        if (curr == goal) break;

        // SECTOR 4: CHECK NEIGHBORS - Skip out-of-bounds, walls, or already seen
        // BFS does NOT consider cellCost — treats all passable cells equally
        for (int i = 0; i < 4; ++i) {
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];

            if (!inBounds(nr, nc)) continue;
            if (isWall(grid[nr][nc])) continue;

            Cell neighbor = {nr, nc};
            if (seen.count(neighbor)) continue;

            // SECTOR 5: QUEUE NEIGHBOR - Mark seen, save parent, push to queue
            seen.insert(neighbor);
            came_from[neighbor] = curr;
            q.push(neighbor);
        }
    }

    // SECTOR 6: RETURN - Output the complete exploration history
    return visited;
}