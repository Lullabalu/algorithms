#include <string>
#include <unordered_map>
#include <vector>

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
};