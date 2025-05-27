The **strip packing problem (SPP)** is a classic optimization problem in combinatorial geometry and operations research. The goal is to pack a set of rectangular items into a **fixed-width, semi-infinite strip** such that the **height of the packed strip is minimized** while ensuring no overlapping items and no rotations (unless allowed).

### **Problem Definition**
- **Input**:
  - A strip of fixed width \( W \) and infinite height.
  - A set of \( n \) rectangles, each with width \( w_i \leq W \) and height \( h_i \).
- **Objective**:
  - Pack all rectangles orthogonally (without rotation unless specified) into the strip, minimizing the total used height \( H \).

### **Variants of the Problem**
1. **Orthogonal Packing**: Rectangles must be placed axis-aligned (no rotations).
2. **Rotations Allowed**: Rectangles can be rotated by 90°.
3. **Guillotine Cutting**: Items must be obtainable via a sequence of edge-to-edge cuts (common in industrial cutting).
4. **Online Strip Packing**: Items arrive sequentially and must be packed immediately without future knowledge.

---

## **Current Approaches to Solve Strip Packing**

### **1. Exact Methods**
- **Integer Linear Programming (ILP)**: Formulates the problem using binary variables for positions and constraints to prevent overlaps.
- **Branch-and-Bound (B&B)**: Explores possible packings systematically, pruning suboptimal branches.
- **Constraint Programming (CP)**: Uses domain reduction and backtracking to find feasible solutions.

*Limitations*: Computationally expensive for large instances (NP-Hard problem).

### **2. Heuristic and Metaheuristic Approaches**
#### **Constructive Heuristics**
- **First-Fit Decreasing Height (FFDH)**: Sorts items by height and places each in the leftmost feasible position.
- **Next-Fit Decreasing Height (NFDH)**: Stacks items in levels, starting a new level if the current item doesn't fit.
- **Best-Fit Decreasing Height (BFDH)**: Places each item in the tightest possible position.

#### **Metaheuristics**
- **Genetic Algorithms (GA)**: Evolves packing solutions using crossover, mutation, and selection.
- **Simulated Annealing (SA)**: Iteratively improves solutions by accepting worse moves probabilistically.
- **Tabu Search (TS)**: Avoids revisiting solutions using a short-term memory (tabu list).

### **3. Approximation Algorithms**
- **Shelf Algorithms** (e.g., NFDH, FFDH): Provide worst-case guarantees (e.g., NFDH gives \( H \leq 2 \cdot OPT + h_{max} \)).
- **Steinberg’s Algorithm**: Guarantees \( H \leq 2 \cdot OPT \) for certain cases.
- **Harmonic Algorithms**: Divide items into classes and pack them in separate regions.

### **4. Advanced Techniques**
- **Skyline-Based Methods**: Tracks the "skyline" of packed items to find optimal placements (e.g., **Bottom-Left (BL)**, **Best-Fit Skyline**).
- **Maximal Rectangles (MaxRects)**: Maintains a list of maximal empty rectangles for efficient placement (used in many game engines).
- **Linear Programming + Rounding**: Solves a relaxed LP and rounds the solution to get a feasible packing.

### **5. Deep Learning & Reinforcement Learning (Emerging Approaches)**
- **Neural Network-Based Predictors**: Predicts good packing orders or placements.
- **Reinforcement Learning (RL)**: Trains agents to sequentially place items to minimize height (e.g., using DQN, PPO).

---

## **Benchmarks and Performance**
- **Best-known heuristics** (e.g., MaxRects, Skyline) perform well in practice but lack strong theoretical guarantees.
- **Hybrid approaches** (e.g., metaheuristics + exact methods) are gaining traction for larger instances.
- **Optimal solutions** are feasible only for small instances (\( n \leq 30 \)) using ILP or B&B.

### **Applications**
- Industrial cutting (wood, glass, metal).
- VLSI chip design.
- Logistics and container loading.
- Texture packing in computer graphics.

Would you like a deeper dive into any specific method?
