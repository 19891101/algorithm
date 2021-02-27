
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

std::vector<int> ComputeSA1(const std::string &s) {
  int n = static_cast<int>(s.size());
  std::vector<int> sa(n);
  for (int i = 0; i < n; ++i) {
    sa[i] = i;
  }
  std::sort(sa.begin(), sa.end(), [&](int p1, int p2) {
    for (; p1 < n && p2 < n && s[p1] == s[p2]; ++p1, ++p2)
      ;
    return p1 < n && p2 < n ? s[p1] < s[p2] : p1 == n;
  });
  return sa;
}

std::vector<int> ComputeSA2(const std::string &s) {
  int n = static_cast<int>(s.size());
  std::vector<int> sa(n);
  std::vector<int> rk(n);
  for (int i = 0; i < n; ++i) {
    sa[i] = i;
    rk[i] = s[i];
  }
  for (int w = 1; w < n; w <<= 1) {
    std::sort(sa.begin(), sa.end(), [&](int x, int y) {
      return rk[x] == rk[y]
                 ? (x + w < n && y + w < n ? rk[x + w] < rk[y + w] : x + w >= n)
                 : rk[x] < rk[y];
    });
    std::vector<int> rk_tmp = rk;
    rk[sa[0]] = 0;
    for (int p = 0, i = 1; i < n; ++i) {
      if (rk_tmp[sa[i]] == rk_tmp[sa[i - 1]] &&
          ((sa[i] + w >= n && sa[i - 1] + w >= n) ||
           (sa[i] + w < n && sa[i - 1] + w < n &&
            rk_tmp[sa[i] + w] == rk_tmp[sa[i - 1] + w]))) {
        rk[sa[i]] = p;
      } else {
        rk[sa[i]] = ++p;
      }
    }
  }
  return sa;
}

std::vector<int> ComputeSA3(const std::string &s) {
  int n = static_cast<int>(s.size());
  std::vector<int> sa(n);
  std::vector<int> rk(n);
  for (int i = 0; i < n; ++i) {
    sa[i] = i;
    rk[i] = s[i];
  }
  for (int w = 1; w < n; w <<= 1) {
    std::sort(sa.begin(), sa.end(), [&](int x, int y) {
      return rk[x] == rk[y]
                 ? (x + w < n && y + w < n ? rk[x + w] < rk[y + w] : x + w >= n)
                 : rk[x] < rk[y];
    });
    std::vector<int> rk_tmp = rk;
    rk[sa[0]] = 0;
    for (int p = 0, i = 1; i < n; ++i) {
      if (rk_tmp[sa[i]] == rk_tmp[sa[i - 1]] &&
          ((sa[i] + w >= n && sa[i - 1] + w >= n) ||
           (sa[i] + w < n && sa[i - 1] + w < n &&
            rk_tmp[sa[i] + w] == rk_tmp[sa[i - 1] + w]))) {
        rk[sa[i]] = p;
      } else {
        rk[sa[i]] = ++p;
      }
    }
  }
  return sa;
}

template <typename Elem>
std::vector<Elem> CountingSort(const std::vector<Elem> &eles,
                               std::function<int(const Elem &)> functor,
                               int C) {
  int n = static_cast<int>(eles.size());
  std::vector<int> bucket(C, 0);
  for (int i = 0; i < n; ++i) {
    ++bucket[functor(eles[i])];
  }
  for (int i = 1; i < C; ++i) {
    bucket[i] += bucket[i - 1];
  }
  std::vector<Elem> result(n);
  for (int i = n - 1; i >= 0; --i) {
    result[--bucket[functor(eles[i])]] = eles[i];
  }
  return result;
}

template <typename Elem>
std::vector<Elem>
RadixSort(const std::vector<Elem> &eles, int k, const std::vector<int> &cs,
          std::function<int(const Elem &, int index)> functor) {
  std::vector<Elem> result = eles;
  for (int i = k - 1; i >= 0; --i) {
    result = CountingSort<Elem>(
        result, std::bind(functor, std::placeholders::_1, i), cs[i]);
  }
  return result;
}

int main() {
  struct Element {
    std::vector<int> key;
    Element(int a = 0, int b = 0, int c = 0) {
      key.push_back(a);
      key.push_back(b);
      key.push_back(c);
    }
  };

  constexpr int K = 3;
  std::vector<int> cs = {3, 2, 3};
  std::vector<Element> eles;
  eles.emplace_back(2, 0, 0);
  eles.emplace_back(1, 1, 2);
  eles.emplace_back(0, 1, 0);
  eles.emplace_back(2, 1, 0);
  std::function<int(const Element &, int index)> functor =
      [](const Element &x, int index) { return x.key[index]; };
  eles = RadixSort<Element>(eles, K, cs, functor);
  for (const auto &e : eles) {
    std::cout << e.key[0] << " " << e.key[1] << " " << e.key[2] << "\n";
  }
  /*
    std::vector<int> eles = {4, 3, 2, 0, 0, 2, 4};
    std::function<int(const int &)> functor = [](const int &x) { return x; };
    auto r = CountingSort(eles, functor, 5);
    for (int x : r)
      std::cout << x << " ";
    std::cout << "\n";
    return 0; */

  /*  auto sa = ComputeSA2("aabaaaab");
    for (int x : sa) {
      std::cout << x << " ";
    }
    std::cout << "\n"; */
  return 0;
}
