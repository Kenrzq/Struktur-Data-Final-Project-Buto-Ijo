# Struktur-Data-Final-Project-Buto-Ijo

|    Nama     |           NRP             |
| :--------: |       :------------:       |
| Aqilah Ibrahim | 5025251260             |
| Athar Rozy Rasyidan | 5025251009        |
| Rizqi Arya Kuskhilbyano | 5025251161    |

---

## Grid Problems

1. 6×6 Open Grid
```
S . . . . .
. . . . . .
. . . . . .
. . . . . .
. . . . . .
. . . . . G
```

No walls, no weights. Every cell costs 1. A clean baseline to compare algorithm traversal order.

2. 8×8 Grid with Walls
```
S . . # . . . .
. # . # # # . .
. # . . . # . .
. # # # . # . #
. . . # . . . .
# # . # # # # .
. . . . . . . .
. # # # # # . G
```

Walls (#) force the algorithms to navigate around obstacles. Tests path reconstruction correctness under constrained topology.

3. 16×32 Weighted Grid
```
S . . . . . 2 . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . # . . . . . . . . . . 3 . . . . . . . . . . . . . . . .
. . # . . . . . . . . 2 . . . . . . . # . . . . . . . . . . . .
. . . . . . . . . # . . . . . . . . . # . . . . . . . . . . . .
. . . . . . . . . . . . . # . . . . . . . . . . . . 2 . . . . .
. . . . . . . 2 . . . . . . . . . . . . . # . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 9 .
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . .
. . . . . . . 2 . . . . . . . . . . . # . . . . . . . . . . . .
. . . . . . . . . . . # . . . . . 3 . . . . . . . . . . . . . .
. . # . . . . . . . . . . . . . . . . . . . . # . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . 2 . . .
. . . . . 2 . . . . . . . # . . . . . . . . . . . . . . . . . .
. . . . . . . . . . . . . . . . . . . . . . . . . . . . . . . G
```

Cell weights: 
2 = cost 2,
3 = cost 3,
9 = cost 9,
. = cost 1,
'#' = impassable wall

The design for this map where there are rows of cost 9 cells makes crossing the shortest past more expensive in cost. The only free cost detour is through the single gap at right side of the grid.

---
# Algorithms

## BFS (Breadth First Search)
Explore everything at distance 1, then distance 2, then distance 3... expanding outward like ripples in water.
```
queue<Cell> q;      // FIFO — first in, first out
set<Cell> seen;

q.push(start);
seen.insert(start);
came_from[start] = start;

while (!q.empty()) {
    Cell curr = q.front();   // take from FRONT
    q.pop();
    visited.push_back(curr);

    if (curr == goal) break;

    for (int i = 0; i < 4; ++i) {
        // check up/down/left/right neighbors
        if (seen.count(neighbor)) continue;

        seen.insert(neighbor);
        came_from[neighbor] = curr;
        q.push(neighbor);    // add to BACK
    }
}
```
The queue is what makes BFS work. When you add neighbors to the back and read from the front, you naturally process all cells at the current "ring" before moving to the next one. This guarantees that the first time you reach the goal, it's via the fewest steps.

Why it fails on Soal 3: BFS treats every passable cell as equal cost. It doesn't know that 9 costs 9, it just sees "passable" and crosses row 7 freely.

## DFS (Depth First Search)
Commit fully to one direction, go as deep as possible, only backtrack when stuck.
```
stack<Cell> stk;    // LIFO — last in, first out
set<Cell> seen;

stk.push(start);
seen.insert(start);
came_from[start] = start;

while (!stk.empty()) {
    Cell curr = stk.top();   // take from TOP
    stk.pop();
    visited.push_back(curr);

    if (curr == goal) break;

    for (int i = 3; i >= 0; --i) {   // reversed loop
        if (seen.count(neighbor)) continue;

        seen.insert(neighbor);
        came_from[neighbor] = curr;
        stk.push(neighbor);  // add to TOP
    }
}
```
The only difference from BFS is stack instead of queue and reading from top instead of front. That single swap completely changes behavior: instead of spreading outward evenly, it dives deep down one path before exploring anything else.
The loop runs i = 3 down to 0 (reversed) because a stack reverses insertion order. Pushing in reverse means the first direction (index 0 = up) ends up on top and gets explored first — keeping traversal order consistent and predictable.
The trade-off you saw in the results: On Soal 1, DFS visited only 21 cells (vs. BFS's 36) but produced a 21-step path (vs. BFS's 11). It wastes almost no time exploring — but finds a terrible path.

--- 

## Dijkstra
Always expand whichever cell has the lowest total accumulated cost so far, not whichever was added earliest.
```
using Entry = pair<int, Cell>;   // (cost, cell)
priority_queue<Entry, vector<Entry>, greater<Entry>> pq;  // min-heap
map<Cell, int> best_cost;

best_cost[start] = 0;
came_from[start] = start;
pq.push({0, start});

while (!pq.empty()) {
    auto [cost, current] = pq.top();
    pq.pop();

    if (cost > best_cost[current]) continue;  // stale entry guard

    visited.push_back(current);
    if (current == goal) break;

    for (int k = 0; k < 4; k++) {
        int new_cost = cost + cellCost(grid[nr][nc]);  // ADD THE CELL'S WEIGHT

        if (!best_cost.count(neighbor) || new_cost < best_cost[neighbor]) {
            best_cost[neighbor] = new_cost;
            came_from[neighbor] = current;
            pq.push({new_cost, neighbor});
        }
    }
}
```



--- 

## A* Search
Dijkstra, but with a hint. Instead of only tracking cost-so-far (g), A* also estimates cost-to-go (h) and prioritizes cells by their total estimated cost f = g + h.
```
inline int heuristic(Cell a, Cell b) {
    return abs(a.first - b.first) + abs(a.second - b.second);  // Manhattan distance
}

using Entry = tuple<int, int, Cell>;   // (f, g, cell)
priority_queue<Entry, vector<Entry>, greater<Entry>> pq;
map<Cell, int> g_cost;

g_cost[start] = 0;
pq.push({heuristic(start, goal), 0, start});

while (!pq.empty()) {
    auto [f, g, current] = pq.top();
    pq.pop();

    if (g > g_cost[current]) continue;  // stale entry guard

    visited.push_back(current);
    if (current == goal) break;

    for (int k = 0; k < 4; k++) {
        int new_g = g + cellCost(grid[nr][nc]);

        if (!g_cost.count(neighbor) || new_g < g_cost[neighbor]) {
            g_cost[neighbor] = new_g;
            came_from[neighbor] = current;
            int h = heuristic(neighbor, goal);
            pq.push({new_g + h, new_g, neighbor});   // f = g + h
        }
    }
}
```

The heuristic is Manhattan distance: |row_current - row_goal| + |col_current - col_goal|. It answers "if there were no walls and no weights, how far am I from the goal?" This is admissible (never overestimates, since minimum cell cost is 1) which is what guarantees A* still finds the optimal path.
In practice, cells closer to the goal get lower f scores and are explored first, A* naturally "steers" toward the destination rather than expanding equally in all directions like Dijkstra.

---

## How to Compile & Run
```
# From within the submission/soalX/ directory
g++ -O2 -std=c++17 solution[algorithm].cpp -o algorithm.exe
./algorithm ../../maps/soalx.txt output[algorithm].txt
open engine/index.html, select the question/map, drag output.txt, click Play/Run.
```

## Output Format 
```
VISITED
row,col
row,col
...
PATH
row,col
row,col
...
```

VISITED lists every cell explored (in order). 
PATH lists the cells from S to G in the found route.

---



















