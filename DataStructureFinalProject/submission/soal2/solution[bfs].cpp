// solution_soal2_bfs.cpp — BFS on 8x8 grid with walls
// Algorithm: Breadth-First Search
// Map: 8x8, walls present (#), all passable cells cost 1
// Guarantee: Shortest path by step count

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
    while (!q.empty()) {
        Cell curr = q.front();
        q.pop();
        visited.push_back(curr);

        if (curr == goal) break;

        // SECTOR 4: CHECK NEIGHBORS - Skip out-of-bounds, walls, or already seen
        for (int i = 0; i < 4; ++i) {
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];

            if (!inBounds(nr, nc)) continue;
            if (isWall(grid[nr][nc])) continue;         // Wall check

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