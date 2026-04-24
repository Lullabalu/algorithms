#include <algorithm>
#include <string>
#include <vector>

using std::max;
using std::min;
using std::string;
using std::vector;

vector<int> Z(const string &s) {
  int n = s.size();
  vector<int> function(n, 0);
  int l = 0, r = 0;
  for (int i = 1; i < n; ++i) {
    function[i] = max(0, min(r - i, function[i - l]));
    while (i + function[i] < n && s[function[i]] == s[i + function[i]]) {
      ++function[i];
    }

    if (i + function[i] > r) {
      l = i;
      r = i + function[i];
    }
  }

  return function;
}