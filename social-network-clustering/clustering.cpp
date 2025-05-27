#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <unordered_set>
#include <random>

using namespace std;

const int MAX_PAGE = 5;

void ParseSOCFB(
    const string& filename,
    vector <vector <int> >& g
) {
    ifstream fin;
    fin.open(filename);

    /*
    % sym unweighted
    % 1269502 63731 63731
    */

    string buf;
    fin >> buf >> buf >> buf;
    int n, m;
    fin >> buf >> m >> n >> n;

    cout << "graph n, m : " << n << ' ' << m << "\n";

    g.resize(n);

    for (int i = 0; i < m; i++) {
        int u, v;
        int w, t;
        fin >> u >> v >> w >> t;
        u--, v--;

        // cout << u << ' ' << v << "\n";

        g[u].push_back(v);
        g[v].push_back(u);
    }
}

bool TryParseInt(
    const string& s,
    int& i
) {
    try
    {
        i = stoi(s);
        return true;
    }
    catch (std::invalid_argument const& ex)
    {
        return false;
    }
    catch (std::out_of_range const& ex)
    {
        return false;
    }
    catch (...) {
        return false;
    }
}

void ParseFriendsterLine(
    const string& line,
    vector <vector <int> >& g
) {
    int i = line.find(':');

    if (i == std::string::npos) {
        return;
    }

    string id_str = line.substr(0, i);
    string friend_list_str = line.substr(i + 1);

    int id = stoi(id_str);

    vector <int> friend_list;

    string delim = ",";
    auto start = 0U;
    auto end = friend_list_str.find(delim);
    while (end != std::string::npos)
    {
        string friend_id_str = friend_list_str.substr(start, end - start);

        int friend_id;
        if (TryParseInt(friend_id_str, friend_id)
            && friend_id < MAX_PAGE * 1'000'000) {
            g[id - 1].push_back(friend_id - 1);
        }
        start = end + delim.length();
        end = friend_list_str.find(delim, start);
    }

    int friend_id;
    string friend_id_str = friend_list_str.substr(start, end);
    if (TryParseInt(friend_id_str, friend_id)
        && friend_id < MAX_PAGE * 1'000'000) {
        g[id - 1].push_back(friend_id - 1);
    }

    if ((id - 1) % 1000000 == 0) {
        cout << id << ' ' << g[id - 1].size() << "\n";
    }
}

void ParseFriendster(
    const string& path,
    vector <vector <int> >& g
) {
    vector <string> filenames;

    g.resize(125000000);

    {
        ifstream fin;
        // cout << path + "list_of_files.txt" << "\n";
        fin.open(path + "list_of_files.txt");
        string filename;
        while (fin >> filename) {
            // cout << "HERE!\n";
            filenames.push_back(filename);
            // cout << filename << "\n";
        }
        fin.close();
    }

    int count = 0;

    for (auto& filename: filenames) {
        count++;
        if (count >= MAX_PAGE) break;
        ifstream fin;
        fin.open(path + filename);

        string line;
        while (fin >> line) {
            ParseFriendsterLine(line, g);
        }

        fin.close();
    }
}

void ClusterizeGreedyOrder(
    const vector <vector <int> >& g,
    vector <int>& greedy_order
) {
    int n = g.size();
    greedy_order.resize(0);
    greedy_order.assign(n, 0);

    vector <int> freq(n, 0);
    set <pair <int, int> > freq_vertex;
    for (int u = 0; u < n; u++) {
        freq_vertex.insert({0, u});
    }

    for (int _ = 0; _ < n; _++) {
        if (_ % 20000 == 0) cout << "clusterize_greedy: " << freq_vertex.size() << ' ' << _ << "\n";
        auto it = --freq_vertex.end();

        // cout << it->first << ' ' << it->second << "\n";

        int u = it->second;
        freq_vertex.erase({freq[u], u});
        greedy_order[u] = _ + 1;
        for (int v: g[u]) {
            if (greedy_order[v]) {
                continue;
            }
            freq_vertex.erase({freq[v], v});
            freq[v]++;
            freq_vertex.insert({freq[v], v});
        }
    }
}

void ClusterizeGreedy(
    const vector <int>& greedy_order,
    vector <int>& colors,
    int k /* cluster size */
) {
    int n = greedy_order.size();
    colors.resize(0);
    colors.assign(n, 0);

    for (int i = 0; i < n; i++) {
        colors[i] = greedy_order[i] / k + 1;
    }
}

void ClusterizeRandom(
    const vector <vector <int> >& g,
    vector <int>& colors,
    int k /* cluster size */
) {
    int color_count = g.size() / k + 1;
    int n = g.size();
    colors.resize(0);
    colors.assign(n, 0);

    for (int i = 0; i < n; i++) {
        colors[i] = rand() % color_count + 1;
    }
}

void Solve(const vector <vector <int> >& g) {
    vector <int> k_array;
    k_array.push_back(1);
    {
        int k = 10;
        while (k <= g.size()) {
            k_array.push_back(k);
            k *= 1.5;
        }
    }

    vector <int> greedy_order;
    ClusterizeGreedyOrder(g, greedy_order);

    for (int k: k_array) {
        cout << "k = " << k << "\n";
        vector <int> colors(g.size());
        ClusterizeGreedy(greedy_order, colors, k);
        vector <int> colors_random(g.size());
        ClusterizeRandom(g, colors_random, k);

        // check metrics
        int sum_colors = 0;
        int sum_edges = 0;
        int sum_random_colors = 0;
        for (int u = 0; u < g.size(); u++) {
            unordered_set <int> color_set;
            unordered_set <int> color_random_set;
            for (int v: g[u]) {
                color_set.insert(colors[v]);
                color_random_set.insert(colors_random[v]);
            }
            sum_random_colors += color_random_set.size();
            sum_colors += color_set.size();
            sum_edges += g[u].size();
        }

        cout << setprecision(2);
        cout << fixed;

        cout << k << ' ' << sum_edges * 1.0 / g.size() << ' ' << sum_colors * 1.0 / g.size() << ' ' << sum_random_colors * 1.0 / g.size() << ' ' << g.size() * 1. / k << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(0);
    // vector <vector <int> > g;
    // ParseSOCFB("data/socfb-wosn-friends.edges", g);
    // Solve(g);

    vector <vector <int> > g;
    g.clear();
    ParseFriendster("data/friendster/", g);
    g.resize(MAX_PAGE * 1'000'000);
    Solve(g);
}
