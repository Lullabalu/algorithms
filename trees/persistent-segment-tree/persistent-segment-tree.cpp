#include <vector>

struct Node {
  long long sum;
  int tl;
  int tr;
  Node *left = nullptr;
  Node *right = nullptr;
  Node(long long sum, int tl, int tr) : sum(sum), tl(tl), tr(tr) {
  }
  Node() : sum(0), tl(0), tr(0) {
  }
};  // Implement your node

inline void UpdateNode(Node *v) {
  v->sum = v->left->sum + v->right->sum;
  v->tl = v->left->tl;
  v->tr = v->right->tr;
}

class PersistentSegmentTree {
 private:
  int n;
  std::vector<Node *>
      nodes;  // Default long long (So that there would be massive additions)
  Node *root;

  Node *Build(int tl, int tr, std::vector<long long> &arr) {
    if (tr - tl == 1) {
      nodes.push_back(new Node(arr[tl], tl, tr));
      return nodes.back();
    }

    int tm = (tl + tr) / 2;
    Node *left = Build(tl, tm, arr);
    Node *right = Build(tm, tr, arr);
    nodes.push_back(new Node());
    Node *cur = nodes.back();
    cur->left = left;
    cur->right = right;
    UpdateNode(cur);
    return cur;
  }

  long long GetSum(Node *v, int tl, int tr) {
    if (tl <= v->tl && v->tr <= tr) {
      return v->sum;
    }
    if (tl >= v->tr || tr <= v->tl) {
      return 0;
    }
    return GetSum(v->left, tl, tr) + GetSum(v->right, tl, tr);
  }

 public:
  PersistentSegmentTree(int n) : n(n), root(nullptr) {
  }
  PersistentSegmentTree(int n, std::vector<long long> &arr)
      : PersistentSegmentTree(n) {
    root = Build(0, n, arr);
  }
  ~PersistentSegmentTree() {
    Clear();
  }

  void BuildTree(std::vector<long long> &arr) {
    Clear();
    n = static_cast<int>(arr.size());
    root = Build(0, n, arr);
  }
  size_t size() const noexcept {
    return static_cast<size_t>(n);
  }

  void Clear() {
    for (Node *v : nodes) {
      delete v;
    }
    nodes.clear();
    n = 0;
    root = nullptr;
  }

  long long Sum(Node *v, int l, int r) {
    return GetSum(v, l, r);
  }
  long long Sum(int l, int r) {
    if (root == nullptr) {
      return 0;
    }
    return GetSum(root, l, r);
  }

  Node *Change(Node *v, int pos, long long x) {
    if (v->tr - v->tl == 1) {
      nodes.push_back(new Node(x, v->tl, v->tr));
      return nodes.back();
    }
    nodes.push_back(new Node(0, v->tl, v->tr));
    Node *cur = nodes.back();

    int tm = (v->tl + v->tr) / 2;

    if (pos < tm) {
      Node *left = Change(v->left, pos, x);
      cur->left = left;
      cur->right = v->right;
    } else {
      Node *right = Change(v->right, pos, x);
      cur->left = v->left;
      cur->right = right;
    }
    UpdateNode(cur);
    return cur;
  }
};