// solution.cpp — THIS IS WHAT YOU WILL WORK ON.
//
// Write your algorithm ONLY inside the solve() function below.
// Do not add other functions. Do not modify harness.h.
//
// Compile: g++ -O2 -std=c++17 solution.cpp -o solution
// Run:     ./solution ../maps/soal1.txt output.txt
// View:    open engine/index.html, select the question/map, drag output.txt, click Play/Run.


#include "harness.h"
#include <queue>
#include <set>
#include <vector>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal){
  vector<Cell> visited; 

  // PERBAIKAN: Hanya gunakan 4 arah gerakan agar sinkron dengan visualisator grid
  int dr[] = {-1, 1, 0, 0};
  int dc[] = { 0, 0,-1, 1};

  queue<Cell> q;
  set<Cell> seen;

  q.push(start);
  seen.insert(start);

  while(!q.empty()) {
    Cell cur = q.front();
    q.pop();

    visited.push_back(cur);

    if (cur == goal) {
      break;
    }

    for(int i = 0; i < 4; ++i) {
      int nr = cur.first + dr[i];
      int nc = cur.second + dc[i];
      Cell neighbor = {nr, nc};

      if (inBounds(nr, nc) && !isWall(grid[nr][nc])) {
        if (seen.find(neighbor) == seen.end()) {
          seen.insert(neighbor);      
          came_from[neighbor] = cur; // Diperlukan harness untuk merekonstruksi PATH
          q.push(neighbor);           
        }
      }
    }
  }

  return visited;
}