#include <cstddef>
#include <queue>
#include <vector>

using std::pair;
using std::queue;
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

  vector<int> &operator[](int n) {
    return edges[n];
  }

  const vector<int> &operator[](int n) const {
    return edges[n];
  }
};

void Bfs(int s, /* const */ Graph &gr, vector<int> &dists) {
  dists.assign(gr.size_v(), -1);
  dists[s] = 0;
  queue<int> nodes;
  nodes.push(s);

  while (!nodes.empty()) {
    int v = nodes.front();
    nodes.pop();
    for (int u : gr[v]) {
      if (dists[u] == -1) {
        dists[u] = dists[v] + 1;
        nodes.push(u);
      }
    }
  }
}