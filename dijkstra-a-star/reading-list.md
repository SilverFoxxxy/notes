https://scholar.google.com/scholar?start=10&q=shortest+path+landmarks&hl=en&as_sdt=0,5&as_ylo=2023

https://ieeexplore.ieee.org/abstract/document/10378872

https://www.tandfonline.com/doi/abs/10.1080/13875868.2022.2130330

https://ojs.aaai.org/index.php/SOCS/article/view/27288

https://ojs.aaai.org/index.php/SOCS/issue/view/565

https://ieeexplore.ieee.org/abstract/document/9458665

https://dbgroup.cs.tsinghua.edu.cn/ligl/papers/shortestpath_icde2021.pdf

https://dl.acm.org/doi/abs/10.1145/3448016.3452826

https://www.sciencedirect.com/science/article/abs/pii/S0016003223000546

https://link.springer.com/chapter/10.1007/978-3-031-35317-8_18

https://link.springer.com/chapter/10.1007/978-3-031-10596-8_3

https://epubs.siam.org/doi/abs/10.1137/22M1523790

https://arxiv.org/pdf/2101.11565

https://scholar.google.com/scholar?as_ylo=2024&q=shortest+path+social+network&hl=en&as_sdt=0,5

https://publisher.uthm.edu.my/ojs/index.php/ijie/article/view/18232

https://arxiv.org/abs/2410.10613

https://onlinelibrary.wiley.com/doi/full/10.1155/2024/1592587

https://epubs.siam.org/doi/abs/10.1137/22M1523790

https://link.springer.com/article/10.1007/s43069-024-00347-3

https://bojie-shen.com/files/AAAI-Goldi.pdf

https://ieeexplore.ieee.org/abstract/document/10650344

https://www.microsoft.com/en-us/research/wp-content/uploads/2010/12/HL-TR.pdf

arc-flags
https://www.researchgate.net/profile/Rolf-Moehring/publication/221131640/figure/fig1/AS:651517329670145@1532345244334/n-the-arc-flag-method-at-each-arc-a-a-vector-fa-of-arc-flags-is-stored-such-that-fai.png

https://link.springer.com/chapter/10.1007/978-3-319-03578-9_24

https://scholar.google.com/scholar?q=point+to+point+shortest+landmarks&hl=en&as_sdt=0,5

https://dl.acm.org/doi/abs/10.1145/2063576.2063834


Here are key datasets for testing graph algorithms with social network friend lists:

**1. Friendster Dataset**
- **117.7M nodes**, **2.58B directed edges** (friendships)
- Includes user-group membership data (38.7M edges)
- Directed relationships (asymmetric friendships)
- Available via ArchiveTeam[5]

**2. Facebook (SNAP/WOSN)**
- Stanford's dataset with **63K nodes**, **817K undirected edges**
- Represents mutual friendships
- Includes anonymized user features[1][4]

**3. Twitter Friends**
- Directed follower relationships
- Smaller scale than Friendster[2]

**Key considerations**:
- For large-scale tests (>1B edges), use Friendster
- For undirected graphs, choose Facebook datasets
- Twitter/Friendster allow testing directional algorithms

All datasets are publicly available through linked sources. Would you like guidance on preprocessing these for specific algorithm types?

Citations:
[1] https://snap.stanford.edu/data/ego-Facebook.html
[2] https://www.kaggle.com/datasets/hwassner/TwitterFriends
[3] https://snap.stanford.edu/data/
[4] https://networkrepository.com/socfb-wosn-friends.php
[5] https://wiki.archiveteam.org/index.php/Friendster_dataset
[6] https://s2.smu.edu/~egabrielsen/cs1342/program5.pdf
[7] https://www.nebula-graph.io/posts/social-networks-with-graph-database-1
[8] https://github.com/ManasiRaje/Graph-and-Network-Flows---Friendship-Graphs
[9] https://data4goodlab.github.io
[10] https://paperswithcode.com/dataset/orkut
[11] https://github.com/benedekrozemberczki/datasets
[12] https://www.kaggle.com/code/alireza151/friends-network-and-communities
[13] https://datascience.stackexchange.com/questions/422/publicly-available-social-network-datasets-apis
[14] https://github.com/melaniewalsh/sample-social-network-datasets
[15] https://paperswithcode.com/dataset/snap
[16] https://paperswithcode.com/datasets?task=graph-clustering
[17] https://github.com/stevenrouk/social-network-graph-analysis
[18] https://www.ll.mit.edu/sites/default/files/publication/doc/social-network-analysis-content-graphs-campbell-ja-22727.pdf
[19] https://assets.ctfassets.net/btheynltg5cn/53nweIQ8YcXmh9uRxAssqi/e43c35ef60dd613fe25689ca55e35ac9/GrafRank.WWW.21.pdf
[20] https://dev.to/rajarakshak/social-network-analysis-using-graph-databases-2pi

---
Answer from Perplexity: pplx.ai/share





Ниже приводится оценка вашей идеи: кластеризовать пользователей «по дружбе», чтобы запрос о друзьях одного юзера обрабатывался на меньшем числе серверов, а также использование «опорных точек» для ускорения поиска кратчайших путей.

Вкратце: ваша гипотеза опирается на два направления — разбиение графа для минимизации межшардовых запросов и выбор опорных точек (landmarks) для эффективного поиска путей. Оба подхода широко изучены, и существующие методы (METIS-шардинг, community detection, ALT-алгоритм) могут дать более предсказуемые и сбалансированные результаты, чем простые жадные эвристики. Ниже — подробный разбор с рекомендациями по улучшению.

Фон: шардинг и партиционирование графов

Шардинг и распределённые графовые БД
Распределённая графовая БД хранит вершины и рёбра на разных узлах кластера, что даёт высокую масштабируемость и отказоустойчивость
puppygraph.com
.
Методы партиционирования включают hash-based и METIS-алгоритм, последний оптимизирует число рёбер между шардами (edge-cut)
openproceedings.org
.
В современных графовых СУБД (e.g. JanusGraph, NebulaGraph) распространены гибридные схемы, сочетающие репликацию и шардинг, чтобы балансировать задержку и загрузку
nebula-graph.io
.
Кластеризация социальных графов
Community detection (детектирование сообществ) ищет плотные подграфы, минимизируя рёбра между ними — прямо отвечает на задачу уменьшения межсерверных запросов
pmc.ncbi.nlm.nih.gov
.
Классические алгоритмы: Fast Greedy Modularity
Nature
, Label Propagation, Spectral Clustering (с последующим k-means)
SpringerLink
.
Network Embedding (node2vec, DeepWalk) проецирует вершины в векторное пространство, где кластеризация становится задачей обычного k-means
ScienceDirect
.
Оценка вашей гипотезы

1. Кластеризация по «дружбе» vs. Community Detection
Жадный подход «берём следующего пользователя с максимальным числом друзей из уже выбранных» похож на локальный greedy метод, но он рискует дать неравномерные шарды и большой edge-cut в гетерогенных регионах графа
Memgraph
.
METIS и Scotch обеспечивают баланс нагрузки и минимальный edge-cut благодаря глобальной оптимизации, а не локальной жадности
MIT Database Group
.
Эксперименты на аналогичных датасетах (Facebook, Twitter) показали, что community detection алгоритмы дают меньший cross-shard traffic, чем жадные эвристики
5harad.com
.
2. Выбор опорных точек (landmarks) для кратчайших путей
ALT-алгоритм: предвычислить расстояния от каждой вершины до набора landmarks; во время запроса использовать нижнюю оценку через triangle inequality
cs.princeton.edu
.
Существует эвристика Farthest-point sampling (далёкие точки), когда следующая landmark — та, у которой максимальная сумма/минимальное расстояние до уже выбранных
faculty.cc.gatech.edu
.
Эмпирические сравнения показали, что отбор landmark по ранжированию по покрытию (какие дают наилучший bound в большом числе пар) эффективнее полностью случайного отбора
citeseerx.ist.psu.edu
.
3. Объединение подходов
Пакетная кластеризация + ALT-ускорение: сначала разбиваем граф на «локальные» шарды (communities), внутри каждого поддерживаем свои landmarks для внутрипартиционной маршрутизации, а запросы по краям маршрутов используют межшардовую координацию
cs.utexas.edu
.
В Dynamic Partitioning (Hermes) возможна адаптивная рекалибровка шардинга на основе текущего трафика запросов, что снизит перекладывание соседних сообществ
uwaterloo.ca
.
Рекомендации по улучшению


A. Используйте проверенные алгоритмы community detection
METIS или KaHIP для начального разбиения с учётом баланса размеров и минимизации edge-cut
MIT Database Group
.
Локальная оптимизация: переназначайте вершины на шард, если они имеют большую часть соседей в другом шарде
openproceedings.org
.
B. Внедрите ALT-алгоритм для shortest-path
Выбор landmarks: сначала рандомные, затем farthest-point sampling для увеличения разброса
faculty.cc.gatech.edu
.
Динамический отбор: при запуске запросов оставляйте в наборе только активные landmarks, улучшающие bound для данного s-t
eplus.uni-salzburg.at
.
C. Оценка качества
Cross-shard query ratio: доля запросов, затрагивающих >1 сервер.
Edge-cut: число рёбер, пересекающих кластеры.
Load balance: равномерность размеров шардов.
Выводы

Жадный greedy-подход может дать быстрые наброски, но для реальных социальных графов лучше применять сообщественные алгоритмы (METIS, spectral, label propagation) для устойчивых результатов.
ALT-алгоритм с разумным отбором landmarks даст эффективные эвристики для shortest-path и снизит число операций обхода.
Гибридные схемы (глобальное разбиение + локальная оптимизация + динамический пересчёт) сейчас считаются промышленным стандартом для масштабирования графовых систем.
В качестве упражнения можно сравнить ваш greedy-алгоритм с METIS/Scotch и ALT-ALT на ваших датасетах, измерив перечисленные метрики.
Таким образом, идея заслуживает внимания как учебное упражнение, но для production-решения рекомендуются методы, проверенные в академических исследованиях и в индустрии.
