# Pathfinding Algorithm Analysis — Data Structures Final Project

**Group:** Buto Ijo

| Name | NRP |
|:---|:---|
| Aqilah Ibrahim | 5025251260 |
| Athar Rozy Rasyidan | 5025251009 |
| Rizqi Arya Kuskhilbyano | 5025251161 |

---

## Overview

This project implements and compares four classical pathfinding algorithms — BFS, DFS, Dijkstra, and A* — across three map environments with increasing complexity. The goal is to evaluate each algorithm's correctness, path optimality, and exploration efficiency under different grid conditions.

---

## Map Problems

### Soal 1 — 6×6 Open Grid (Uniform Weights)

```
S . . . . .
. . . . . .
. . . . . .
. . . . . .
. . . . . .
. . . . . G
```

No walls. Every cell costs 1. Serves as a baseline for comparing traversal order and step count between algorithms.

---

### Soal 2 — 8×8 Grid with Walls (Uniform Weights)

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

Wall obstacles (`#`) constrain movement into narrow corridors. Tests path reconstruction correctness under restricted topology.

---

### Soal 3 — 16×32 Weighted Grid (Variable Costs + Walls)

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

**Cell costs:**

| Symbol | Cost |
|:---:|:---:|
| `.` | 1 |
| `2` | 2 |
| `3` | 3 |
| `9` | 9 |
| `#` | Impassable |

Row 7 is a near-complete barrier of cost-9 cells. Row 8 is a solid wall with a single gap at the rightmost column. The only cost-efficient route requires detouring to the far right to pass through that gap, rather than crossing the expensive row 7 cells directly.

---

## Algorithms

### BFS — Breadth-First Search

BFS uses a FIFO queue to expand nodes level by level, visiting all cells at distance `k` before any cell at distance `k+1`. This guarantees the shortest path in terms of step count on unweighted graphs.

```cpp
queue<Cell> q;
set<Cell> seen;

q.push(start);
seen.insert(start);
came_from[start] = start;

while (!q.empty()) {
    Cell curr = q.front();
    q.pop();
    visited.push_back(curr);

    if (curr == goal) break;

    for (int i = 0; i < 4; ++i) {
        if (seen.count(neighbor)) continue;
        seen.insert(neighbor);
        came_from[neighbor] = curr;
        q.push(neighbor);
    }
}
```

**Limitation on Soal 3:** BFS treats all passable cells as equal cost. It does not account for tile weights, so it will cross cost-9 cells freely instead of routing around them.

---

### DFS — Depth-First Search

DFS uses a LIFO stack to commit fully to one direction before backtracking. It explores as deep as possible along each branch before trying alternatives.

```cpp
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

    for (int i = 3; i >= 0; --i) {   // reversed to maintain consistent traversal order
        if (seen.count(neighbor)) continue;
        seen.insert(neighbor);
        came_from[neighbor] = curr;
        stk.push(neighbor);
    }
}
```

The loop iterates in reverse (`i = 3` down to `0`) because a stack reverses insertion order. Pushing in reverse ensures that the first direction (index 0) ends up on top and is explored first, keeping traversal order consistent with BFS for comparison.

**Note:** DFS does not guarantee the shortest path. Its efficiency is highly dependent on whether the map layout happens to align with its traversal direction.

---

### Dijkstra's Algorithm

Dijkstra uses a min-heap priority queue, always expanding the cell with the lowest accumulated cost from the start. This guarantees the optimal path on weighted graphs.

```cpp
using Entry = pair<int, Cell>;
priority_queue<Entry, vector<Entry>, greater<Entry>> pq;
map<Cell, int> best_cost;

best_cost[start] = 0;
came_from[start] = start;
pq.push({0, start});

while (!pq.empty()) {
    auto [cost, current] = pq.top();
    pq.pop();

    if (cost > best_cost[current]) continue;  // discard stale entries

    visited.push_back(current);
    if (current == goal) break;

    for (int k = 0; k < 4; k++) {
        int new_cost = cost + cellCost(grid[nr][nc]);

        if (!best_cost.count(neighbor) || new_cost < best_cost[neighbor]) {
            best_cost[neighbor] = new_cost;
            came_from[neighbor] = current;
            pq.push({new_cost, neighbor});
        }
    }
}
```

**Limitation:** Dijkstra expands nodes in all directions equally. On large maps, this results in a wide exploration area even when the goal is in one direction.

---

### A\* Search

A\* extends Dijkstra by incorporating a heuristic `h(n)` — an estimate of the remaining cost to the goal. Each node is prioritized by `f(n) = g(n) + h(n)`, where `g(n)` is the actual cost from the start and `h(n)` is the heuristic estimate.

```cpp
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

    if (g > g_cost[current]) continue;  // discard stale entries

    visited.push_back(current);
    if (current == goal) break;

    for (int k = 0; k < 4; k++) {
        int new_g = g + cellCost(grid[nr][nc]);

        if (!g_cost.count(neighbor) || new_g < g_cost[neighbor]) {
            g_cost[neighbor] = new_g;
            came_from[neighbor] = current;
            int h = heuristic(neighbor, goal);
            pq.push({new_g + h, new_g, neighbor});
        }
    }
}
```

The heuristic used is **Manhattan Distance**: `|row_curr − row_goal| + |col_curr − col_goal|`. This is admissible (never overestimates, since the minimum cell cost is 1), which guarantees A* produces the optimal path. Because cells closer to the goal receive lower `f` scores, A* focuses its search toward the target rather than expanding uniformly in all directions.

---

## Performance Results

| Problem | Algorithm | Visited | Total Cost | Path Length |
|:---|:---|:---:|:---:|:---:|
| **Soal 1** | **BFS ✓ BEST** | **36** | **10** | **10** |
| | DFS | 21 | 20 | 20 |
| | Dijkstra | 36 | 10 | 10 |
| | A* | 36 | 10 | 10 |
| **Soal 2** | BFS | 35 | 14 | 14 |
| | **DFS ✓ BEST** | **18** | **14** | **14** |
| | Dijkstra | 37 | 14 | 14 |
| | A* | 28 | 14 | 14 |
| **Soal 3** | BFS | 272 | 294 | 46 |
| | DFS | 149 | 322 | 144 |
| | Dijkstra | 278 | 46 | 46 |
| | **A* ✓ BEST** | **151** | **46** | **46** |

---

## How to Compile & Run

```bash
# From within the submission/soalX/ directory
g++ -O2 -std=c++17 solution[algorithm].cpp -o algorithm.exe
./algorithm ../../maps/soalx.txt output[algorithm].txt
```

To visualize: open `engine/index.html`, select the map, drag in the output `.txt` file, then click **Play / Run**.

---

## Output Format

Each algorithm writes a `.txt` file with two sections:

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

`VISITED` lists every cell evaluated, in exploration order.  
`PATH` lists the cells from `S` to `G` along the found route.

---

## Conclusion

| Algorithm | Optimal Path | Handles Weights | Efficient on Large Maps |
|:---|:---:|:---:|:---:|
| BFS | ✓ (unweighted) | ✗ | ✓ |
| DFS | ✗ | ✗ | Varies |
| Dijkstra | ✓ | ✓ | Moderate |
| A* | ✓ | ✓ | ✓ |

- **BFS** is best for unweighted, uniform-cost grids. It guarantees the shortest path with minimal overhead.
- **DFS** does not guarantee shortest paths. It can achieve low node counts in specific layouts, but this is not consistent across map types.
- **Dijkstra** guarantees optimal paths on weighted graphs but explores broadly, which increases memory usage on large maps.
- **A\*** is the most suitable algorithm for large, weighted grids with a known goal location. It matches Dijkstra's path accuracy while evaluating significantly fewer nodes through heuristic guidance.
