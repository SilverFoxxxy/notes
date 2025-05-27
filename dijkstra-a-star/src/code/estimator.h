#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

#define ll long long
#define ld long double
#define F first
#define S second

struct Edge {
  int v;
  ll w;
};

struct Node {
  ll x;
  ll y;
  ld x0;
  ld y0;
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

class Estimator {
public:
  Estimator();

  Estimator(
    vector <vector <Edge> >& g
  );

  virtual ll Predict(int u, int v);

  virtual void Dump(std::ofstream& wfile);
};

class MSTEstimator : public Estimator {
public:
  MSTEstimator(vector <vector <Edge> >& g);

  ll Predict(int u, int v);

private:
  void DFS(
    int u,
    vector <vector <pair <int, ll> > >& tree,
    int p = -1
  );

  vector <vector <pair <int, ll> > > binup;
  vector <int> depth;
};

class DijkstraEstimator : public Estimator {
public:
  DijkstraEstimator(
    vector <vector <Edge> >& g,
    vector <Node>& nodes,
    int k = 10
  );

  virtual ll Predict(int u, int v);

  void Dump(std::ofstream& wfile);

  vector <int>& GetStartPoints();

private:
  void Dijkstra(
    int s,
    vector <vector <Edge> >& g,
    vector <ll>& dist
  );

  vector <vector <ll> > dist;
  int k;
  vector <int> start_points;
  vector <Node> start_points_data;
};

// class KEstimator: public DijkstraEstimator {
// public:
//   KEstimator() {

//   }

//   ll Predict(int u, int v);
// };

class EuclideanEstimator: public Estimator {
public:
  EuclideanEstimator(vector <Node>& nodes);

  ll Predict(int u, int v);

private:
  vector <Node>& nodes;
};

class EstimatorList: public Estimator {
public:
  EstimatorList(vector <Estimator*> est);

  ll Predict(int u, int v);

private:
  vector <Estimator*> est;
};
