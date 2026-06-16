#include "harness.h"
#include <stack>
#include <set>
#include <vector>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal) {

    vector<Cell> visited;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = { 0, 0, -1, 1};

    stack<Cell> stk;
    set<Cell> seen;

    stk.push(start);
    seen.insert(start);
    came_from[start] = start;

    while (!stk.empty()) {
        Cell curr = stk.top();
        stk.pop();
        visited.push_back(curr);

        if (curr == goal) break;

        for (int i = 3; i >= 0; --i) {
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];

            if (!inBounds(nr, nc)) continue;
            if (isWall(grid[nr][nc])) continue;

            Cell neighbor = {nr, nc};
            if (seen.count(neighbor)) continue;

            seen.insert(neighbor);
            came_from[neighbor] = curr;
            stk.push(neighbor);
        }
    }

    int total_cost = 0;
    Cell cur = goal;
    while (!(cur == start)) {
    total_cost += cellCost(grid[cur.first][cur.second]);
    cur = came_from[cur];}
    
    total_cost += cellCost(grid[start.first][start.second]);
    cerr << "TotalCost=" << total_cost << "\n";

    return visited;
}
