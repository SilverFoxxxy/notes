#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>
#include <fstream>
#include <chrono>
#include <iomanip>

using namespace std;

#define ll long long
#define ld long double
#define F first
#define S second

const int N = 15;
const int MAX_ITER = 1e9;
const int GRAPH_COUNT = 1;

struct Edge {
    int v;
    ll w;
};

struct EdgeInfo {
    int u;
    int v;
    ll w;
};

bool EdgeInfoComp(
    EdgeInfo& a,
    EdgeInfo& b
) {
    return a.w < b.w;
}

struct Node {
    ll x;
    ll y;
};

struct Graph {
    vector <vector <Edge> > g;
    vector <Node> nodes;
    vector <EdgeInfo> sorted_edges;

    void Sort() {
        for (auto& gu: g) {
            sort(gu.begin(), gu.end(), [](const Edge& a, const Edge& b) {
                return (a.w < b.w);
            });
        }
    }
};

class DSU {
public:
  DSU(int n);

  int Get(int u);

  bool Unite(int u, int v);

private:
  int n = 0;
  vector <int> p;
  vector <int> y;
};

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

void GetRandomGraph(
    vector <vector <Edge> >& g,
    vector <Node>& nodes,
    vector <EdgeInfo>& edges
) {
    int n = N;

	g.resize(n, vector <Edge> (n));
	nodes.resize(n);

    for (int u = 0; u < n; u++) {
        for (int v = u + 1; v < n; v++) {
            ll w = rand() % 100000;
            g[u][v] = {v, w};
            g[v][u] = {u, w};
            edges.push_back({u, v, w});
        }
    }

    sort(edges.begin(), edges.end(), EdgeInfoComp);
}

ll TSPBasic(
    int i, // iteration
    int u, // last vertex
    vector <int>& p,
    vector <bool>& used,
    vector <vector <Edge> >& g,
    ll d,
    ll& timer
) {
    timer++;
    if (timer > MAX_ITER) {
        return -1;
    }
    int n = p.size();
    if (i == n) {
        return d;
    }
    ll ans = 1e9;
    for (auto [v, w]: g[u]) {
        if (used[v]) continue;
        used[v] = true;
        p[i] = v;
        ans = min(ans, TSPBasic(i + 1, v, p, used, g, d + w, timer));
        used[v] = false;
    }
    return ans;
}

ll TSP(
    int i, // iteration
    int u, // last vertex
    vector <int>& p,
    vector <bool>& used,
    vector <vector <Edge> >& g,
    ll d,
    ll& timer,
    ll& cur_ans
) {
    if (d >= cur_ans) {
        return d;
    }
    timer++;
    if (timer > MAX_ITER) {
        return -1;
    }
    int n = p.size();
    if (i == n) {
        cur_ans = min(cur_ans, d);
        return d;
    }
    ll ans = 1e9;
    for (auto [v, w]: g[u]) {
        if (used[v]) continue;
        used[v] = true;
        p[i] = v;
        ans = min(ans, TSP(i + 1, v, p, used, g, d + w, timer, cur_ans));
        used[v] = false;
    }
    return ans;
}

ll PredictMST(
    int u,
    vector <bool>& used,
    vector <vector <Edge> >& g,
    vector <EdgeInfo>& sorted_edges
) {
    int count = 0;

    DSU dsu(g.size());

    ll dsu_w = 1e9;
    for (int v = 0; v < g.size(); v++) {
        if (!used[v]) {
            dsu_w = min(dsu_w, g[u][v].w);
        }
    }

    int n = 0;
    for (int i = 0; i < used.size(); i++) {
        n += !used[i];
    }
    for (int i = 0; i < sorted_edges.size() && count < n - 1; i++) {
        auto [u, v, w] = sorted_edges[i];
        if (used[u] || used[v]) continue;
        if (dsu.Unite(u, v)) {
            dsu_w += w;
            count++;
        }
    }

    return dsu_w;
}

ll TSPMST(
    int i, // iteration
    int u, // last vertex
    vector <int>& p,
    vector <bool>& used,
    vector <vector <Edge> >& g,
    vector <EdgeInfo>& sorted_edges, // must be sorted
    ll d,
    ll& timer,
    ll& cur_ans,
    ll last_mst = 1e9
) {
    if (d + last_mst >= cur_ans) {
        last_mst = PredictMST(u, used, g, sorted_edges);
        if (d + last_mst >= cur_ans) {
            return cur_ans;
        }
    }

    timer++;
    if (timer > MAX_ITER) {
        return -1;
    }
    int n = p.size();
    if (i == n) {
        cur_ans = min(cur_ans, d);
        return d;
    }
    ll ans = 1e9;
    for (auto [v, w]: g[u]) {
        if (used[v]) continue;
        used[v] = true;
        p[i] = v;
        ans = min(ans, TSPMST(i + 1, v, p, used, g, sorted_edges, d + w, timer, cur_ans, last_mst));
        used[v] = false;
    }
    return ans;
}

template <typename T>
void Shuffle(
    T begin,
    T end
) {
    size_t size = end - begin;

    for (size_t i = 0; i < size; i++) {
        swap(begin[i], begin[rand() % size]);
    }
}

// ll TSPMSTRandom(
//     int i, // iteration
//     int u, // last vertex
//     vector <int>& p,
//     vector <bool>& used,
//     vector <vector <Edge> >& g,
//     vector <EdgeInfo>& sorted_edges, // must be sorted
//     ll d,
//     ll& timer,
//     ll& cur_ans,
//     ll last_mst = 1e9
// ) {
//     if (d + last_mst >= cur_ans) {
//         last_mst = PredictMST(u, used, g, sorted_edges);
//         if (d + last_mst >= cur_ans) {
//             return cur_ans;
//         }
//     }

//     timer++;
//     if (timer > 1e8) {
//         return -1;
//     }
//     int n = p.size();
//     if (i == n) {
//         cur_ans = min(cur_ans, d);
//         return d;
//     }
//     ll ans = 1e9;

//     Shuffle(g[u].begin(), g[u].end());

//     for (auto [v, w]: g[u]) {
//         if (used[v]) continue;
//         used[v] = true;
//         p[i] = v;
//         ans = min(ans, TSPMSTRandom(i + 1, v, p, used, g, sorted_edges, d + w, timer, cur_ans, last_mst));
//         used[v] = false;
//     }
//     return ans;
// }

void Solve() {
    vector <Graph> graphs;

    vector <ld> time(4, 0);

    int q = GRAPH_COUNT;
    for (int i = 0; i < q; i++) {
        graphs.push_back({});
        auto& g = graphs.back();
        GetRandomGraph(g.g, g.nodes, g.sorted_edges);
    }

    // cout << graphs.size() << "\n";

    // bool mst_flag = false;
    bool slow_flag = false;
    // bool cut_flag = false;
    bool mst_flag = true;
    // bool slow_flag = true;
    bool cut_flag = true;
    bool sorted_flag = false;
    bool sorted_mst_flag = true;

    cout << setprecision(3);
    cout << fixed;

    if (mst_flag) {
        const auto start{std::chrono::steady_clock::now()};
        for (auto& graph: graphs) {
            int n = graph.nodes.size();
            vector <int> p(n, 0);
            vector <bool> used(n, false);
            used[0] = 1;

            ll timer_tsp = 0;

            ll cur_ans = 1e9;
            ll ans = TSPMST(1, 0, p, used, graph.g, graph.sorted_edges, 0, timer_tsp, cur_ans);
            cout << ans << "\n";

            // cout << ans << " | t_mst = " << timer_tsp << "\n";
        }
        const auto end{std::chrono::steady_clock::now()};

        const std::chrono::duration<double> elapsed_seconds{end - start};
        time[0] = elapsed_seconds.count();

        cout << "time mst: " << elapsed_seconds.count() << "\n";
    }

    if (cut_flag) {
        const auto start{std::chrono::steady_clock::now()};
        bool flag_tl = false;
        for (auto& graph: graphs) {
            int n = graph.nodes.size();
            vector <int> p(n, 0);
            vector <bool> used(n, false);
            used[0] = 1;

            ll timer_tsp = 0;

            ll cur_ans = 1e9;
            ll ans = TSP(1, 0, p, used, graph.g, 0, timer_tsp, cur_ans);

            const auto end{std::chrono::steady_clock::now()};
            const std::chrono::duration<double> elapsed_seconds{end - start};
            if (elapsed_seconds.count() > 1) {
                flag_tl = true;
                break;
            }

            cout << ans << "\n";

            // cout << ans << " | t_cut = " << timer_tsp << "\n";
        }
        const auto end{std::chrono::steady_clock::now()};

        const std::chrono::duration<double> elapsed_seconds{end - start};
        time[1] = elapsed_seconds.count();

        cout << "time cut: " << (flag_tl ? -1 : elapsed_seconds.count()) << "\n";
    }

    if (slow_flag) {
        const auto start{std::chrono::steady_clock::now()};
        bool flag_tl = false;
        for (auto& graph: graphs) {
            int n = graph.nodes.size();
            vector <int> p(n, 0);
            vector <bool> used(n, false);
            used[0] = 1;

            ll timer_tsp = 0;

            ll cur_ans = 1e9;
            ll ans = TSPBasic(1, 0, p, used, graph.g, 0, timer_tsp);

            // cout << ans << " | t0 = " << timer_tsp << "\n";
            const auto end{std::chrono::steady_clock::now()};
            const std::chrono::duration<double> elapsed_seconds{end - start};
            if (elapsed_seconds.count() > 1) {
                flag_tl = true;
                break;
            }
        }
        const auto end{std::chrono::steady_clock::now()};

        const std::chrono::duration<double> elapsed_seconds{end - start};
        time[2] = elapsed_seconds.count();

        cout << "time slow: " << (flag_tl ? -1 : elapsed_seconds.count()) << "\n";
    }

    for (auto& graph: graphs) {
        graph.Sort();
    }

    if (sorted_flag) {
        const auto start{std::chrono::steady_clock::now()};
        bool flag_tl = false;
        for (auto& graph: graphs) {
            int n = graph.nodes.size();
            vector <int> p(n, 0);
            vector <bool> used(n, false);
            used[0] = 1;

            ll timer_tsp = 0;

            ll cur_ans = 1e9;

            ll ans = TSP(1, 0, p, used, graph.g, 0, timer_tsp, cur_ans);

            const auto end{std::chrono::steady_clock::now()};
            const std::chrono::duration<double> elapsed_seconds{end - start};
            if (elapsed_seconds.count() > 1) {
                flag_tl = true;
                break;
            }

            // cout << ans << " | t_cut = " << timer_tsp << "\n";
        }
        const auto end{std::chrono::steady_clock::now()};

        const std::chrono::duration<double> elapsed_seconds{end - start};
        time[1] = elapsed_seconds.count();

        cout << "time sorted: " << (flag_tl ? -1 : elapsed_seconds.count()) << "\n";
    }

    if (sorted_mst_flag) {
        const auto start{std::chrono::steady_clock::now()};
        for (auto& graph: graphs) {
            int n = graph.nodes.size();
            vector <int> p(n, 0);
            vector <bool> used(n, false);
            used[0] = 1;

            ll timer_tsp = 0;

            ll cur_ans = 1e9;
            ll ans = TSPMST(1, 0, p, used, graph.g, graph.sorted_edges, 0, timer_tsp, cur_ans);
            cout << ans << "\n";

            // cout << ans << " | t_mst = " << timer_tsp << "\n";
        }
        const auto end{std::chrono::steady_clock::now()};

        const std::chrono::duration<double> elapsed_seconds{end - start};
        time[0] = elapsed_seconds.count();

        cout << "time sorted mst: " << elapsed_seconds.count() << "\n";
    }

    cout << "\n";


    // {
    //     int n = nodes.size();
    //     vector <int> p(n, 0);
    //     vector <bool> used(n, false);
    //     used[0] = 1;

    //     ll timer_tsp = 0;

    //     ll cur_ans = 1e9;
    //     ll ans = TSPMSTRandom(1, 0, p, used, g, sorted_edges, 0, timer_tsp, cur_ans);

    //     cout << ans << " | t_mst_rand = " << timer_tsp << "\n";
    // }
    // solve();
}

int main() {
    int t = 100;
    while (t--) {
        Solve();
    }
}
