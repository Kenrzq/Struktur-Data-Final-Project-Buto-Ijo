#include "harness.h"
#include <queue>
#include <set>
#include <vector>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal){

    // SECTOR 1: INIT - Prep tracking list, 8-way movement vectors, queue, and seen set
    vector<Cell> visited; 
    int dr[] = {-1, 1, 0, 0, -1, -1,  1, 1};
    int dc[] = { 0, 0,-1, 1, -1,  1, -1, 1};
    queue<Cell> q;
    set<Cell> seen;

    // SECTOR 2: START - Queue start cell and mark it as seen immediately
    q.push(start);
    seen.insert(start);

    // SECTOR 3: BFS LOOP - Process cells level-by-level; stop if goal reached
    while(!q.empty()) {
        Cell curr = q.front();
        q.pop();
        visited.push_back(curr);

        if (curr == goal) {
            break;
        }

        // SECTOR 4: CHECK NEIGHBORS - Look 8 ways; verify bounds and ensure it's not a solid wall
        for(int i = 0; i < 8; ++i) {
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];
            Cell neighbor = {nr, nc};

            if (inBounds(nr, nc) && !isWall(grid[nr][nc])) {
                
                // SECTOR 5: QUEUE NEIGHBOR - If unseen, mark seen, save parent link, push to queue
                if (seen.find(neighbor) == seen.end()) {
                    seen.insert(neighbor);      
                    came_from[neighbor] = curr; 
                    q.push(neighbor);           
                }
            }
        }
    }

    // SECTOR 6: RETURN - Output the complete exploration history
    return visited;
}