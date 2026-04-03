### Dijkstra

Shortest paths from source `s` in a weighted graph.

* Works for **non-negative** edge weights

* `dists[v] = -1` → unreachable

* Start from `s`, set `dists[s] = 0`

* For each edge `(v -> u, w)`:

  * if `u` is unreachable or a shorter path is found, update `dists[u]`

* Uses `set` to always process the vertex with the smallest current distance

* Complexity: `O((V + E) log E)`
