#include "src/code/estimator.h"
#include "src/code/progress_bar.h"
#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include <ctime>
#include <chrono>
#include <unordered_map>

using namespace std;

#define ll long long
#define ld long double
#define F first
#define S second

struct DistNode {
  ll x = 1e18;

  DistNode() {}

  DistNode(ll x): x(x) {}

  operator ll() const { return x; }
};

pair <ll, ll> AStar(
  int s,
  int t,
  vector <vector <Edge> >& g,
  Estimator *est,
  vector <int>& order
) {
  // ll predict_count = 0;
  ll pq_count = 0;
  // vector <bool> used(g.size(), false);
  unordered_map <int, bool> used;
  priority_queue <pair <ll, int> > pq;
  pq.push({0, s});

  // vector <ll> dist(g.size(), 1e18);
  unordered_map <int, DistNode> dist;
  dist[s].x = 0;

  while (!pq.empty()) {
    auto [d, u] = pq.top();
    d = dist[u].x;
    // cout << u << ' ' << d << "\n";

    pq.pop();

    if (used[u]) continue;
    used[u] = true;

    order.push_back(u);

    if (u == t) {
      return {d, pq_count};
    }

    for (auto [v, w]: g[u]) {
      if (dist[v].x > d + w) {
        dist[v].x = d + w;
        // predict_count++;
        pq_count++;
        pq.push({-d - w - est->Predict(v, t), v});
      }
    }
  }

  return {1e18, pq_count};
}

struct EstimatorInfo {
  Estimator *est = nullptr;
  string filename;
};

void DumpOrder(
  Estimator *est,
  vector <int>& order,
  vector <Node>& nodes,
  string filename
) {
  // est.Dump();
  std::ofstream wfile;
  wfile.open(filename);
  wfile << order.size() << "\n";
  for (int i = 0; i < order.size(); i++) {
    wfile << nodes[order[i]].x0 << " " << nodes[order[i]].y0 << "\n";
  }
  est->Dump(wfile);
}

void Solve(
  vector <vector <Edge> >& g,
  vector <Node>& nodes,
  vector <pair <int, int> >& query,
  bool with_dump = false
) {
  vector <EstimatorInfo> estimators = {
    {new DijkstraEstimator(g, nodes, 6), "path_dump/dij_1.txt"},
    {new DijkstraEstimator(g, nodes, 6), "path_dump/dij_2.txt"},
    {new DijkstraEstimator(g, nodes, 6), "path_dump/dij_3.txt"},
    {new DijkstraEstimator(g, nodes, 6), "path_dump/dij_4.txt"},
    {new EuclideanEstimator(nodes), "path_dump/euc.txt"}
  };

  vector <ll> check_sum(estimators.size(), 0);
  vector <ll> check_count_sum(estimators.size(), 0);

  // int query_id = 0;
  for (auto [u, v]: query) {
    // query_id++;

    int i = -1;
    for (auto [est, dump_filename]: estimators) {
      i++;
      vector <int> ord_dump;
      auto [cur_dist, cur_count] = AStar(u, v, g, est, ord_dump);
      check_sum[i] += cur_dist;
      check_count_sum[i] += cur_count;

      if (with_dump) {
        DumpOrder(est, ord_dump, nodes, dump_filename);
      }
    }
  }

  for (int i = 0; i < estimators.size(); i++) {
    cout << check_sum[i] << ' ' << check_count_sum[i] << "\n";
  }
  cout << "\n";
}

pair <ll, ll> GetXYFromLongLang(ld y, ld x) {
  // Расстояние в метрах = градусы широты * 111,132 км/градус * 1000 м/км
  // Расстояние в метрах = градусы долготы * 111,132 * cos(широта) км/градус * 1000 м/км
  return {(ll)((x - 55) * 111.132 * 1000), (ll)((y - 37) * 111.132 * cos(x * acos(-1) / 180.) * 1000)};
}

void GetGraph(
  vector <vector <Edge> >& g,
  vector <Node>& nodes,
  string filename
) {
  std::ifstream rfile;
  rfile.open(filename);

  int n, m;
  rfile >> n >> m;
  cout << n << ' ' << m << "\n";
  nodes.resize(n);
  g.resize(n);
  for (int i = 0; i < n; i++) {
    ld x;
    ld y;
    rfile >> x >> y;

    auto [x1, y1] = GetXYFromLongLang(x, y);
    nodes[i].x = x1;
    nodes[i].y = y1;
    nodes[i].x0 = x;
    nodes[i].y0 = y;
  }

  for (int i = 0; i < m; i++) {
    int u, v;
    ld w;
    rfile >> u >> v >> w;
    w = ceil(max(w, (ld)sqrt(pow(nodes[u].x - nodes[v].x, 2) + pow(nodes[u].y - nodes[v].y, 2))));
    g[u].push_back({v, (ll)(w)});
    g[v].push_back({u, (ll)(w)});
  }
}

void GetRandomGraph(
  vector <vector <Edge> > g,
  vector <Node>& nodes
) {
  int n = 100000;
  double p = 10;
  int m = p * n;

  // int q = 100;

  g.resize(n);
  nodes.resize(n);

  for (int i = 0; i + 1 < n; i++) {
    int u = i;
    int v = i + 1;
    g[u].push_back({v, (ll)1e18 / n});
    g[v].push_back({u, (ll)1e18 / n});
  }

  for (int i = 0; i < m; i++) {
    int u = rand() % n;
    int v = rand() % n;
    int w = rand();
    g[u].push_back({v, w});
    g[v].push_back({u, w});
  }
}

void GetQueries(
  vector <pair <int, int> >& query,
  int n
) {
  query = {
    {263787, 96267}
    // {263177, 336633}
    // {239903, 192430}
    // {85029, 76203}
    // {29727, 53749}
    // {219164, 178051}
    // {14170, 16468}
    // {4068, 4413}
    // {49, 100}
    // {49, 100}
    // {rand() % n, rand() % n}
    // {5182, 6673}
    // {9430, 5793},
  };
}

void GetRandomQueries(
  vector <pair <int, int> >& query,
  int n,
  int query_count = 10
) {
  int q = query_count;
  for (int i = 0; i < q; i++) {
    int u = rand() % n;
    int v = rand() % n;

    query.push_back({u, v});
  }
}

int DFSVisit(
  int u,
  vector <vector <Edge> >& g,
  vector <bool>& used,
  int depth = 0
) {
  cout << u + 1 << " " << depth << " connect\n";
  if (used[u]) return 0;
  used[u] = true;

  int sz = 1;

  for (auto& e: g[u]) {
    sz += DFSVisit(e.v, g, used, depth + 1);
  }

  return sz;
}

bool CheckConnectivity(
  vector <vector <Edge> >& g
) {
  int n = g.size();

  vector <bool> used(n, false);
  int count = DFSVisit(0, g, used);

  for (int i = 0; i < n; i++) {
    if (used[i]) continue;
    // int count = DFSVisit(i, g, used);
    // cout << count << " = sz" << (i + 1) << "\n";
  }
  return (count == n);
}

void BuildEstBlocks(
  vector <vector <Estimator*> >& est_blocks,
  vector <string>& block_names,
  vector <vector <Edge> >& g,
  vector <Node>& nodes,
  vector <int>& list_k,
  int block_size
) {

  const int kMaxK = 8;

  // vector <int> list_k = {1, 2, 3, 5, 7, 10, 15, 20};
  // vector <int> list_k = {1};
  // const int kBlockSize = 5;
  const int kBlockSize = block_size;

  int all_work = 2;
  for (int j = 0; j < list_k.size(); j++) {
    for (int i = 0; i < kBlockSize; i++) {
      all_work++;
    }
  }

  // int all_work = 2 + kMaxK * kBlockSize;
  ProgressBar progress_bar(all_work);

  {
    est_blocks.push_back({});
    // for (int i = 0; i < 1; i++) {
    string cur_block_name = "Dijkstra";
    est_blocks.back().push_back(new Estimator());
    block_names.push_back(cur_block_name);
    progress_bar.Update();
  }

  {
    est_blocks.push_back({});
    // for (int i = 0; i < 1; i++) {
    string cur_block_name = "Euclidean Estimator";
    est_blocks.back().push_back(new EuclideanEstimator(nodes));
    block_names.push_back(cur_block_name);

    progress_bar.Update();
  }

  // }

  for (int k: list_k) {
    est_blocks.push_back({});
    string cur_block_name = "K-Estimator (k = ";
    cur_block_name += to_string(k);
    cur_block_name += ")";
    block_names.push_back(cur_block_name);
    for (int i = 0; i < kBlockSize; i++) {
      est_blocks.back().push_back(new DijkstraEstimator(g, nodes, k));
      progress_bar.Update();
    }
  }

  for (int k_i = 0; k_i < list_k.size(); k_i++) {
    est_blocks.push_back({});
    string cur_block_name = "Euc + K-Est (k = ";
    cur_block_name += to_string(list_k[k_i]);
    cur_block_name += ")";
    block_names.push_back(cur_block_name);
    for (int i = 0; i < kBlockSize; i++) {
      est_blocks.back().push_back(new EstimatorList({est_blocks[0][0], est_blocks[k_i + 2][i]}));
      progress_bar.Update();
    }
  }

}

void DFSColor(
  int u,
  vector <vector <Edge> >& g,
  vector <int>& color,
  int cur_color
) {
  if (color[u]) return;
  color[u] = cur_color;
  for (auto& e: g[u]) {
    DFSColor(e.v, g, color, cur_color);
  }
}

void CheckQueryConnected(
  vector <vector <Edge> >& g,
  vector <pair <int, int> >& query,
  vector <bool>& is_query_connected
) {
  int n = g.size();
  vector <int> color(n, 0);
  int cur_color = 1;

  for (int i = 0; i < n; i++) {
    if (color[i]) continue;
    DFSColor(i, g, color, cur_color);
    cur_color++;
  }

  for (int i = 0; i < query.size(); i++) {
    auto [u, v] = query[i];
    // cout << "is_conn: " << (color[u] == color[v]) << "\n";
    is_query_connected[i] = (color[u] == color[v]);
  }
}

void GetStats(
  vector <vector <Edge> >& g,
  vector <Node>& nodes,
  vector <pair <int, int> >& query,
  vector <int>& k_list,
  int block_size
) {
  vector <vector <Estimator*> > est_blocks;
  vector <string> block_names;

  BuildEstBlocks(est_blocks, block_names, g, nodes, k_list, block_size);

  vector <ll> check_sum(est_blocks.size(), 0);
  vector <ll> count_sum(est_blocks.size(), 0);
  vector <ld> time(est_blocks.size(), 0);

  vector <bool> is_query_connected(query.size(), false);

  CheckQueryConnected(g, query, is_query_connected);

  int all_algo_count = 0;
  for (auto& b: est_blocks) {
    all_algo_count += b.size();
  }

  ll all_query_count = all_algo_count * query.size();
  ProgressBar progress_bar(all_query_count);

  vector <vector <ll> > heap_size(est_blocks.size());

  for (int est_block_idx = 0; est_block_idx < est_blocks.size(); est_block_idx++) {
    ll cur_sum = 0;
    ll cur_count_sum = 0;

    ll cur_block_size = est_blocks[est_block_idx].size();

    const auto start{std::chrono::steady_clock::now()};
    // const auto fb{Fibonacci(42)};
    for (auto est: est_blocks[est_block_idx]) {
      for (int i = 0; i < query.size(); i++) {
        progress_bar.Update();
        if (!is_query_connected[i]) continue;
        auto [u, v] = query[i];

        vector <int> ord_dump;
        auto [cur_dist, cur_count] = AStar(u, v, g, est, ord_dump);

        cur_sum += cur_dist;
        cur_count_sum += cur_count;

        heap_size[est_block_idx].push_back(cur_count);
        // if (est_block_idx == 0) {
        //   cout << heap_size[0].back() << "\n";
        // }
      }
    }
    const auto end{std::chrono::steady_clock::now()};
    const std::chrono::duration<double> elapsed_seconds{end - start};

    check_sum[est_block_idx] += (cur_sum + cur_block_size - 1) / cur_block_size;
    count_sum[est_block_idx] += (cur_count_sum + cur_block_size - 1) / cur_block_size;
    time[est_block_idx] += elapsed_seconds.count() / cur_block_size;
  }

  cout << "|Algorithm | Heap Size | Time | Check Sum |\n";
  cout << "|-|-|-|-|\n";
  for (int i = 0; i < est_blocks.size(); i++) {
    cout << "|" << block_names[i] << "|" << count_sum[i] << "|" << time[i] << "|" << check_sum[i] << "|\n";
  }

  for (int i = 0; i < est_blocks.size(); i++) {
    cout << "[\"" << block_names[i] << "\", " << count_sum[i] << ", " << time[i] << ", " << check_sum[i] << "],\n";
  }

  vector <ld> est_speed_up(est_blocks.size());
  for (int i = 0; i < est_blocks.size(); i++) {
    ll count = 0;
    ld ratio_sum = 0;
    for (int j = 0; j < heap_size[0].size(); j++) {
      if (heap_size[0][j] == 0) {
        ratio_sum += 1;
        continue;
      }
      // cout << heap_size[i][j] << ' ' << heap_size[0][j] << "\n";
      // cout << heap_size[i][j] * 1. / heap_size[0][j] << "\n";
      ratio_sum += heap_size[i][j] * 1. / heap_size[0][j];
      count++;
    }

    est_speed_up[i] = ratio_sum / count;
  }

  cout << setprecision(2);
  cout << fixed;

  cout << "|Algorithm | Speed Up (heap size) |\n";
  cout << "|-|-|-|-|\n";
  for (int i = 0; i < est_blocks.size(); i++) {
    cout << "|" << block_names[i] << "|" << 1. / est_speed_up[i] << "|\n";
  }

  for (int i = 0; i < est_blocks.size(); i++) {
    cout << "[\"" << block_names[i] << "\", " << 1. / est_speed_up[i] << "],\n";
  }
}

int main() {
  srand(time(0));
  vector <vector <Edge> > g;
  vector <Node> nodes;
  GetGraph(g, nodes, "../graph_dump/graph_0.txt");

  // if (CheckConnectivity(g)) {
  //   cout << "is_connected!\n";
  // } else {
  //   cout << "not_connected!\n";
  // }

  vector <pair <int, int> > query;
  // GetQueries(query, nodes.size());
  GetRandomQueries(query, nodes.size(), 100);

  cout << nodes.size() << "\n";
  // GetRandomGraph(g, nodes);
  // Solve(g, nodes, query, true);

  vector <int> k_list = {1, 2, 3, 5, 7, 10, 15, 20, 30, 45};
  // vector <int> k_list = {1, 2, 5, 10};
  int block_size = 5;
  // int block_size = 1;

  GetStats(g, nodes, query, k_list, block_size);
}
