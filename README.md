# notes

[Технологии программирования - семинары](tp-seminars)
=======
## Темы

[?????]()

### Отложенные темы
[Эвристики A* (в неор. графе с неотр. весами)](dijkstra-a-star/README.md)
- Уже придумали до нас: [статья](dijkstra-a-star/files/EPP%20shortest%20path%20algorithms.pdf)

- Потенциал: улучшать

- Можно поисследовать расстановку landmarks
- Поисследовать расстановку landmarks в других видах графов

[A Hub-Based Labeling Algorithm for
Shortest Paths on Road Networks, December 2010](https://www.microsoft.com/en-us/research/wp-content/uploads/2010/12/HL-TR.pdf)
Упоминаются arc-flags, landmark-based,

- Reaches, shortcuts

- Hotpath - эвристика. Будем отмечать частоту появления рёбер (вершин) в кратчайших путях (или в процессах поиска). Частоту учтём в приоритетах.

[Многомерное дерево Фенвика в условиях ограниченной памяти](nd-fenwick/README.md)
- Возможно, решает те же проблемы, что и BTree или KDTree
- Потенциал: сказать что придумано что-то крутое, если в таком формате ещё не было сделано

[Dynamic TSP](dtsp/README.md)
- Не особо ускоряется, максимум обзорную статью писать
