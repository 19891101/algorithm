
#include <algorithm>
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

int main() {
  auto sa = ComputeSA2("aabaaaab");
  for (int x : sa) {
    std::cout << x << " ";
  }
  std::cout << "\n";
  return 0;
}
