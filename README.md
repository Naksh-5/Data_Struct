# Smart Campus Information and Resource Management System
**PIMPRI CHINCHWAD COLLEGE OF ENGINEERING (PCCOE), PUNE**  
**Department of Information Technology**  
*S.Y. B.Tech Information Technology | Academic Year 2026–27*  
*Course: Data Structures and Algorithms (Integrated Assignment)*

---

## 📌 Project Overview
The **Smart Campus Information and Resource Management System** is a unified, menu-driven C++ application that integrates fundamental and advanced data structures and algorithms to streamline educational operations, student service desks, academic analysis, campus transportation, and building network routing.

All underlying data structures (Linked Lists, Stacks, Queues, Binary Search Trees, AVL Trees, B+ Trees, Graphs, and Hash Tables) are **manually implemented from scratch** without relying on standard container abstractions, fulfilling all academic and pedagogical requirements.

---

## 🏗️ System Architecture & Modular Layout

```
Data_Struct/
├── src/
│   ├── Common.hpp                 # Student model, ANSI formatting, safe I/O helpers
│   ├── Module1_StudentSearch.hpp  # Linear Search & Binary Search on Student Records
│   ├── Module2_SortingFiles.hpp   # Bubble, Selection, Insertion Sort & File I/O
│   ├── Module3_SparseMatrix.hpp   # 2D Matrix, 3-Tuple (Triplet) Sparse Analysis
│   ├── Module4_LinkedLists.hpp    # Singly (Requests), Doubly (Shuttle), Circular (Announcements)
│   ├── Module5_StacksQueues.hpp   # Undo Stack, Infix/Postfix/Prefix, Linear & Circular Queues
│   ├── Module6_Trees.hpp          # BST (10 ops), AVL Tree (Rotations), B+ Tree (Range Queries)
│   ├── Module7_CampusGraph.hpp    # Campus Network (Adj Matrix/List, BFS, DFS, Prim MST, Dijkstra)
│   ├── Module8_HashTable.hpp      # Hash Table (Linear Probing, Tombstones, Load Factor)
│   └── Module9_Complexity.hpp     # Formatted Complexity Analysis Tables
├── main.cpp                       # Master Interactive Menu & System Coordinator
├── grades.txt                     # Raw grade records for file sorting
├── PROJECT_REPORT.md              # Complete Academic Submission Report
└── README.md                      # Documentation & Execution Guide
```

---

## 🚀 Compilation and Execution

### Prerequisites
- Any C++17 compliant compiler: `g++`, `clang++`, or `MSVC (cl.exe)`

### Compilation Command
Open your terminal in the project directory and run:

```bash
# Using GCC (MinGW on Windows / Linux / macOS)
g++ -std=c++17 -Wall -Wextra main.cpp -o smart_campus

# Run the executable
# On Windows:
smart_campus.exe

# On Linux / macOS:
./smart_campus
```

### Quick Run with MSVC:
```cmd
cl /std:c++17 /EHsc main.cpp /Fe:smart_campus.exe
smart_campus.exe
```

---

## 📋 Summary of Implemented Modules

| Module | Core Data Structures / Algorithms | Key Features & Real-World Campus Role |
|---|---|---|
| **Module 1** | Dynamic Arrays, Linear & Binary Search | Student records (ID, name, div, contact, marks), search comparison metrics, duplicate rejection. |
| **Module 2** | Bubble, Selection, Insertion Sort, File Streams | Sort by marks, ID, name; file-based grade ingestion (`grades.txt`), input validation, writing `sorted_grades.txt`. |
| **Module 3** | 2D Array, 3-Tuple (Triplet) Sparse Matrix | Sparse student-subject grade matrix, memory savings analysis ($NZ < \frac{R \times C}{3}$), subject averages. |
| **Module 4** | Singly, Doubly, & Circular Linked Lists | Maintenance ticketing (SLL), bidirectional campus shuttle (DLL), continuous broadcast playlist (CLL). |
| **Module 5** | Custom Linked Stack, Array Queues | Full Undo history for student modifications, Infix $\to$ Postfix & Prefix conversion, Linear vs Circular service scheduling. |
| **Module 6** | BST, AVL Tree, B+ Tree (Order 3) | 10 BST operations (mirror, copy, height, level-order), AVL self-balancing rotations (LL/RR/LR/RL), B+ Tree leaf linked range queries. |
| **Module 7** | Graph (Adj Matrix & Adj List) | BFS, DFS, Prim's Minimum Spanning Tree (MST cabling cost), Dijkstra's shortest navigation paths with exact reconstructed sequences. |
| **Module 8** | Open Addressing Hash Table | Division hash $k \bmod M$, Linear Probing, Tombstone (`DELETED`) deletion preservation, load factor $\alpha = n/m$. |
| **Module 9** | System Integration & Evaluation | Unified navigation, shared data consistency, automated test suite, master complexity matrix. |

---

## 🧪 Quick Demo Dataset
The system comes pre-loaded with rich, realistic campus data:
- Pre-populated student registry (IDs: 101 to 106).
- 5x4 Sparse academic grade matrix.
- Campus shuttle route (Main Gate $\leftrightarrow$ Hostels).
- Maintenance requests and digital signage announcements.
- Realistic PCCOE campus map (7 buildings, 9 weighted interconnecting roads).

Selecting menu option `10` at any time re-initializes the system to its baseline demo state.

---

## 👥 Authors & Academic Details
- **Institution:** Pimpri Chinchwad College of Engineering (PCCOE), Pune
- **Department:** Department of Information Technology
- **Class:** S.Y. B.Tech IT (Academic Year 2026–27)
- **Course:** Data Structures and Algorithms
