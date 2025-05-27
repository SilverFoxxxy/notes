The strip packing problem is a classical two-dimensional geometric optimization problem where a set of axis-aligned rectangular items must be packed without overlap into a strip of fixed width and infinite height, with the goal of minimizing the total height used[2]. This problem is strongly NP-hard and has practical applications in industries like paper cutting, logistics, and manufacturing[7][9].

### Problem Definition
- Given a strip with fixed width and infinite height.
- Pack a set of rectangles orthogonally (no rotation allowed in many variants).
- Minimize the height of the strip needed to contain all rectangles without overlap.

### Current Approaches

**1. Exact Algorithms**
- Branch-and-bound (B&B) methods are widely used for exact solutions, often enhanced with reduction procedures, lower and upper bounds, and dynamic programming techniques to prune the search space[5][7].
- The *Positions and Covering* methodology generates valid packing positions and uses set-covering formulations to find optimal configurations, effective for small to medium-sized instances[5].
- Mixed-Integer Linear Programming (MILP) models have been developed, combining discrete and continuous spatial representations to efficiently solve the problem and adapt to industrial constraints[9].
- Exact algorithms are computationally expensive and typically limited to smaller problem sizes.

**2. Heuristic and Metaheuristic Methods**
- Greedy heuristics like Bottom-Left (BL), Bottom-Left Fit (BLF), and Bottom-Left-Decreasing (BLD) place rectangles by sorting them and fitting them in the lowest and leftmost positions possible[6][8].
- BLD* is a stochastic variant of BLD that tries multiple randomized orderings and has shown improved results over classical heuristics in short computational times[8].
- Hyperheuristics manage sequences of low-level heuristics and use hill-climbing to improve solutions, providing effective and efficient solutions for larger instances[6].

**3. Hybrid and Interactive Approaches**
- Hybrid spatial representation models combine discrete and continuous representations along the strip's width and height, respectively, to balance solution quality and computational effort[9].
- Interactive systems that combine human reasoning with algorithmic speed have been shown to produce near-optimal solutions faster than automated methods alone[8].

### Summary Table of Approaches

| Approach Type           | Key Features                                               | Strengths                              | Limitations                         |
|------------------------|------------------------------------------------------------|--------------------------------------|-----------------------------------|
| Exact Algorithms       | Branch-and-bound, MILP, Positions and Covering             | Optimal solutions, theoretical bounds| Computationally expensive, limited to small/medium instances |
| Heuristics             | Bottom-Left, BLD, BLD*, Hyperheuristics                    | Fast, scalable, good quality          | No guarantee of optimality         |
| Hybrid/Interactive     | Combined discrete/continuous models, human-in-the-loop     | Adaptable, balances quality and speed | Requires user input or complex modeling |

### Research Trends and Challenges
- Improving exact algorithms to handle larger instances more efficiently[5][7].
- Designing heuristics that balance solution quality and computational time, including stochastic and hyperheuristic methods[6][8].
- Developing flexible MILP models that can incorporate additional practical constraints beyond classical definitions[9].
- Leveraging interactive approaches to combine computational power with human intuition[8].

In conclusion, the strip packing problem remains a challenging optimization problem with a rich variety of solution methods ranging from exact algorithms to heuristics and hybrid models. The choice of approach depends on problem size, required solution quality, and computational resources[1][9].

Citations:
[1] https://www.sciencedirect.com/science/article/pii/S0952197608001826
[2] https://en.wikipedia.org/wiki/Strip_packing_problem
[3] https://kpfu.ru/portal/docs/F233516594/e04_12.pdf
[4] https://www.mdpi.com/2076-3417/12/4/1965
[5] https://journals.plos.org/plosone/article?id=10.1371%2Fjournal.pone.0245267
[6] http://www-sop.inria.fr/members/Ignacio.Araya/publis/iaraya-HSPpaper.pdf
[7] https://pubsonline.informs.org/doi/10.1287/opre.1100.0833
[8] https://www.eecs.harvard.edu/~michaelm/postscripts/ijcai2003.pdf
[9] https://egon.cheme.cmu.edu/Papers/CastroGrossmannStripPacking.pdf

---
Answer from Perplexity: pplx.ai/share
