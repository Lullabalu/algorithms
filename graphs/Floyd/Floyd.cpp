#include <algorithm>
#include <cstddef>
#include <vector>

using std::vector;

struct Edge {
  int to = 0;
  long long w = 0;

  bool operator<(const Edge &other) const {
    if (w != other.w) {
      return w < other.w;
    }
    return to < other.to;
  }
};

struct Graph {
 public:
  size_t count_nodes = 0;
  size_t count_edges = 0;
  vector<vector<Edge>> edges;
  Graph(int n) : count_nodes(n), edges(vector<vector<Edge>>(n)) {
  }

  void AddEdge(int from, Edge e) {
    edges[from].push_back(e);
    count_edges += 1;
  }

  size_t size_v() const noexcept {
    return count_nodes;
  }

  size_t size_e() const noexcept {
    return count_edges;
  }

  vector<Edge> &operator[](int n) {
    return edges[n];
  }

  const vector<Edge> &operator[](int n) const {
    return edges[n];
  }
};

const long long cInf = 1e18;

void Floyd(/* const */ Graph &gr, vector<vector<long long>> &dists) {
  dists.assign(gr.size_v(), vector<long long>(gr.size_v(), cInf));
  for (int v = 0; v < gr.size_v(); ++v) {
    dists[v][v] = 0;
    for (const Edge &edge : gr[v]) {
      dists[v][edge.to] = std::min(edge.w, dists[v][edge.to]);
    }
  }

  for (int mid = 0; mid < gr.size_v(); ++mid) {
    for (int i = 0; i < gr.size_v(); ++i) {
      for (int j = 0; j < gr.size_v(); ++j) {
        if (dists[i][mid] < cInf && dists[mid][j] < cInf) {
          dists[i][j] = std::min(dists[i][j], dists[i][mid] + dists[mid][j]);
        }
      }
    }
  }
}