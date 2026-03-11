#include <functional>

template<typename Func>
double TernarySearch(Func f, double  left, double right, double eps) {
  while (right - left > eps) {
    double m1 = left + (right - left) / 3;
    double m2 = right - (right - left) / 3;
    if (f(m1) < f(m2)) {
      right = m2;
    }
    else {
      left = m1;
    }
  }

  return (right + left) / 2;
}