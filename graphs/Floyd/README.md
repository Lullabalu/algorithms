### Floyd

All-pairs shortest paths in a weighted graph.

* Handles **positive and negative weights** (no negative cycles)

* `dists[i][j]` — shortest distance from `i` to `j`

* Initialize:

  * `dists[v][v] = 0`
  * edges from graph

* For each intermediate vertex `k`:

  * try to improve paths `i → j` via `k`

* Complexity: `O(V^3)`
