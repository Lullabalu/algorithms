#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

using std::queue;
using std::string;
using std::unordered_map;
using std::vector;

struct Node {
  unordered_map<int, int> next;
  bool terminal = false;
};

struct Bor {
  vector<Node> nodes;

  Bor() : nodes(1) {
  }

  int AddString(const string &s) {
    /* returns last node */
    int v = 0;

    for (char c : s) {
      int ord = c - 'a';
      if (!nodes[v].next.contains(ord)) {
        nodes[v].next[ord] = nodes.size();
        nodes.push_back(Node());
      }
      v = nodes[v].next[ord];
    }
    nodes[v].terminal = true;
    return v;
  }

  bool CheckString(const string &s) {
    int v = 0;
    for (char c : s) {
      int ord = c - 'a';
      if (!nodes[v].next.contains(ord)) {
        return false;
      }
      v = nodes[v].next[ord];
    }

    return nodes[v].terminal;
  }

  size_t size() const noexcept {
    return nodes.size();
  }
  Node &operator[](int ind) {
    return nodes[ind];
  }
};

const int c26 = 26;
vector<vector<int>> Aho_Corasick(const string &t, vector<string> &strings) {
  int n = strings.size();
  int full_size = 0;
  for (int i = 0; i < n; ++i) {
    full_size += strings[i].size();
  }
  vector<vector<int>> entry(n);

  Bor bor;

  vector<vector<int>> ids(full_size + 1);
  for (int i = 0; i < n; ++i) {
    int node = bor.AddString(strings[i]);
    ids[node].push_back(i);
  }

  vector<int> link(bor.size(), -1);
  vector<int> heavy_link(bor.size(), -1);
  vector<vector<int>> step(bor.size(), vector<int>(c26, -1));
  for (char c = 'a'; c <= 'z'; ++c) {
    if (bor[0].next.contains(c - 'a')) {
      step[0][c - 'a'] = bor[0].next[c - 'a'];
    } else {
      step[0][c - 'a'] = 0;
    }
  }

  queue<int> q;
  q.push(0);
  while (!q.empty()) {
    int v = q.front();
    q.pop();
    for (int c = 0; c < c26; ++c) {
      if (!bor[v].next.contains(c)) {
        continue;
      }

      int u = bor[v].next[c];
      link[u] = (v == 0) ? 0 : step[link[v]][c];
      for (int d = 0; d < c26; ++d) {
        if (bor[u].next.contains(d)) {
          step[u][d] = bor[u].next[d];
        } else {
          step[u][d] = step[link[u]][d];
        }
      }
      heavy_link[u] = (bor[link[u]].terminal) ? link[u] : heavy_link[link[u]];
      q.push(u);
    }
  }

  int v = 0;
  for (int i = 0; i < t.size(); ++i) {
    v = step[v][t[i] - 'a'];
    if (bor[v].terminal) {
      for (int id : ids[v]) {
        entry[id].push_back(i + 2 - strings[id].size());
      }
    }

    for (int u = heavy_link[v]; u > 0; u = heavy_link[u]) {
      for (int id : ids[u]) {
        entry[id].push_back(i + 2 - strings[id].size());
      }
    }
  }
  return entry;
}