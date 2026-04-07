#include <algorithm>
#include <memory>
#include <vector>

using std::vector;

typedef long long Ll;

const Ll cMod = 998244353;  // Mod
const Ll cG = 3;            // Primitive root
static vector<int>
    g_rev;  // Array of bit-reversed indices.
            // Example: for 3-bit indices, i = 1 = 001, g_rev[i] = 100 = 4.

inline Ll BinPow(long long arg, long long degree) {
  // Returns arg^degree modulo cMod.
  arg = (arg % cMod + cMod) % cMod;
  Ll result = 1;
  while (degree > 0) {
    if ((degree & 1) != 0) {
      result = (result * arg) % cMod;
    }
    arg = (arg * arg) % cMod;
    degree >>= 1;
  }
  return result;
}

struct Polynomial;
Polynomial operator*(const Polynomial &pol1, const Polynomial &pol2);

struct Polynomial {
  int degree = 0;
  /*
  Number of stored coefficients.
  Example:
    1 + x -> degree = 2
  */
  vector<Ll> odds;
  Polynomial() : degree(1), odds(1, 0) {
  }
  Polynomial(int cof) : degree(cof + 1), odds(degree, 0) {
    // Create x^cof
    odds[cof] = 1;
  }
  Polynomial(vector<Ll> &&arr) : degree(arr.size()), odds(std::move(arr)) {
  }
  Polynomial(vector<Ll> &arr) : degree(arr.size()), odds(arr) {
  }
  Polynomial(const Polynomial &other) = default;
  Polynomial(Polynomial &&other)
      : degree(other.degree),
        odds(std::move(other.odds)) {
  }
  Polynomial(const Polynomial &other, int zero)
      : degree(other.degree / 2),
        odds(degree, 0) {
    /* Special constructor, returns poly with even or odd odds

    Example:
      Polynomial({a0, a1, a2, a3, a4, a5}, 0) =  {a0, a2, a4}
      Polynomial({a0, a1, a2, a3, a4, a5}, 1) = {a1, a3, a5}
    */
    for (int i = 0; i < degree; ++i) {
      odds[i] = other[2 * i + zero];
    }
  }

  void Resize(int need) {
    // Resize to need size
    odds.resize(need, 0);
    degree = need;
  }

  void RemoveZeros() {
    // Remove leading zeros
    for (int i = degree - 1; i >= 0; --i) {
      if (odds[i] != 0) {
        odds.resize(i + 1);
        degree = i + 1;
        return;
      }
    }
    odds.resize(0);
    degree = 0;
  }
  long long operator[](int index) const {
    // Очев
    return odds[index];
  }

  int Size() const {
    return degree;
  }
  Polynomial &operator+=(const Polynomial &other) {
    if (degree < other.degree) {
      Resize(other.degree);
    }
    for (int i = 0; i < other.degree; ++i) {
      odds[i] = (odds[i] + other.odds[i]) % cMod;
    }
    return *this;
  }
  Polynomial operator+(const Polynomial &other) {
    Polynomial copy(*this);
    return copy += other;
  }
  Polynomial &operator-=(const Polynomial &other) {
    if (degree < other.degree) {
      Resize(other.degree);
    }
    for (int i = 0; i < other.degree; ++i) {
      odds[i] = (odds[i] - other.odds[i] + cMod) % cMod;
    }
    return *this;
  }
  Polynomial operator-(const Polynomial &other) const {
    Polynomial copy(*this);
    return copy -= other;
  }

  Polynomial Back(int mod, bool &flag) {
    // Returns the inverse polynomial modulo x^mod.
    // If the inverse does not exist, sets flag = false.
    // Initially call with flag = true.

    if (!flag) {
      return Polynomial();
    }
    if (mod == 1) {
      if (odds.empty() || odds[0] % cMod == 0) {
        flag = false;
        return Polynomial();
      }
      vector<Ll> base(1);
      base[0] = BinPow(odds[0] % cMod, cMod - 2);
      return Polynomial(base);
    }

    Polynomial bk = Back(mod / 2, flag);
    if (!flag) {
      return Polynomial();
    }

    Polynomial cur(*this);
    if (cur.degree != mod) {
      cur.Resize(mod);
    }

    Polynomial pb = cur * bk;
    if (pb.degree != mod) {
      pb.Resize(mod);
    }
    vector<Ll> two_vec(1, 2);
    Polynomial two(two_vec);
    Polynomial ost = two - pb;

    Polynomial res = bk * ost;
    if (res.degree != mod) {
      res.Resize(mod);
    }
    return res;
  }

  Polynomial Reversive() const {
    // Returns the polynomial with reversed coefficients.
    Polynomial copy(*this);
    std::reverse(copy.odds.begin(), copy.odds.end());
    return copy;
  }

  explicit operator bool() const {
    // Before check remove zeros
    return degree != 0;
  }
};

inline void PrepareRev(int cur_n) {
  // Prepare g_rev for cur n
  if (0 == cur_n) {
    return;
  }
  g_rev.assign(cur_n, 0);
  int lg = 0;
  while ((1 << lg) < cur_n) {
    ++lg;
  }
  for (int i = 1; i < cur_n; ++i) {
    g_rev[i] = (g_rev[i >> 1] >> 1) | ((i & 1) << (lg - 1));
  }
}

void BitReverse(vector<Ll> &arr) {
  // Swap elements of array so that the element at
  // position i -> to position g_rev[i]
  int n_size = arr.size();
  PrepareRev(n_size);
  for (int i = 0; i < n_size; ++i) {
    if (i < g_rev[i]) {
      std::swap(arr[i], arr[g_rev[i]]);
    }
  }
}

void Ntt(vector<Ll> &arr, bool invert) {
  /*
  invert:
    false: arr of coefficients -> arr of values
    true: arr of values -> arr of coefficients
  */

  int n_size = arr.size();
  BitReverse(arr);

  for (int len = 1; len < n_size; len <<= 1) {
    Ll wlen = BinPow(cG, (cMod - 1) / (len << 1));
    if (invert) {
      wlen = BinPow(wlen, cMod - 2);
    }

    for (int i = 0; i < n_size; i += (len << 1)) {
      Ll cur_w = 1;
      for (int j = 0; j < len; ++j) {
        Ll first = arr[i + j];
        Ll second = (arr[i + j + len] * cur_w) % cMod;
        Ll sum = first + second;
        if (sum >= cMod) {
          sum -= cMod;
        }
        Ll sub = first - second;
        if (sub < 0) {
          sub += cMod;
        }
        arr[i + j] = sum;
        arr[i + j + len] = sub;
        cur_w = (cur_w * wlen) % cMod;
      }
    }
  }

  if (invert) {
    Ll inv_n = BinPow(n_size, cMod - 2);
    for (Ll &elem : arr) {
      elem = (elem * inv_n) % cMod;
    }
  }
}

vector<Ll> Convolution(const vector<Ll> &pol1, const vector<Ll> &pol2) {
  // Returns coefficients of arr C = A * B, where A = pol1, B = pol2
  if (pol1.empty() || pol2.empty()) {
    return {};
  }
  int need = pol1.size() + pol2.size() - 1;
  int n_size = 1;
  while (n_size < need) {
    n_size <<= 1;
  }

  vector<Ll> vec1;
  vector<Ll> vec2;
  vec1.assign(pol1.begin(), pol1.end());
  vec1.resize(n_size, 0);
  vec2.assign(pol2.begin(), pol2.end());
  vec2.resize(n_size, 0);

  Ntt(vec1, false);
  Ntt(vec2, false);
  for (int i = 0; i < n_size; ++i) {
    vec1[i] = (vec1[i] * vec2[i]) % cMod;
  }
  Ntt(vec1, true);
  vec1.resize(need);
  return vec1;
}

Polynomial operator*(const Polynomial &pol1, const Polynomial &pol2) {
  if (pol1.degree == 0 || pol2.degree == 0) {
    return Polynomial();
  }
  vector<Ll> res = Convolution(pol1.odds, pol2.odds);
  return Polynomial(res);
}

std::ostream &operator<<(std::ostream &out, const Polynomial &pol) {
  for (const auto &c : pol.odds) {
    out << c << ' ';
  }
  return out;
}

Polynomial operator/(const Polynomial &pol1, const Polynomial &pol2) {
  if (pol1.degree < pol2.degree) {
    return Polynomial();
  }
  Polynomial reverse_f = pol1.Reversive();
  Polynomial reverse_g = pol2.Reversive();

  int cur = 1;
  while (cur < pol1.degree - pol2.degree + 1) {
    cur *= 2;
  }

  reverse_f.Resize(cur);
  reverse_g.Resize(cur);
  bool flag = true;
  Polynomial g_back = reverse_g.Back(cur, flag);
  if (!flag) {
    return Polynomial();
  }
  Polynomial reverse_q = reverse_f * g_back;
  reverse_q.Resize(pol1.degree - pol2.degree + 1);
  Polynomial pol_q = reverse_q.Reversive();
  return pol_q;
}

Polynomial operator%(const Polynomial &pol1, const Polynomial &pol2) {
  return pol1 - (pol2 * (pol1 / pol2));
}