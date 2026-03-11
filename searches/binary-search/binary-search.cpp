#include <concepts>
#include <vector>
#include <functional>

template<
    typename T,
    typename Compare = std::less<T>
>
requires std::predicate<Compare, T, T>
int BinarySearch(const std::vector<T>& v, const T& target,  Compare comp = {}) {
  int left = 0;
  int right = v.size();

  while (left < right) {
    int middle = (left + right) / 2;

    if (comp(target, v[middle]))
      {
        right = middle;
      }
      else
      {
        left = middle + 1;
      }
  }

  return left;
}