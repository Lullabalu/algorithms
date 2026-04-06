#include <concepts>
#include <vector>
#include <functional>
#include <cstddef>
#include <utility>


template<
    typename HeapElem,
    typename Compare = std::less<HeapElem>
>
requires std::predicate<Compare, HeapElem, HeapElem>
struct Heap {
public:
  explicit Heap(const std::vector<HeapElem>& data, Compare comp) : heap(data), comp(comp) {
    for (int i = static_cast<int>(heap.size()) / 2 - 1; i >= 0; --i) {
        SiftDown(i);
    }
  }
  explicit Heap(const std::vector<HeapElem>& data) : Heap(data, Compare{}) {}

  Heap() = default;
  explicit Heap(Compare comp): comp(comp) {}
  

  void SiftUp(int i) {
    while (i > 0 && comp(heap[i], heap[(i - 1) / 2])) {
      std::swap(heap[i], heap[(i - 1) / 2]);
      i = (i - 1) / 2;
    }
  }

  void SiftDown(int i) {
    while (2 * i + 1 < static_cast<int>(heap.size())) {
      int left = 2 * i + 1;
      int right = 2 * i + 2;

      int best = left;
      if (right < heap.size() && comp(heap[right], heap[left])) {
        best = right;
      }
      if (!comp(heap[best], heap[i])) {
        break;
      }

      std::swap(heap[i], heap[best]);
      i = best;
    }
  }

  HeapElem ExtractTop() {
    HeapElem mn = heap[0];
    heap[0] = heap.back();
    heap.pop_back();
    if (!heap.empty()) {
      SiftDown(0);
    }
    return mn;
  }

  const HeapElem& GetTop() const {
    return heap[0];
  }

  void Insert(const HeapElem& elem) {
    heap.push_back(elem);
    SiftUp(heap.size() - 1);
  }

  void Insert(HeapElem&& elem) {
    heap.push_back(std::move(elem));
    SiftUp(heap.size() - 1);
  }

  std::size_t Size() const noexcept {
    return heap.size();
  }

  bool Empty() const noexcept {
    return heap.empty();
  }

  void Clear() noexcept {
    heap.clear();
  }

private:
  std::vector<HeapElem> heap = {};
  Compare comp = {};
};