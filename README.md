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
2 = cost 2
3 = cost 3
9 = cost 9
. = cost 1
# = impassable wall

The design for this map where there are rows of cost 9 cells makes crossing the shortest past more expensive in cost. The only free cost detour is through the single gap at right side of the grid.


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

## Results Summary

| Problem | Algorithm | Visited | Total Cost | Path Length |
| :--------: | :----------: | :--------: | :--------: | :--------: |
| Soal 1 | BFS | 36 | 10 | 10 |
| Soal 1 | DFS | 21 | 20 | 20 |
| Soal 1 | Dijkstra | 36 | 10 | 10 |
| Soal 1 | A* | 36 | 10 | 10 |
| Soal 2 | BFS | 35 | 14 | 14 |
| Soal 2 | DFS | 18 | 14 | 14 |
| Soal 2 | Dijkstra | 37 | 14 | 14 |
| Soal 2 | A* | 28 | 14 | 14 |
| Soal 3 | BFS | 272 | 294 | 46 |
| Soal 3 | DFS | 149 | 322 | 145 |
| Soal 3 | Dijkstra | 278 | 46 | 46 |
| Soal 3 | A* | 151 | 46 | 46 |



















