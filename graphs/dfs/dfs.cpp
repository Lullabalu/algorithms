#include <cstddef>
#include <vector>

using std::vector;

struct Graph {
 public:
  size_t count_nodes = 0;
  size_t count_edges = 0;
  vector<vector<int>> edges;
  Graph(int n) : count_nodes(n), edges(vector<vector<int>>(n)) {
  }

  void AddEdge(int from, int to) {
    edges[from].push_back(to);
    count_edges += 1;
  }

  size_t size_v() const noexcept {
    return count_nodes;
  }

  size_t size_e() const noexcept {
    return count_edges;
  }
};

void Dfs(int v, /* const */ Graph &gr, vector<int> &color) {
  color[v] = 1;
  // Do something
  for (auto u : gr.edges[v]) {
    // Do something
    if (color[u] == 1) {
      // Do something
    }
    if (color[u] == 0) {
      Dfs(u, gr, color);
    }
  }
  color[v] = 2;
}