# PIMPRI CHINCHWAD COLLEGE OF ENGINEERING (PCCOE), PUNE
## Department of Information Technology
### S.Y. B.Tech Information Technology | Academic Year 2026–27
### Course: Data Structures and Algorithms (Integrated Assignment)

---

# PROJECT REPORT
# Smart Campus Information and Resource Management System

---

## Table of Contents
1. [Problem Statement & System Objectives](#1-problem-statement--system-objectives)
2. [System Architecture & Design](#2-system-architecture--design)
3. [Module-by-Module Algorithmic Specifications & Pseudocode](#3-module-by-module-algorithmic-specifications--pseudocode)
   - [Module 1: Student Information Management & Searching](#module-1-student-information-management--searching)
   - [Module 2: Student Record Sorting & File Handling](#module-2-student-record-sorting--file-handling)
   - [Module 3: Sparse Matrix Representation & Grade Analysis](#module-3-sparse-matrix-representation--grade-analysis)
   - [Module 4: Campus Services Management Using Linked Lists](#module-4-campus-services-management-using-linked-lists)
   - [Module 5: Stacks, Queues & Expression Conversion](#module-5-stacks-queues--expression-conversion)
   - [Module 6: Hierarchical Indexing Using Trees (BST, AVL, B+ Tree)](#module-6-hierarchical-indexing-using-trees)
   - [Module 7: Campus Network Representation & Optimization](#module-7-campus-network-representation--optimization)
   - [Module 8: Student Record Retrieval Using Hashing](#module-8-student-record-retrieval-using-hashing)
   - [Module 9: Integration & Consistency Management](#module-9-integration--consistency-management)
4. [Master Time & Auxiliary Space Complexity Analysis](#4-master-time--auxiliary-space-complexity-analysis)
5. [Verification, Test Cases & Observed Results](#5-verification-test-cases--observed-results)
6. [Sample Console Input / Output Transcripts](#6-sample-console-input--output-transcripts)
7. [Conclusion & Critical Architectural Trade-Offs](#7-conclusion--critical-architectural-trade-offs)

---

## 1. Problem Statement & System Objectives

### 1.1 Problem Statement
A large educational institution manages complex, heterogeneous streams of information: student academic records, course performance matrices, facility maintenance tickets, shuttle transportation routes, recurring administrative tasks, campus network connectivity, and immediate record retrieval. Handling these subsystems through disconnected, ad-hoc programs leads to data redundancy, stale records, request queuing delays, and high algorithmic overhead when scaling to thousands of entities.

To resolve these challenges, the **Smart Campus Information and Resource Management System** integrates linear, hierarchical, graph, and hashed data structures into a single unified C++ architecture. Rather than treating data structures as isolated textbook exercises, this system models a cohesive real-world campus enterprise where student data flows consistently across multiple indices, search structures, and optimization engines.

### 1.2 System Objectives
1. **Efficient Record Management:** Maintain structured student data with robust validation, duplicate prevention, and search retrieval.
2. **Algorithmic Searching & Sorting:** Implement and empirically evaluate Linear Search, Binary Search, Bubble Sort, Selection Sort, and Insertion Sort on real datasets.
3. **Sparse Representation:** Model sparse course enrollment data using 3-Tuple (Triplet) representations to conserve RAM.
4. **Dynamic Linear Sequences:** Manage campus facility workflows using Singly, Doubly, and Circular Linked Lists.
5. **Reversible Operations & Task Scheduling:** Implement transaction undo capabilities via Stacks, expression evaluation via Polish notations, and service queues via Linear and Circular buffers.
6. **Hierarchical Indexing:** Deliver ordered retrieval through Binary Search Trees (BST), balanced fast access via AVL Trees, and high-efficiency range scans through B+ Trees.
7. **Network Optimization:** Model campus facilities as weighted graphs to compute minimum cabling cost (Prim's MST) and shortest navigational pathways (Dijkstra's Algorithm).
8. **Constant-Time Retrieval:** Deploy Open Addressing Hash Tables with Linear Probing and Tombstone deletion management.
9. **Architectural Consistency:** Ensure that record insertions, deletions, and updates are synchronized across all active indexes without dangling references or memory leaks.

---

## 2. System Architecture & Design

### 2.1 Architectural Flow Diagram
```
                          +------------------------------------------+
                          |        Main System Coordinator           |
                          |                (main.cpp)                |
                          +--------------------+---------------------+
                                               |
         +--------------------+----------------+--------------------+--------------------+
         |                    |                |                    |                    |
+--------v-------+   +--------v-------+ +------v--------+   +-------v--------+   +-------v--------+
|   Module 1 & 2 |   |    Module 3    | |   Module 4    |   |    Module 5    |   |    Module 6    |
| Student Store  |   | Sparse Matrix  | | Linked Lists  |   | Stacks & Queues|   | Hierarchical   |
| & File Sort    |   | Triplet Engine | | SLL, DLL, CLL |   | Undo, Infix, Q |   | Trees Index    |
+--------+-------+   +----------------+ +---------------+   +-------+--------+   +-------+--------+
         |                                                          |                    |
         |----------------------------------------------------------+                    |
         |         (Shared Record Set: ID, Name, Marks, Div)                             |
         |                                                                               |
+--------v-------+                                                              +--------v-------+
|    Module 8    |                                                              |    Module 7    |
| Hash Table     |<-------------------------------------------------------------| Campus Graph   |
| Direct Lookup  |                                                              | Prim, Dijkstra |
+----------------+                                                              +----------------+
```

### 2.2 Component Responsibilities
- **Data Model Layer (`Common.hpp`):** Central `Student` model containing ID, name, division, contact string, and mark arrays, plus ANSI color formatting and type-safe input readers.
- **Sequential Storage & File I/O (`Module1_StudentSearch.hpp`, `Module2_SortingFiles.hpp`):** In-memory array management, file parsing from `grades.txt`, validation of integers, and exporting to `sorted_grades.txt`.
- **Mathematical Matrix Compression (`Module3_SparseMatrix.hpp`):** Dynamic 2D arrays converted to Triplet `(row, col, value)` lists.
- **Dynamic Node Management (`Module4_LinkedLists.hpp`):** Singly linked list for tickets, doubly linked list for two-way shuttle travel, and circular linked list for digital notices.
- **Buffer & Parsing Engines (`Module5_StacksQueues.hpp`):** LIFO stack capturing inverse operations for rollback; operator stacks parsing infix arithmetic to postfix/prefix; FIFO queues.
- **Multi-Index Tree Store (`Module6_Trees.hpp`):** BST for recursive operations, AVL tree with balance factor self-balancing, and B+ Tree with linked leaf records for $O(\log N + K)$ range queries.
- **Spatial Routing Engine (`Module7_CampusGraph.hpp`):** Graph network represented simultaneously via Adjacency Matrix and Adjacency List.
- **Fast Key-Value Storage (`Module8_HashTable.hpp`):** Open addressing hash table resolving collisions via Linear Probing and marking deletions via Tombstones.

---

## 3. Module-by-Module Algorithmic Specifications & Pseudocode

### Module 1: Student Information Management & Searching
- **Linear Search:** Scans elements iteratively from index $0$ to $N-1$. Works unconditionally on unsorted datasets.
  ```text
  Algorithm LinearSearch(Array A, TargetKey):
    For index i from 0 to Length(A) - 1:
      If A[i].id == TargetKey:
        Return i (Record Found)
    Return -1 (Not Found)
  ```
- **Binary Search:** Exploits monotonicity on an array sorted by Student ID. Computes midpoint $mid = low + \lfloor(high - low) / 2\rfloor$, discarding half the search window each step.
  ```text
  Algorithm BinarySearch(SortedArray A, TargetKey):
    low = 0, high = Length(A) - 1
    While low <= high:
      mid = low + (high - low) / 2
      If A[mid].id == TargetKey: Return mid
      Else if A[mid].id < TargetKey: low = mid + 1
      Else: high = mid - 1
    Return -1 (Not Found)
  ```

### Module 2: Student Record Sorting & File Handling
- **Bubble Sort:** Iterates through $N$ passes, comparing adjacent pairs $(A[j], A[j+1])$. Uses a boolean flag `swapped` to abort early if no inversions occur, achieving $O(N)$ best-case time.
- **Selection Sort:** Scans the unsorted prefix to locate the minimum key, executing at most $N-1$ swaps across execution.
- **Insertion Sort:** Iteratively inserts element $A[i]$ into its correct position within sorted sub-array $A[0 \dots i-1]$ by shifting larger elements one position right.
  ```text
  Algorithm InsertionSort(Array A):
    For i from 1 to Length(A) - 1:
      key = A[i]
      j = i - 1
      While j >= 0 and A[j] > key:
        A[j + 1] = A[j]
        j = j - 1
      A[j + 1] = key
  ```
- **File Handling:** Safely consumes `grades.txt`, ignores blank or malformed lines, validates integer bounds $[0, 100]$, sorts using Insertion Sort, and writes outputs to `sorted_grades.txt`.

### Module 3: Sparse Matrix Representation & Grade Analysis
- **Triplet Transformation:** Scans dense matrix $M[R][C]$. Cell $(0, 0)$ of the triplet structure stores header metadata: `(rows, cols, nonZeroCount)`. Each subsequent triplet stores `(r, c, value)`.
- **Memory Conservation Threshold:**
  $$\text{Storage}_{\text{Dense}} = R \times C \times \text{sizeof}(\text{int})$$
  $$\text{Storage}_{\text{Triplet}} = 3 \times (NZ + 1) \times \text{sizeof}(\text{int})$$
  Triplet representation saves physical memory if and only if:
  $$3(NZ + 1) < R \times C \iff NZ < \frac{R \times C}{3}$$
  If non-zero density is below $33.3\%$, Triplet compression is mathematically superior.

### Module 4: Campus Services Management Using Linked Lists
1. **Singly Linked List (Maintenance Requests):** Unidirectional pointers (`next`). Head insertion is $O(1)$; deletion of arbitrary nodes requires traversing to the preceding node ($O(N)$).
2. **Doubly Linked List (Campus Shuttle Route):** Bidirectional pointers (`prev`, `next`). Supports forward route traversal (Outbound: Gate $\to$ Hostels) and reverse traversal (Inbound: Hostels $\to$ Gate) in $O(N)$ without recursive stack overhead. Deletion of a given node pointer is $O(1)$.
3. **Circular Singly Linked List (Announcements):** Maintained using a single `last` pointer (where `last->next == head`). Enables $O(1)$ insertions at both the front and rear. Facilitates infinite cyclic playback across campus digital displays.

### Module 5: Stacks, Queues & Expression Conversion
- **Undo Transaction Engine:** Implemented via a linked stack storing `(ActionType, StudentRecord)`.
  - When an insertion occurs, an `ACTION_INSERT` token is pushed. Rollback triggers deletion.
  - When a deletion occurs, the full `StudentRecord` is captured alongside an `ACTION_DELETE` token. Rollback restores the student into the active dataset.
- **Infix to Postfix / Prefix Conversion:**
  - Evaluates operator precedence: `^` (3, Right-to-Left), `* /` (2, Left-to-Right), `+ -` (1, Left-to-Right).
  - Prefix generation: Reverses the infix string (inverting parentheses), applies postfix generation with adjusted associativity, and reverses the final output.
- **Linear Queue vs. Circular Queue:**
  - Linear queue triggers **False Overflow** when rear reaches capacity, even if front positions were vacated by dequeue operations.
  - Circular queue solves this via modular arithmetic:
    $$\text{rear} = (\text{rear} + 1) \bmod \text{Capacity}, \quad \text{front} = (\text{front} + 1) \bmod \text{Capacity}$$
    $$\text{Full Condition: } (\text{rear} + 1) \bmod \text{Capacity} == \text{front}$$

### Module 6: Hierarchical Indexing Using Trees
- **Binary Search Tree (BST):** Satisfies $Key(Left) < Key(Node) < Key(Right)$.
  - **Deletion Cases:**
    1. Node is a Leaf: Directly freed and parent pointer nulled.
    2. Node has One Child: Parent bypasses node to point to the child.
    3. Node has Two Children: Node replaced by Inorder Successor (minimum node in right subtree), then successor is deleted recursively.
  - Traversal variants: Inorder (sorted), Preorder, Postorder, and Queue-based Level-Order (Breadth-First).
- **AVL Tree:** Enforces self-balancing via height invariants:
  $$\text{Balance Factor } BF(u) = \text{Height}(u.left) - \text{Height}(u.right) \in \{-1, 0, +1\}$$
  Automatically triggers one of 4 rotations upon insertion/deletion:
  - **LL:** Right rotation on unbalanced node.
  - **RR:** Left rotation on unbalanced node.
  - **LR:** Left rotation on left child, followed by Right rotation on node.
  - **RL:** Right rotation on right child, followed by Left rotation on node.
- **B+ Tree (Order 3):**
  - All data records are stored exclusively in **Leaf Nodes**.
  - Internal nodes store only router keys and child pointers.
  - Leaves are linked sequentially via `next` pointers.
  - Range Query $[minID, maxID]$ searches down to the leaf containing $minID$ in $O(\log N)$, then sequentially iterates across the linked leaves in $O(K)$, avoiding full-tree traversals.

### Module 7: Campus Network Representation & Optimization
- **Dual Representation:** Maintained concurrently as a 2D matrix ($O(V^2)$ lookup) and adjacency list ($O(V+E)$ traversal).
- **Prim's Algorithm (MST):**
  ```text
  Algorithm PrimsMST(Graph G, Source s):
    Initialize key[v] = INF, parent[v] = -1, inMST[v] = False
    key[s] = 0
    Repeat V times:
      u = unvisited vertex with minimum key[u]
      inMST[u] = True
      For each neighbor (v, weight) of u:
        If not inMST[v] and weight < key[v]:
          key[v] = weight
          parent[v] = u
  ```
- **Dijkstra's Shortest Path:** Reconstructs explicit vertex sequences using a stack tracing `parent[v]` backwards from destination to source.

### Module 8: Student Record Retrieval Using Hashing
- **Hash Function:** Division method: $h(k) = k \bmod M$, where $M$ is chosen as a prime number (e.g., $M = 11$).
- **Collision Resolution:** Open Addressing with Linear Probing:
  $$h(k, i) = (h(k) + i) \bmod M, \quad \text{for } i = 0, 1, \dots, M-1$$
- **Tombstone Slot Preservation:**
  - Removing a key marks its state as `DELETED` rather than `EMPTY`.
  - Searches continue probing through `DELETED` slots, preventing broken probe chains.
  - Insertions reuse `DELETED` slots, keeping memory compact.

---

## 4. Master Time & Auxiliary Space Complexity Analysis

| Data Structure / Algorithm | Best Time | Average Time | Worst Time | Space Complexity | Practical Trade-off Analysis |
|---|---|---|---|---|---|
| **Linear Search** | $O(1)$ | $O(N)$ | $O(N)$ | $O(1)$ | No sorting prerequisite; slow on large scales. |
| **Binary Search** | $O(1)$ | $O(\log N)$ | $O(\log N)$ | $O(1)$ | Requires sorted input; optimal for static lookups. |
| **Bubble Sort** | $O(N)$ | $O(N^2)$ | $O(N^2)$ | $O(1)$ | Fast on nearly-sorted data via early-exit flag. |
| **Selection Sort** | $O(N^2)$ | $O(N^2)$ | $O(N^2)$ | $O(1)$ | Minimizes write/swap operations (at most $N-1$). |
| **Insertion Sort** | $O(N)$ | $O(N^2)$ | $O(N^2)$ | $O(1)$ | Ideal for small or streaming datasets. |
| **Singly Linked List** | $O(1)$ (Head) | $O(N)$ | $O(N)$ | $O(N)$ | Forward traversal only; minimal pointer overhead. |
| **Doubly Linked List** | $O(1)$ (Ends) | $O(N)$ | $O(N)$ | $O(N)$ | Bidirectional traversal; extra pointer per node. |
| **Circular Linked List** | $O(1)$ (Ends) | $O(N)$ | $O(N)$ | $O(N)$ | Continuous looping; requires careful termination. |
| **Undo Stack** | $O(1)$ | $O(1)$ | $O(1)$ | $O(N)$ | Instant rollback of recent operations. |
| **Circular Queue** | $O(1)$ | $O(1)$ | $O(1)$ | $O(N)$ | Eliminates false overflow via modulo arithmetic. |
| **Binary Search Tree** | $O(1)$ | $O(\log N)$ | $O(N)$ | $O(N)$ | Can degenerate to linked list on sorted input. |
| **AVL Tree** | $O(1)$ | $O(\log N)$ | $O(\log N)$ | $O(N)$ | Guaranteed $O(\log N)$ search via strict rotations. |
| **B+ Tree (Order M)** | $O(1)$ | $O(\log_M N)$ | $O(\log_M N)$ | $O(N)$ | Shallow tree; optimal for disk storage and range queries. |
| **Graph BFS / DFS** | $O(V+E)$ | $O(V+E)$ | $O(V+E)$ | $O(V)$ | Complete path reachability & connectivity check. |
| **Prim's MST** | $O(V^2)$ | $O(V^2)$ | $O(V^2)$ | $O(V)$ | Connects all buildings with minimum total cost. |
| **Dijkstra Shortest Path**| $O(V^2)$ | $O(V^2)$ | $O(V^2)$ | $O(V)$ | Computes point-to-point shortest navigational path. |
| **Hash Table (Open Addr)**| $O(1)$ | $O(1)$ | $O(N)$ | $O(M)$ | Instant ID lookup; degrades if load factor $\alpha > 0.7$. |

---

## 5. Verification, Test Cases & Observed Results

The implementation was validated against all 13 required test conditions:

| Test ID | Test Scenario Description | Input Condition | Expected System Behavior | Observed Result | Status |
|---|---|---|---|---|---|
| **TC-01** | Multi-record population | 6 student records | Tabular display formatted correctly | All rows aligned with computed averages | PASS |
| **TC-02** | Unsuccessful search | Search ID `999` in Linear/Binary | Return `-1`, display friendly error | "Student ID 999 not present" | PASS |
| **TC-03** | Duplicate ID insertion | Insert existing ID `101` | System rejects with warning | "ID 101 already exists! Duplicates rejected" | PASS |
| **TC-04** | Empty structure deletion | Delete from empty list | Detect underflow gracefully | "List is empty. Nothing to delete" | PASS |
| **TC-05** | Linked list boundary ops | Insert/Delete Head and Tail nodes | Correct pointer updates, no dangling links | Head and tail updated cleanly without leaks | PASS |
| **TC-06** | Buffer overflow / underflow | Dequeue empty queue; Enqueue full queue | Underflow / Overflow flags raised | "Queue Underflow" and "Queue Overflow" | PASS |
| **TC-07** | Missing grade file | `grades.txt` absent | Catch file error, auto-generate fallback | Created sample file and proceeded safely | PASS |
| **TC-08** | Extreme sorted inputs | Array $[10, 20, 30]$ & $[30, 20, 10]$ | $O(N)$ comparisons vs $O(N^2)$ comparisons | Insertion sort verified: 0 shifts vs $N(N-1)/2$ shifts | PASS |
| **TC-09** | BST 3 deletion cases | Delete leaf, 1-child, 2-child nodes | Tree structure preserved; inorder intact | Successor promoted properly in 2-child case | PASS |
| **TC-10** | AVL 4-rotation triggers | Insert sequences triggering LL, RR, LR, RL | Rotations triggered, $|BF| \le 1$ preserved | All heights rebalanced to $\le 2$ | PASS |
| **TC-11** | Disconnected graph | Isolated vertex with no edges | BFS/DFS and Prim warn disconnected | "Warning: Graph is Disconnected!" | PASS |
| **TC-12** | Hash collisions & tombstones| Keys hashing to same slot, delete, re-search | Probing continues past `DELETED` slots | Element found behind tombstone | PASS |
| **TC-13** | Sparse matrix boundaries | 100% zero matrix & 100% non-zero matrix | Correct header tuple and storage analysis | Correctly reports $0\%$ and $100\%$ sparsity | PASS |

---

## 6. Sample Console Input / Output Transcripts

### 6.1 Module 1: Binary Search Verification
```
Enter Student ID to search (Binary Search): 104
[Found] Record located via Binary Search (after ID sort) at index 3:
+--------+--------------------+------+--------------+---------+---------------------+
| ID     | Name               | Div  | Contact      | Average | Marks (M, P, C, CS) |
+--------+--------------------+------+--------------+---------+---------------------+
|    104 | Sneha Deshmukh     |    B |  09765432101 |   88.5% |   88,  90,  85,  91 |
+--------+--------------------+------+--------------+---------+---------------------+
-> Binary Search Comparisons made: 2 (O(log N) search on sorted data)
```

### 6.2 Module 3: Sparse Matrix & Triplet Reconstruction
```
--- Triplet (3-Tuple) Sparse Representation ---
+-------+-------+-------+---------------------------------+
| Row   | Col   | Value | Description                     |
+-------+-------+-------+---------------------------------+
|     5 |     4 |    11 | Header: [Rows, Cols, NonZeros]  |
+-------+-------+-------+---------------------------------+
|     0 |     0 |    85 | S1 (Aarav) -> Mathematics       |
|     0 |     2 |    78 | S1 (Aarav) -> Chemistry         |
|     0 |     3 |    90 | S1 (Aarav) -> Computer Sci      |
|     1 |     1 |    76 | S2 (Diya) -> Physics            |
|     1 |     2 |    82 | S2 (Diya) -> Chemistry          |
|     2 |     0 |    91 | S3 (Rohan) -> Mathematics       |
|     2 |     1 |    88 | S3 (Rohan) -> Physics           |
|     2 |     3 |    95 | S3 (Rohan) -> Computer Sci      |
|     3 |     2 |    74 | S4 (Sneha) -> Chemistry         |
|     3 |     3 |    80 | S4 (Sneha) -> Computer Sci      |
|     4 |     0 |    79 | S5 (Ishaan) -> Mathematics      |
+-------+-------+-------+---------------------------------+
Total elements: 20 | Non-Zero Entries: 11 | Sparsity: 45.0%
```

### 6.3 Module 5: Infix Conversion Benchmark
```
Verified Specification Benchmark:
  Infix:   A+B*C
  Postfix: ABC*+ (Expected: ABC*+)
  Prefix:  +A*BC (Expected: +A*BC)
```

### 6.4 Module 7: Dijkstra Shortest Path Navigation
```
--- Dijkstra's Shortest Path from Source: [0: Main Gate / Admin] ---
+----+---------------------------+----------+---------------------------------------------------+
| ID | Destination Building       | Distance | Shortest Route Path Sequence                      |
+----+---------------------------+----------+---------------------------------------------------+
|  0 | Main Gate / Admin         |       0m | [0]                                               |
|  1 | IT & CS Department        |     150m | [0]->[1]                                          |
|  2 | Central Library           |     200m | [0]->[2]                                          |
|  3 | Central Auditorium        |     370m | [0]->[1]->[3]                                     |
|  4 | Student Hostels           |     500m | [0]->[2]->[4]                                     |
|  5 | Sports Arena / Ground     |     550m | [0]->[1]->[3]->[5]                                |
|  6 | Cafeteria & Student Mess  |     620m | [0]->[2]->[4]->[6]                                |
+----+---------------------------+----------+---------------------------------------------------+
```

---

## 7. Conclusion & Critical Architectural Trade-Offs

The development of the **Smart Campus Information and Resource Management System** illustrates the practical applications of classic computer science data structures:

1. **Trade-offs in Hierarchical Indexing:**
   - Standard **BSTs** offer intuitive recursive implementations, but their vulnerability to worst-case $O(N)$ degeneration under presorted insertions makes them unreliable for production indexing.
   - **AVL Trees** solve this with strict height balancing, guaranteeing $O(\log N)$ search, but their rotation overhead during frequent insertions and deletions is non-trivial.
   - **B+ Trees** emerge as the superior index structure for databases: by storing all records in linked leaves and keeping the tree broad and shallow, they provide consistent search times and enable $O(\log N + K)$ range queries.

2. **Storage Optimization in Sparse Domains:**
   - Dense matrices require $O(R \times C)$ memory regardless of enrollment. Triplet representations reduce storage whenever sparsity exceeds $66.7\%$.

3. **Hashing vs. Search Trees:**
   - Open Addressing Hash Tables provide expected $O(1)$ lookups, outperforming tree searches. However, maintaining load factor $\alpha \le 0.7$ and using Tombstone markers during deletion are essential to prevent clustering.

4. **Network Optimization:**
   - Graph representations highlight the fundamental difference between **Prim's MST** (which minimizes total infrastructure cabling cost across the campus) and **Dijkstra's Algorithm** (which minimizes the traversal distance for an individual student travelling between two buildings).

By integrating these data structures into a cohesive system, this project demonstrates how thoughtful algorithmic selection produces reliable, memory-efficient, and maintainable software.

---
*Report submitted in partial fulfillment of the academic requirements for S.Y. B.Tech Information Technology, PCCOE Pune.*
