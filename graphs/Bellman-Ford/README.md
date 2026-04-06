### Bellman-Ford

Shortest paths from source `s` in a graph with possible negative weights.

* Handles **negative edges**

* `dists[v] = INF` → unreachable

* Start from `s`, set `dists[s] = 0`

* Relax all edges `V - 1` times

* Extra iteration detects a **negative cycle**

* Returns `true` if a reachable negative cycle exists

* Complexity: `O(V * E)`
