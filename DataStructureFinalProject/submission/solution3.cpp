// solution_soal3.cpp
// Map: 16x32, HAS weights (2,3,9) AND walls (#)
// Algorithm: Dijkstra — BFS upgraded to handle cell costs
//
// The problem with BFS on a weighted map:
//   BFS treats every cell the same, so it may pick a path through
//   many expensive cells (9s) when a longer but cheaper route exists.
//
// What changes from soal2 → soal3:
//   1. queue<Cell>        -->  priority_queue ordered by COST (cheapest first)
//   2. set<Cell> seen     -->  map<Cell,int> best_cost  (tracks cheapest cost found)
//   3. push neighbor      -->  only push if we found a CHEAPER route to it
//   4. cellCost()         -->  used to add the actual cost of entering a cell
//
// Dijkstra guarantees the CHEAPEST cost path (not just fewest steps).
//
// Compile: g++ -O2 -std=c++17 solution_soal3.cpp -o sol3
// Run:     ./sol3 ../maps/soal3.txt output3.txt

#include "harness.h"
#include <queue>
#include <map>

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal) {

    vector<Cell> visited;

    int dr[] = {-1, 1,  0, 0};
    int dc[] = { 0, 0, -1, 1};

    // *** CHANGE 1: priority queue instead of plain queue ***
    // Each entry is {cost_so_far, cell}
    // The queue always gives us the CHEAPEST unprocessed cell first
    using Entry = pair<int, Cell>;
    priority_queue<Entry, vector<Entry>, greater<Entry>> pq;

    // *** CHANGE 2: track cheapest known cost to reach each cell ***
    map<Cell, int> best_cost;

    best_cost[start]   = 0;
    came_from[start]   = start;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [cost, current] = pq.top();
        pq.pop();

        // If we already processed this cell with a cheaper cost, skip
        // (stale entry — a better route was found after this was queued)
        if (cost > best_cost[current]) continue;

        visited.push_back(current);

        if (current == goal) break;

        for (int k = 0; k < 4; k++) {
            int nr = current.first  + dr[k];
            int nc = current.second + dc[k];

            if (!inBounds(nr, nc))    continue; // outside grid, skip
            if (isWall(grid[nr][nc])) continue; // wall, skip

            Cell neighbor  = {nr, nc};

            // *** CHANGE 3: calculate the actual cost to enter this neighbor ***
            int new_cost = cost + cellCost(grid[nr][nc]); // cellCost: '.'=1, '2'=2, '9'=9, etc.

            // *** CHANGE 4: only queue neighbor if this is cheaper than before ***
            if (!best_cost.count(neighbor) || new_cost < best_cost[neighbor]) {
                best_cost[neighbor] = new_cost;
                came_from[neighbor] = current; // remember how we got here
                pq.push({new_cost, neighbor});
            }
        }
    }

    return visited;
}