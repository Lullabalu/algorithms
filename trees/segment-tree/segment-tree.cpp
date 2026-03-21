#include <vector>


struct Node { 
  long long sum;
  int sz;
  Node(long long sum, int sz): sum(sum), sz(sz) {}
  Node(): sum(0), sz(0){}
}; // Implement your node

Node Combine(Node x, long long tx, Node y, long long ty) {
  // Implement your function here
  return Node(x.sum + x.sz * tx + y.sum + ty * y.sz, x.sz + y.sz);
};

class SegmentTree {
 private:
  int n;
  std::vector<Node> nodes; // Default int (So that there would be massive additions)
  std::vector<long long> mod;

  void Build(int v, int tl, int tr, std::vector<long long>& arr) {
    if (tr - tl == 1) {
      nodes[v] = Node(arr[tl], 1);
      return;
    }

    int tm = (tl + tr) / 2;
    Build(v * 2 + 1, tl, tm, arr);
    Build(v * 2 + 2, tm, tr, arr);
    nodes[v] = Combine(nodes[v * 2 + 1], mod[v*2+1], nodes[v * 2 + 2], mod[v*2+2]);
  }

  void Push(int v) {
    mod[v * 2 + 1] += mod[v];
    mod[v * 2 + 2] += mod[v];
    mod[v] = 0;
  }

  void AddMulti(int v, int tl, int tr, int l, int r, long long x) {
    if (l <= tl && tr <= r) {
      mod[v] += x;
      return;
    }

    if (tl >= r || tr <= l) {
      return;
    }

    int tm = (tl + tr) / 2;
    Push(v);
    AddMulti(v * 2 + 1, tl, tm, l, r, x);
    AddMulti(v * 2 + 2, tm, tr, l, r, x);

    nodes[v] = Combine(nodes[v * 2 + 1], mod[v*2+1], nodes[v * 2 + 2], mod[v*2+2]);
  }

  void AddOne(int v, int tl, int tr, int pos, long long x) {
    if (tr - tl == 1) {
      nodes[v].sum += x;
      return;
    }
    Push(v);
    int tm = (tl + tr) / 2;
    if (pos < tm) {
      AddOne(v * 2 + 1, tl, tm, pos, x);
    } else {
      AddOne(v * 2 + 2, tm, tr, pos, x);
    }
    nodes[v] = Combine(nodes[v * 2 + 1], mod[v*2+1], nodes[v * 2 + 2], mod[v*2+2]);
  }

  long long GetSum(int v, int tl, int tr, int l, int r) {
    if (l <= tl && tr <= r) {
      return nodes[v].sum + mod[v] * nodes[v].sz;
    }

    if (tl >= r || tr <= l) {
      return 0;
    }
    Push(v);
    int tm = (tr + tl) / 2;
    return GetSum(v * 2 + 1, tl, tm, l, r) + GetSum(v * 2 + 2, tm, tr, l, r);
  }

  long long GetOne(int v, int tl, int tr, int pos) {
    if (tr - tl == 1) {
      return nodes[v].sum + mod[v];
    }
    int tm = (tl + tr) / 2;
    Push(v);
    if (pos < tm) {
      return GetOne(v * 2 + 1, tl, tm, pos);
    }
    return GetOne(v * 2 + 2, tm, tr, pos);
  }

public:
  SegmentTree(int n): n(n), nodes(4 * n), mod(4*n) {}
  SegmentTree(int n, std::vector<long long>&arr):  SegmentTree(n) {
    Build(0, 0, n, arr);
  }

  void BuildTree(std::vector<long long>& arr) {
    n = static_cast<int>(arr.size());
    nodes.assign(4 * n, Node());
    mod.assign(4 * n, 0);
    Build(0, 0, n, arr);
  }
  size_t size() const noexcept {
    return static_cast<size_t>(n);
  }

  void clear() {
    n = 0;
    nodes.clear();
    mod.clear();
  }

  long long Sum(int l, int r) {
    return GetSum(0, 0, n, l, r);
  }

  long long Get(int pos) {
    return GetOne(0, 0, n, pos);
  }

  void Add(int pos, long long x) {
    AddOne(0, 0, n, pos, x);
  }

  void Add(int l, int r, long long x) {
    AddMulti(0, 0, n, l, r, x);
  }

  void Update(int pos, long long x) {
    long long last = Get(pos);
    long long diff = x - last;
    Add(pos, diff);
  }
};