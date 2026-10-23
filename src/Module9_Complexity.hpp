#ifndef MODULE9_COMPLEXITY_HPP
#define MODULE9_COMPLEXITY_HPP

#include "Common.hpp"

class ComplexityMaster {
public:
    static void displayFullAnalysis() {
        std::cout << "\n" << Color::YELLOW << Color::BOLD
                  << "===================================================================================================================\n"
                  << "               PIMPRI CHINCHWAD COLLEGE OF ENGINEERING, PUNE | S.Y. B.TECH IT 2026-27                              \n"
                  << "               DATA STRUCTURES & ALGORITHMS: MASTER TIME & SPACE COMPLEXITY ANALYSIS                               \n"
                  << "===================================================================================================================\n"
                  << Color::RESET;

        std::cout << Color::CYAN << Color::BOLD
                  << "1. SEARCHING & SORTING ALGORITHMS (MODULES 1 & 2)\n" << Color::RESET
                  << "+-------------------+-----------+------------+------------+-----------+--------------------------------------------+\n"
                  << "| Algorithm         | Best Time | Avg Time   | Worst Time | Aux Space | Key Computational Trait & Factors          |\n"
                  << "+-------------------+-----------+------------+------------+-----------+--------------------------------------------+\n"
                  << "| Linear Search     | O(1)      | O(N)       | O(N)       | O(1)      | No preconditions; scans sequentially.      |\n"
                  << "| Binary Search     | O(1)      | O(log N)   | O(log N)   | O(1)      | STRICT REQUIREMENT: Input must be sorted.  |\n"
                  << "| Bubble Sort       | O(N)      | O(N^2)     | O(N^2)     | O(1)      | Stable; O(N) best case with swapped flag.  |\n"
                  << "| Selection Sort    | O(N^2)    | O(N^2)     | O(N^2)     | O(1)      | Unstable; minimizes swaps (at most N-1).   |\n"
                  << "| Insertion Sort    | O(N)      | O(N^2)     | O(N^2)     | O(1)      | Stable; adaptive; O(N^2) on reverse input. |\n"
                  << "+-------------------+-----------+------------+------------+-----------+--------------------------------------------+\n\n";

        std::cout << Color::CYAN << Color::BOLD
                  << "2. LINEAR DATA STRUCTURES (MODULES 4 & 5)\n" << Color::RESET
                  << "+-----------------------------+-----------+------------+-----------+-----------------------------------------------+\n"
                  << "| Data Structure & Operation  | Time Cmplx| Aux Space  | Discipline| Key Behavioral Characteristic                 |\n"
                  << "+-----------------------------+-----------+------------+-----------+-----------------------------------------------+\n"
                  << "| Singly Linked List Insert   | O(1) / O(N| O(N)       | Dynamic   | O(1) at head; O(N) at tail without tail ptr.  |\n"
                  << "| Singly Linked List Delete   | O(N)      | O(1)       | Dynamic   | Requires predecessor pointer to update link.  |\n"
                  << "| Doubly Linked List Insert   | O(1)      | O(N)       | Dynamic   | Supports bidirectional traversal (Prev/Next). |\n"
                  << "| Doubly Linked List Delete   | O(1)*     | O(1)       | Dynamic   | O(1) if target node pointer given directly.   |\n"
                  << "| Circular Linked List (CLL)  | O(1)      | O(N)       | Dynamic   | Last node points back to first; cyclic cycle. |\n"
                  << "| Undo Stack (Push / Pop)     | O(1)      | O(N)       | LIFO      | Reverses insertions and deletions safely.     |\n"
                  << "| Infix to Postfix / Prefix   | O(N)      | O(N)       | Operator  | Respects BODMAS precedence & associativity.   |\n"
                  << "| Linear Queue (Enq / Deq)    | O(1)      | O(N)       | FIFO      | Suffers from False Overflow after deqs.       |\n"
                  << "| Circular Queue (Enq / Deq)  | O(1)      | O(N)       | Modulo    | Reuses vacated memory cells via (rear+1)%MAX. |\n"
                  << "+-----------------------------+-----------+------------+-----------+-----------------------------------------------+\n\n";

        std::cout << Color::CYAN << Color::BOLD
                  << "3. HIERARCHICAL TREE INDEXING (MODULE 6)\n" << Color::RESET
                  << "+-----------------------+-----------+------------+------------+-----------+----------------------------------------+\n"
                  << "| Tree Structure        | Best Time | Avg Time   | Worst Time | Aux Space | Structural Advantage / Limitation      |\n"
                  << "+-----------------------+-----------+------------+------------+-----------+----------------------------------------+\n"
                  << "| Binary Search Tree    | O(1)      | O(log N)   | O(N)       | O(N)      | Degenerates to linked list on sorted.  |\n"
                  << "| AVL Tree (Balanced)   | O(1)      | O(log N)   | O(log N)   | O(N)      | Enforces BF in {-1,0,+1}; 4 rotations. |\n"
                  << "| B+ Tree (Order M)     | O(1)      | O(log_M N) | O(log_M N) | O(N)      | Shallow height; linked leaves enable   |\n"
                  << "|                       |           |            |            |           | O(log_M N + K) range scan queries!     |\n"
                  << "+-----------------------+-----------+------------+------------+-----------+----------------------------------------+\n\n";

        std::cout << Color::CYAN << Color::BOLD
                  << "4. GRAPH NETWORK & HASHING (MODULES 7 & 8)\n" << Color::RESET
                  << "+-----------------------------+------------+------------+-----------+------------------------------------------+\n"
                  << "| Algorithm / Structure       | Adj Matrix | Adj List   | Aux Space | Practical Use in Smart Campus System     |\n"
                  << "+-----------------------------+------------+------------+-----------+------------------------------------------+\n"
                  << "| BFS Traversal               | O(V^2)     | O(V + E)   | O(V)      | Discovers shortest unweighted paths.     |\n"
                  << "| DFS Traversal               | O(V^2)     | O(V + E)   | O(V)      | Cycle detection & campus connectivity.   |\n"
                  << "| Prim's MST Algorithm        | O(V^2)     | O(V^2)     | O(V)      | Minimizes total road/cabling cost.       |\n"
                  << "| Dijkstra Shortest Path      | O(V^2)     | O(V^2)     | O(V)      | Point-to-point shortest navigation route.|\n"
                  << "| Hash Table (Open Address)   | O(1) Avg   | O(N) Worst | O(M)      | Fast O(1) student lookup via ID mod M.   |\n"
                  << "+-----------------------------+------------+------------+-----------+------------------------------------------+\n\n";

        std::cout << Color::GREEN << Color::BOLD
                  << "===================================================================================================================\n"
                  << " SUMMARY OF DESIGN TRADE-OFFS & SELECTION RATIONALE:\n"
                  << "  * Range Queries:       B+ Tree selected over AVL/BST due to contiguous leaf-level linked chains.\n"
                  << "  * Direct ID Lookup:    Hash Table chosen for O(1) average lookup vs O(log N) in balanced trees.\n"
                  << "  * Sparse Grade Data:   Triplet Representation saves memory when matrix density is < 33.3%.\n"
                  << "  * Service Scheduling:  Circular Queue chosen over Linear Queue to prevent false overflow.\n"
                  << "  * Campus Navigation:   Dijkstra's Algorithm used for individual routing; Prim's MST for network infrastructure.\n"
                  << "===================================================================================================================\n"
                  << Color::RESET;
    }
};

#endif // MODULE9_COMPLEXITY_HPP
