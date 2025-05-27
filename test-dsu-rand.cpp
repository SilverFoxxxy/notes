#include <iostream>
#include <vector>
#include <random>

using namespace std;

int cnt = 0;

int get_root(int u, vector <int>& p) {
  int par = p[u];
  while (par != u) {
    cnt++;
    u = par;
    par = p[u];
  }

  return u;
}

void solve() {
  cnt = 0;
  int n = 100000;
  int m = 200000;

  vector <int> p(n);
  vector <int> prior(n);

  vector <int> d(n, 0);

  for (int i = 0; i < n; i++) {
    p[i] = i;
    prior[i] = rand();
  }

  for (int i = 0; i < m; i++) {
    int u = rand() % n;
    int v = rand() % n;

    u = get_root(u, p);
    v = get_root(v, p);

    if (u == v) continue;

    if (i % 2) {
      // if (prior[u] < prior[v]) {
      //   swap(u, v);
      // }

      // if (d[u] > d[v]) swap(u, v);
      // p[u] = v;
      // if (d[u] == d[v]) {
      //   d[v]++;
      //   continue;
      // }

      if (rand() % 2) {
        swap(u, v);
      }

      p[u] = v;
    }
  }

  cout << cnt << endl;
}

int main() {
  solve();
}
