#include <algorithm>
#include <concepts>
#include <cstddef>
#include <functional>
#include <utility>
#include <vector>

template <typename HeapElem, typename Compare = std::less<HeapElem>>
  requires std::predicate<Compare, HeapElem, HeapElem>
struct Tree {
  HeapElem root;
  std::vector<Tree> children;

  Tree() = default;
  explicit Tree(const HeapElem &v) : root(v) {
  }
  explicit Tree(HeapElem &&v) : root(std::move(v)) {
  }

  [[nodiscard]]
  std::size_t order() const noexcept {
    return children.size();
  }

  [[nodiscard]]
  static Tree merge(Tree lhs, Tree rhs, Compare comp = Compare{}) {
    if (comp(rhs.root, lhs.root)) {
      std::swap(lhs, rhs);
    }
    lhs.children.push_back(std::move(rhs));
    return lhs;
  }
};

template <typename HeapElem, typename Compare = std::less<HeapElem>>
  requires std::predicate<Compare, HeapElem, HeapElem>
struct BinomialHeap {
 private:
  using TreeT = Tree<HeapElem, Compare>;

  std::vector<TreeT *> trees;
  [[no_unique_address]] Compare comp;

  void ensure_size(std::size_t n) {
    if (trees.size() < n) {
      trees.resize(n, nullptr);
    }
  }
  void trim() {
    while (!trees.empty() && trees.back() == nullptr) {
      trees.pop_back();
    }
  }

 public:
  BinomialHeap() = default;
  explicit BinomialHeap(Compare comp) : comp(std::move(comp)) {
  }
  BinomialHeap(const BinomialHeap &) = delete;
  BinomialHeap &operator=(const BinomialHeap &) = delete;
  BinomialHeap(BinomialHeap &&other) noexcept
      : trees(std::move(other.trees)),
        comp(std::move(other.comp)) {
    other.trees.clear();
  }
  BinomialHeap &operator=(BinomialHeap &&other) noexcept {
    if (this != &other) {
      for (auto *t : trees) {
        delete t;
      }
      trees = std::move(other.trees);
      comp = std::move(other.comp);
      other.trees.clear();
    }
    return *this;
  }
  ~BinomialHeap() {
    for (auto *t : trees) {
      delete t;
    }
  }

  bool empty() const noexcept {
    for (auto *t : trees) {
      if (t != nullptr)
        return false;
    }
    return true;
  }

  void insert(HeapElem value) {
    BinomialHeap tmp(comp);
    tmp.ensure_size(1);
    tmp.trees[0] = new TreeT(std::move(value));
    merge(tmp);
  }

  void merge(BinomialHeap &other) {
    std::size_t max_sz = std::max(trees.size(), other.trees.size());
    ensure_size(max_sz + 1);
    other.ensure_size(max_sz + 1);

    TreeT *ost = nullptr;

    for (std::size_t i = 0; i <= max_sz; ++i) {
      TreeT *a = trees[i];
      TreeT *b = other.trees[i];

      int cnt = (a != nullptr) + (b != nullptr) + (ost != nullptr);

      if (cnt == 0) {
        continue;
      }

      if (cnt == 1) {
        trees[i] = a ? a : (b ? b : ost);
        ost = nullptr;
      } else if (cnt == 2) {
        TreeT *x;
        TreeT *y;

        if (!a) {
          x = b;
          y = ost;
        } else if (!b) {
          x = a;
          y = ost;
        } else {
          x = a;
          y = b;
        }

        ost = new TreeT(TreeT::merge(std::move(*x), std::move(*y), comp));
        delete x;
        delete y;
        trees[i] = nullptr;
      } else {
        trees[i] = ost;
        ost = new TreeT(TreeT::merge(std::move(*a), std::move(*b), comp));
        delete a;
        delete b;
      }

      other.trees[i] = nullptr;
    }

    trim();
    other.trim();
  }

  const HeapElem &top() const {
    TreeT *best = nullptr;

    for (auto *t : trees) {
      if (!t) {
        continue;
      }
      if (!best || comp(t->root, best->root)) {
        best = t;
      }
    }
    return best->root;
  }

  HeapElem extract_top() {
    std::size_t best_i = 0;
    bool found = false;

    for (std::size_t i = 0; i < trees.size(); ++i) {
      if (!trees[i]) {
        continue;
      }
      if (!found || comp(trees[i]->root, trees[best_i]->root)) {
        best_i = i;
        found = true;
      }
    }

    TreeT *best = trees[best_i];
    HeapElem res = std::move(best->root);

    BinomialHeap tmp(comp);
    tmp.ensure_size(best->order());

    for (std::size_t i = 0; i < best->children.size(); ++i) {
      tmp.trees[i] = new TreeT(std::move(best->children[i]));
    }

    delete best;
    trees[best_i] = nullptr;

    merge(tmp);
    return res;
  }
};
