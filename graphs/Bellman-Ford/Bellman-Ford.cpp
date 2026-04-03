#include <algorithm>
#include <cstddef>
#include <vector>

using std::pair;
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

bool BellmanFord(int s, /* const */ Graph &gr, vector<long long> &dists) {
  /*
  Returns true if there is a negative cycle
  */

  dists.assign(gr.size_v(), cInf);

  dists[s] = 0;

  for (int i = 0; i + 1 < gr.size_v(); ++i) {
    for (int v = 0; v < gr.size_v(); ++v) {
      if (dists[v] == cInf) {
        continue;
      }
      for (const Edge &e : gr[v]) {
        dists[e.to] = std::min(dists[e.to], dists[v] + e.w);
      }
    }
  }

  for (int v = 0; v < gr.size_v(); ++v) {
    if (dists[v] == cInf) {
      continue;
    }
    for (const Edge &e : gr[v]) {
      if (dists[e.to] > dists[v] + e.w) {
        return true;
      }
    }
  }
  return false;
}