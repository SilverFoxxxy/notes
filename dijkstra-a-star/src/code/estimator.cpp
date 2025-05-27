#include "estimator.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
// using namespace std;

DSU::DSU(int n): n(n) {
  p.assign(n, 0);
  y.assign(n, 0);
  for (int i = 0; i < n; i++) {
    p[i] = i;
    y[i] = rand();
  }
}

int DSU::Get(int u) {
  while (p[u] != u) {
    u = p[u];
  }
  return u;
}

bool DSU::Unite(int u, int v) {
  u = Get(u);
  v = Get(v);

  if (u == v) {
    return false;
  }

  if (y[u] > y[v]) {
    swap(u, v);
  }

  p[u] = p[v];

  return true;
}

Estimator::Estimator() {}

Estimator::Estimator(
  vector <vector <Edge> >& g
) {}

ll Estimator::Predict(int u, int v) {
  return 0;
}

void Estimator::Dump(std::ofstream& wfile) {}

MSTEstimator::MSTEstimator(
  vector <vector <Edge> >& g
): binup(20, vector <pair <int, ll> > (g.size())), depth(g.size(), 0) {
  int n = g.size();
  DSU dsu(n);
  vector <pair <int, int> > edges;
  vector <pair <ll, int> > mst_edges;
  vector <pair <ll, int> > w_e;
  for (int i = 0; i < n; i++) {
    for (auto [v, w]: g[i]) {
      if (i < v) {
        w_e.push_back({w, edges.size()});
        edges.push_back({i, v});
      }
    }
  }

  sort(w_e.begin(), w_e.end());

  for (int i = 0; i < w_e.size(); i++) {
    auto [u, v] = edges[w_e[i].S];
    if (dsu.Unite(u, v)) {
      mst_edges.push_back({w_e[i].S, w_e[i].F});
    }
  }

  vector <vector <pair <int, ll> > > tree(n);
  for (auto [i, w]: mst_edges) {
    auto [u, v] = edges[i];

    tree[u].push_back({v, w});
    tree[v].push_back({u, w});
  }

  cout << mst_edges.size() << "\n";

  DFS(0, tree);
  for (int k = 1; k < 20; k++) {
    for (int i = 0; i < n; i++) {
      auto [p1, w1] = binup[k - 1][i];
      auto [p2, w2] = binup[k - 1][p1];

      binup[k][i] = {p2, max(w1, w1)};
    }
  }
}

ll MSTEstimator::Predict(int u, int v) {
  if (depth[u] > depth[v]) {
    swap(u, v);
  }

  ll w = 0;

  int h = depth[u] - depth[v];
  for (int k = 0; k < 20; k++) {
    if (h & (1 << k)) {
      auto [v2, w2] = binup[k][v];
      v = v2;
      w = max(w, w2);
    }
  }

  if (u == v) {
    return w;
  }

  for (int k = 19; k >= 0; k--) {
    if (binup[k][u].F == binup[k][v].F) {
      continue;
      auto [u2, w1] = binup[k][u];
      auto [v2, w2] = binup[k][v];
      u = u2;
      v = v2;
      w = max(w, max(w1, w2));
    }
  }

  auto [u2, w1] = binup[0][u];
  auto [v2, w2] = binup[0][v];
  u = u2;
  v = v2;
  w = max(w, max(w1, w2));

  return w;
}

void MSTEstimator::DFS(
  int u,
  vector <vector <pair <int, ll> > >& tree,
  int p // = -1
) {
  if (p == -1) {
    binup[0][u] = {u, 0};
  }
  // cout << u << " in dfs\n";
  for (auto [v, w]: tree[u]) {
    if (v == p) continue;

    binup[0][v] = {u, w};
    depth[v] = depth[u] + 1;
    DFS(v, tree, u);
  }
}

DijkstraEstimator::DijkstraEstimator(
  vector <vector <Edge> >& g,
  vector <Node>& nodes,
  int k // = 10
): dist(k, vector <ll> (g.size(), 1e18)), k(k) {
  int n = g.size();

  // vector <int> dump;

  for (int i = 0; i < k; i++) {
    int s = rand() % n;
    start_points.push_back(s);
    start_points_data.push_back(nodes[s]);
    Dijkstra(s, g, dist[i]);
  }

  // DumpOrder(dump, nodes, "dijkstra-estimator-list.txt");
}

ll DijkstraEstimator::Predict(int u, int v) {
  ll d = 0;
  for (int i = 0; i < k; i++) {
    d = max(d, abs(dist[i][u] - dist[i][v]));
  }
  return d;
}

vector <int>& DijkstraEstimator::GetStartPoints() {
  return start_points;
}

void DijkstraEstimator::Dump(std::ofstream& wfile) {
  wfile << k << "\n";
  for (auto& node: start_points_data) {
    wfile << node.x0 << ' ' << node.y0 << "\n";
  }
}

void DijkstraEstimator::Dijkstra(
  int s,
  vector <vector <Edge> >& g,
  vector <ll>& dist
) {
  priority_queue <pair <ll, int> > pq;
  vector <bool> used(g.size());

  pq.push({0, s});
  dist[s] = 0;

  while (!pq.empty()) {
    auto [d, u] = pq.top();
    pq.pop();
    if (used[u]) {
      continue;
    }
    used[u] = true;

    for (auto [v, w]: g[u]) {
      if (dist[v] > dist[u] + w) {
        dist[v] = dist[u] + w;
        pq.push({-dist[v], v});
      }
    }
  }
}

// KEstimator::Predict {

// }

EuclideanEstimator::EuclideanEstimator(vector <Node>& nodes): nodes(nodes) {
  // cout << nodes.size() << "\n";
}

ll EuclideanEstimator::Predict(int u, int v) {
  // cout << nodes.size() << "\n";
  // cout << u << ' ' << v << "\n";
  return sqrt(pow(nodes[u].x - nodes[v].x, 2) + pow(nodes[u].y - nodes[v].y, 2));
}

EstimatorList::EstimatorList(vector <Estimator*> est): est(est) {}

ll EstimatorList::Predict(int u, int v) {
  ll d = est[0]->Predict(u, v);
  for (auto e: est) {
    // cout << e << " ";
    // cout << "***\n";
    ll cur_predict = e->Predict(u, v);
    // cout << cur_predict << " ";
    d = max(d, cur_predict);
  }
  // cout << " => " << d << "\n";
  // exit(0);
  return d;
}

/*
pair <ll, ll> TwoStartDijkstra(
  int s,
  int t,
  vector <vector <Edge> >& g
) {
  priority_queue <pair <ll, int> > pq1;
  priority_queue <pair <ll, int> > pq2;

  vector <ll> dist1(g.size(), 1e18);
  vector <ll> dist2(g.size(), 1e18);

  vector <bool> used(g.size(), false);

  dist1[s] = 0;
  dist2[t] = 0;

  int pq_count = 0;

  while (true) {
    auto [d1, u] = pq1.top();
    auto [d2, v] = pq2.top();

    ll d = d1;

    auto pq = &pq1;

    if (d1 < d2) {
      for (auto [x, w]: g[u]) {
        if (dist1[x] > dist1[u] + w) {
          dist1[x] = dist1[u]
        }
      }
      // pq = &pq2;
      // d = d2;
      // u = v;
    } else {

    }

    // for (auto [nxt, w]: g[u]) {
    //   if (dist[nxt] > dist[u] + )
    // }
  }
}
*/
