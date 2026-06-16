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

Each algorithm is implemented as a `solve()` function in its own `.cpp` file. A shared `harness.h` handles map parsing, path reconstruction via the global `came_from` map, and output writing.

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

Row 7 is a near-complete barrier of cost-9 cells. Row 8 is a solid wall with a single gap at the rightmost column. The only cost-efficient route requires detouring to the far right to pass through that gap, rather than crossing the expensive row-7 cells directly.

---

## Algorithms

### BFS — Breadth-First Search

BFS uses a FIFO queue to expand nodes level by level, visiting all cells at distance `k` before any cell at distance `k+1`. This guarantees the fewest steps to the goal on uniform-cost graphs.

The implementation is identical across Soal 1, 2, and 3. Soal 2 and 3 add an `isWall` check; Soal 1 omits it since there are no walls.

```cpp
// solution[bfs].cpp (soal2 / soal3 variant shown — soal1 does not have isWall check yet)
vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal) {

    vector<Cell> visited;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = { 0, 0, -1, 1};

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
            int nr = curr.first + dr[i];
            int nc = curr.second + dc[i];

            if (!inBounds(nr, nc)) continue;
            if (isWall(grid[nr][nc])) continue;

            Cell neighbor = {nr, nc};
            if (seen.count(neighbor)) continue;

            seen.insert(neighbor);
            came_from[neighbor] = curr;
            q.push(neighbor);
        }
    }

    return visited;
}
```

**Limitation on Soal 3:** BFS treats all passable cells as equal regardless of weight. It does not call `cellCost`, so it crosses cost-9 cells freely instead of routing around them. The path found is the shortest in steps, not in cost.

---

### DFS — Depth-First Search

DFS uses a LIFO stack to commit fully to one direction before backtracking. It explores as deep as possible along each branch before trying alternatives.

The loop runs `i = 3` down to `0` (reversed) because a stack reverses insertion order — pushing in reverse ensures direction index 0 (up) ends up on top and is explored first, keeping traversal order consistent.

```cpp
// solution[dfs].cpp (soal2 / soal3 variant shown — soal1 does not have isWall check yet)
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

        for (int i = 0; i < 4; ++i) {
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

    return visited;
}
```

**Note:** DFS does not guarantee the shortest path. Its efficiency is entirely dependent on whether the map layout happens to align with its traversal direction.

---

### Dijkstra's Algorithm

Dijkstra uses a min-heap priority queue, always expanding the cell with the lowest accumulated cost from the start. This guarantees the optimal path on weighted graphs.

**Soal 1 and 2** use `new_cost = cost + 1` (uniform cost — no need to read cell weights).  
**Soal 3** uses `new_cost = cost + cellCost(grid[nr][nc])` to account for variable terrain costs.

```cpp
// solution[dijkstra].cpp — soal1/soal2 variant (uniform cost)
vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal) {

    vector<Cell> visited;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = { 0, 0, -1, 1};

    using Entry = pair<int, Cell>;
    priority_queue<Entry, vector<Entry>, greater<Entry>> pq;
    map<Cell, int> dist;

    dist[start] = 0;
    came_from[start] = start;
    pq.push({0, start});

    while (!pq.empty()) {
        auto [cost, current] = pq.top();
        pq.pop();

        if (cost > dist[current]) continue;  // discard stale entries

        visited.push_back(current);

        if (current == goal) break;

        for (int k = 0; k < 4; ++k) {
            int nr = current.first + dr[k];
            int nc = current.second + dc[k];

            if (!inBounds(nr, nc)) continue;
            if (isWall(grid[nr][nc])) continue;

            Cell neighbor = {nr, nc};
            int new_cost = cost + 1;  // soal3: cost + cellCost(grid[nr][nc])

            if (!dist.count(neighbor) || new_cost < dist[neighbor]) {
                dist[neighbor] = new_cost;
                came_from[neighbor] = current;
                pq.push({new_cost, neighbor});
            }
        }
    }

    return visited;
}
```

**Limitation:** Dijkstra expands nodes in all directions without any directional guidance. On large maps this results in a wide exploration area even when the goal is in one direction.

---

### A\* Search

A\* extends Dijkstra by incorporating a heuristic `h(n)` — an estimate of the remaining cost to the goal. Each node is prioritized by `f(n) = g(n) + h(n)`, where `g(n)` is the actual accumulated cost and `h(n)` is the Manhattan Distance to the goal.

As with Dijkstra, **Soal 1 and 2** use `new_cost = cost + 1`; **Soal 3** uses `cellCost`.

```cpp
// solution[astar].cpp — soal1/soal2 variant (uniform cost)
inline int heuristic(Cell a, Cell b) {
    return abs(a.first - b.first) + abs(a.second - b.second);
}

vector<Cell> solve(vector<vector<char>>& grid, Cell start, Cell goal) {

    vector<Cell> visited;
    int dr[] = {-1, 1, 0, 0};
    int dc[] = { 0, 0, -1, 1};

    using Entry = tuple<int, int, Cell>;   // (f, g, cell)
    priority_queue<Entry, vector<Entry>, greater<Entry>> pq;
    map<Cell, int> dist;

    dist[start] = 0;
    came_from[start] = start;
    pq.push({heuristic(start, goal), 0, start});

    while (!pq.empty()) {
        auto [f, cost, current] = pq.top();
        pq.pop();

        if (cost > dist[current]) continue;  // discard stale entries

        visited.push_back(current);

        if (current == goal) break;

        for (int k = 0; k < 4; ++k) {
            int nr = current.first + dr[k];
            int nc = current.second + dc[k];

            if (!inBounds(nr, nc)) continue;
            if (isWall(grid[nr][nc])) continue;

            Cell neighbor = {nr, nc};
            int new_cost = cost + 1;  // soal3: cost + cellCost(grid[nr][nc])

            if (!dist.count(neighbor) || new_cost < dist[neighbor]) {
                dist[neighbor] = new_cost;
                came_from[neighbor] = current;
                int h = heuristic(neighbor, goal);
                pq.push({new_cost + h, new_cost, neighbor});
            }
        }
    }

    return visited;
}
```

The Manhattan Distance heuristic is admissible — it never overestimates the true remaining cost since the minimum cell cost is 1. This guarantees A* finds the optimal path. Because cells closer to the goal receive lower `f` scores, A* focuses its search toward the target rather than expanding uniformly in all directions like Dijkstra.

---

## Performance Results

| Problem | Algorithm | Visited | Total Cost | Path Length |
|:---|:---|:---:|:---:|:---:|
| **Soal 1** | **BFS BEST** | **36** | **10** | **10** |
| | DFS | 21 | 20 | 20 |
| | Dijkstra | 36 | 10 | 10 |
| | A* | 36 | 10 | 10 |
| **Soal 2** | BFS | 35 | 14 | 14 |
| | **DFS BEST** | **18** | **14** | **14** |
| | Dijkstra | 37 | 14 | 14 |
| | A* | 28 | 14 | 14 |
| **Soal 3** | BFS | 272 | 294 | 46 |
| | DFS | 149 | 322 | 144 |
| | Dijkstra | 278 | 46 | 46 |
| | **A* BEST** | **151** | **46** | **46** |

*Visited = total cells evaluated. Total Cost = sum of `cellCost` for each cell entered along the path. Path Length = number of steps (cells − 1).*

---

## How to Compile & Run

```bash
# From within the submission/soalX/ directory
g++ -O2 -std=c++17 "solution[bfs].cpp" -o bfs.exe
./bfs.exe ../../maps/soal1.txt "output[bfs].txt"
```

Repeat for each algorithm (`bfs`, `dfs`, `dijkstra`, `astar`) and each soal (`soal1`, `soal2`, `soal3`).

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
`PATH` lists the cells from `S` to `G` along the found route (reconstructed by the harness from `came_from`).

---

## Conclusion

- **BFS** is best for unweighted, uniform-cost grids. It guarantees the shortest path in steps with minimal overhead.
- **DFS** does not guarantee shortest paths. It can achieve low node evaluation counts in specific layouts, but this is not consistent across map types.
- **Dijkstra** guarantees the optimal path on weighted graphs but explores broadly in all directions, which increases node evaluations on large maps.
- **A\*** is the most suitable algorithm for large, weighted grids with a known goal location. It matches Dijkstra's path accuracy while evaluating significantly fewer nodes through heuristic guidance.
