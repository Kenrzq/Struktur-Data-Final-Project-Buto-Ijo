// =============================================================
//  harness.h  — LOCKED SECTION. Students are NOT allowed to modify this file.
//  Tasks: read map, provide types & utils, call solve(), write output.
// =============================================================
#ifndef HARNESS_H
#define HARNESS_H

#include <vector>
#include <utility>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
using namespace std;

using Cell = pair<int,int>;          // (row, column)

// Global structure that MUST be filled by students inside solve():
//   came_from[child] = parent   -> used by the harness to reconstruct the PATH.
// Students only need to write: came_from[neighbor] = current;
map<Cell,Cell> came_from;

// ---- utils for students (can be used in solve) ----
int rows_g, cols_g;
inline bool inBounds(int r,int c){ return r>=0 && r<rows_g && c>=0 && c<cols_g; }

// cost to enter a cell. '.','S','G' = 1.  '1'..'9' = that number.  '#' = wall (unused).
inline int cellCost(char ch){ if(ch>='1'&&ch<='9') return ch-'0'; return 1; }
inline bool isWall(char ch){ return ch=='#'; }

// Declaration filled by students (see solution_*.cpp):
vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal);

// ---- internal: read map ----
static bool readMap(const string& path, vector<vector<char>>& grid, Cell& start, Cell& goal){
  ifstream f(path);
  if(!f){ cerr<<"Cannot open map: "<<path<<"\n"; return false; }
  f >> rows_g >> cols_g;
  grid.assign(rows_g, vector<char>(cols_g));
  for(int r=0;r<rows_g;r++) for(int c=0;c<cols_g;c++){
    string tok; f >> tok; char ch = tok[0];
    grid[r][c]=ch;
    if(ch=='S') start={r,c};
    if(ch=='G') goal={r,c};
  }
  return true;
}

// ---- internal: reconstruct path from came_from ----
static vector<Cell> rebuildPath(Cell start, Cell goal){
  vector<Cell> path;
  if(start==goal){ path.push_back(start); return path; }
  if(came_from.find(goal)==came_from.end()) return path; // goal unreachable
  Cell cur=goal;
  while(!(cur==start)){
    path.push_back(cur);
    auto it=came_from.find(cur);
    if(it==came_from.end()){ return {}; } // broken chain
    cur=it->second;
  }
  path.push_back(start);
  reverse(path.begin(), path.end());
  return path;
}

// ---- internal: write output ----
static void writeOutput(const string& path, const vector<Cell>& visited, const vector<Cell>& finalPath){
  ofstream f(path);
  f << "VISITED\n";
  for(auto&[r,c]:visited) f<<r<<","<<c<<"\n";
  f << "PATH\n";
  for(auto&[r,c]:finalPath) f<<r<<","<<c<<"\n";
}

// ---- locked main ----
int main(int argc, char** argv){
  string mapPath = argc>1 ? argv[1] : "map.txt";
  string outPath = argc>2 ? argv[2] : "output.txt";
  vector<vector<char>> grid; Cell start, goal;
  if(!readMap(mapPath, grid, start, goal)) return 1;

  came_from.clear();
  vector<Cell> visited = solve(grid, start, goal);   // <-- student's code
  vector<Cell> finalPath = rebuildPath(start, goal); // <-- harness

  writeOutput(outPath, visited, finalPath);
  cerr << "Visited="<<visited.size()<<"  PathLen="<<(finalPath.empty()?-1:(int)finalPath.size()-1)<<"\n";
  return 0;
}
#endif
