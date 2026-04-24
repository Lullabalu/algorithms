#include <algorithm>
#include <string>
#include <vector>

using std::max;
using std::pair;
using std::string;
using std::vector;

const int c256 = 256;
void FirstSort(const string &str, vector<int> &perm, vector<int> &cls,
               int &classes) {

  /* first Sort
  return perm, cls and count of classes
  */
  int n = str.size();
  vector<int> cnt(max(n, c256), 0);

  for (int i = 0; i < n; ++i) {
    ++cnt[static_cast<unsigned char>(str[i])];
  }

  for (int i = 1; i < cnt.size(); ++i) {
    cnt[i] += cnt[i - 1];
  }

  for (int i = 0; i < n; ++i) {
    int pos = --cnt[static_cast<unsigned char>(str[i])];
    perm[pos] = i;
  }

  cls[perm[0]] = 0;
  classes = 1;
  for (int i = 1; i < n; ++i) {
    if (str[perm[i]] != str[perm[i - 1]]) {
      ++classes;
    }
    cls[perm[i]] = classes - 1;
  }
}

void Sort(vector<int> &left, vector<int> &cls, int classes, vector<int> &perm) {
  /*
  Stable sort using left sides and classes if left sides
  [left][right]

  */
  int n = left.size();
  vector<int> cnt(classes, 0);
  for (int i = 0; i < n; ++i) {
    ++cnt[cls[left[i]]];
  }

  vector<int> pos(classes, 0);
  for (int clas = 1; clas < classes; ++clas) {
    pos[clas] = pos[clas - 1] + cnt[clas - 1];
  }

  for (int i = 0; i < n; ++i) {
    int clas = cls[left[i]];
    perm[pos[clas]++] = left[i];
  }
}

int NewClasses(vector<int> &perm, vector<int> &cls, int len,
               vector<int> &cls_tmp) {
  /*
  Create new classes using perm, old_classes (cls) and len of half
  */
  int n = cls_tmp.size();
  cls_tmp[perm[0]] = 0;
  int classes = 1;

  for (int i = 1; i < n; ++i) {
    pair<int, int> prev = {cls[perm[i - 1]], cls[(perm[i - 1] + len) % n]};
    pair<int, int> cur = {cls[perm[i]], cls[(perm[i] + len) % n]};
    if (cur != prev) {
      ++classes;
    }

    cls_tmp[perm[i]] = classes - 1;
  }

  return classes;
}

vector<int> SuffixArr(const string &input_str) {
  /* Create SuffixArr

  */
  string str = input_str;
  str.push_back('\0');

  int n = str.size();
  vector<int> cls(n, 0);
  vector<int> perm(n, 0);
  int classes = 0;
  FirstSort(str, perm, cls, classes);

  vector<int> perm_tmp(n, 0);
  vector<int> cls_tmp(n, 0);

  for (int k = 0; (1 << k) < n; ++k) {
    int len = (1 << k);

    for (int i = 0; i < n; ++i) {
      int shift = perm[i] - len;
      if (shift < 0) {
        shift += n;
      }
      perm_tmp[i] = shift;
    }

    Sort(perm_tmp, cls, classes, perm);

    classes = NewClasses(perm, cls, len, cls_tmp);
    cls.swap(cls_tmp);
  }

  vector<int> ans;
  for (int i = 1; i < n; ++i) {
    ans.push_back(perm[i]);
  }
  return ans;
}