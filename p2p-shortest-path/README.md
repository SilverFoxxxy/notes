## Introduction
Motivated by computing driving directions, the problem of finding point-to-point shortest paths.

There are too many preprocessing-based methods too describe in full—we refer the reader
to [12] (12] D. Delling, P. Sanders, D. Schultes, and D. Wagner. Engineering Route Planning Algorithms. In
J. Lerner, D. Wagner, and K. Zweig, editors, Algorithmics of Large and Complex Networks, volume
5515 of LNCS, pages 117–139. Springer, 2009.) for a comprehensive overview.

We present here only some highlights with the algorithms
that are relevant to our study.

- [**Arc flags**](algorithms/arc-flags.md)
? (+ dynamic)

- **Landmark-based $A^*$ search**

reduce the search space by directing the search towards the goal.

- **Highway hierarchies [26, 27] and**
- **Reach [20, 19]**

use hierarchical properties of road networks to sparsify the search

- **Shortcut**

One of the key ingredients of efficient implementations of these methods is the notion of a shortcut(introduced by Sanders and Schultes [26]), which is a new arc representing a shortest path between its endpoints. Shortcuts by themselves became the basis of
- **Contraction hierarchies (CH) [16]**

an elegant algorithm that motivated much follow-up work. Another method is

- **Transit node routing (TNR) [3, 4]**

which is based on the observation that there is a small set of vertices that cover all sufficiently long shortest paths out of any region of a road network. This allows long-range queries to be reduced to a small number of table look-ups. The most efficient implementation of TNR uses CH during the preprocessing stage and to handle local queries [16]. One can combine goal-direction with hierarchical methods or TNR [19, 6, 5]. In particular

The fastest previously known (2010) point-to-point shortest path algorithm [6] combines elements from TNR (using CH) and arc flags.

It is six orders of magnitude faster than Dijkstra’s algorithm for random (long-range) queries. Although these algorithms have been shown to work well in practice,

A ***theoretical analysis*** has
been given only recently, in a paper by Abraham et al. [1]. It is based on modeling road networks
as graphs with low highway dimension. The method with the best time bounds, presented by
Abraham et al. as a variant of TNR, is actually a labeling algorithm. Labeling algorithms have
been studied before in the distributed computing literature [15, 30]. In particular, [15] gives
upper and lower bounds on label sizes for general graphs, trees, planar graphs, and bounded
degree graphs.
Like most speedup techniques on road networks, the labeling algorithm works in two stages.
The preprocessing stage computes for each vertex v a forward label Lf (v) and a reverse label
Lr(v). The forward label consists of a set of vertices w, together with their respective distances
dist(v, w) from v. Similarly, the reverse label consists of a set of vertices u, each with its distance
dist(u, v) to v. The labels have the following cover property: For every pair of distinct vertices
s and t, Lf (s) ∩ Lr(t) contains a vertex u on a shortest path from s to t. To emphasize that a
labeling has the cover property, we call a labeling valid.
The query stage of the labeling algorithm is quite simple, given the cover property. Given
s and t, find the vertex u ∈ Lf (s) ∩ Lr(t) that minimizes dist(s, u) + dist(u, t) and return the
corresponding path. One can think of a label for a vertex v as a set of hubs to which v has a
direct connection. The cover property ensures that any two vertices share at least one hub on the
shortest path between them.
The results of Abraham et al. [1] suggest that the labeling algorithm is interesting from
a theoretical viewpoint, but leave open the existence of a practical implementation. In fact,
as described in their paper, the algorithm appears impractical. First, preprocessing, although
polynomial-time, is too slow for continent-size networks. Second, the worst-case bound on the

- **Dynamically Maintaining Shortest Path Trees
under Batches of Updates**

Мб отложенными операциями?

https://link.springer.com/chapter/10.1007/978-3-319-03578-9_24

- **Euclidean hub**

- [**Fast fully dynamic landmark-based estimation of shortest path distances in very large graphs**](https://dl.acm.org/doi/abs/10.1145/2063576.2063834)

- [**A learning-based method for computing shortest path distances on road networks**](https://dbgroup.cs.tsinghua.edu.cn/ligl/papers/shortestpath_icde2021.pdf)

Решает задачу неточно, через сжатие графа (?) в пространство низкой размерности (?)

- [**Gamification of the Graph Theory Course. Finding the Shortest Path by a Greedy Algorithm**](https://link.springer.com/chapter/10.1007/978-3-031-35317-8_18)

- [**Dynamic path planning of mobile robot based on improved simulated annealing algorithm**](https://www.sciencedirect.com/science/article/pii/S0016003223000546)

- [**Optimizing Connections: Applied Shortest Path Algorithms for MANETs**]()

- [**A New Deterministic Algorithm for Fully Dynamic All-Pairs Shortest Paths**]

- [**A Distributed Solution for Efficient K Shortest Paths Computation Over Dynamic Road Networks**](https://ieeexplore.ieee.org/abstract/document/10378872)

Выбор landmarks как оптимизационная задача.
