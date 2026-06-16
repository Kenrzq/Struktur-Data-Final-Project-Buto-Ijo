// solution_soal1_bfs.cpp — BFS on 6x6 open grid
// Algorithm: Breadth-First Search
// Map: 6x6, no walls, no weights — all cells are '.' with cost 1
// Guarantee: Finds the shortest path by STEP COUNT (= shortest cost on uniform grid)
// On this map BFS is OPTIMAL because all cell costs are equal (1).
// It expands outward in rings, so the first time it reaches G is guaranteed shortest.

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

    // SECTOR 3: BFS LOOP - Process cells level-by-level; stop if goal reached
    // On this open 6x6 grid BFS expands in concentric diamond rings from S
    while (!q.empty()) {
        Cell curr = q.front();
        q.pop();
        visited.push_back(curr);

        if (curr == goal) break;

        // SECTOR 4: CHECK NEIGHBORS - Skip out-of-bounds, walls, or already seen
        // No walls on this map, but inBounds and isWall checks remain for correctness
        for (int i = 0; i < 4; ++i) {
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];

            if (!inBounds(nr, nc)) continue;
            if (isWall(grid[nr][nc])) continue;

            Cell neighbor = {nr, nc};
            if (seen.count(neighbor)) continue;

            // SECTOR 5: QUEUE NEIGHBOR - Mark seen, save parent link, push to queue
            seen.insert(neighbor);
            came_from[neighbor] = curr;
            q.push(neighbor);
        }
    }

    // SECTOR 6: RETURN - Output the complete exploration history
    return visited;
}