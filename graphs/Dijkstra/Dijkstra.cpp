#include <cstddef>
#include <set>
#include <vector>

using std::pair;
using std::set;
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

void Dijkstra(int s, /* const */ Graph &gr, vector<long long> &dists) {
  dists.assign(gr.size_v(), -1);

  set<Edge> nodes;
  dists[s] = 0;
  nodes.insert({s, 0});

  while (!nodes.empty()) {
    while (!nodes.empty()) {
      Edge top = *nodes.begin();
      if (dists[top.to] != top.w) {
        nodes.erase(nodes.begin());
        continue;
      } else {
        break;
      }
    }
    if (nodes.empty()) {
      break;
    }

    Edge top = *nodes.begin();
    nodes.erase(nodes.begin());

    int v = top.to;

    for (const Edge& e : gr[v]) {
      if (dists[e.to] == -1 || dists[e.to] > dists[v] + e.w) {
        dists[e.to] = dists[v] + e.w;
        nodes.insert({e.to, e.w + dists[v]});
      }
    }
  }
}