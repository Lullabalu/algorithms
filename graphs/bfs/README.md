### BFS

Queue-based breadth-first traversal.

* Computes shortest distances in an unweighted graph

* `dists[v] = -1` → unvisited

* Start from `s`, set `dists[s] = 0`

* For each neighbor `u`:

  * `dists[u] == -1` → set `dists[u] = dists[v] + 1`, push to queue

* Complexity: `O(V + E)`
