/**
 * ============================================================================
 * PIMPRI CHINCHWAD COLLEGE OF ENGINEERING (PCCOE), PUNE
 * Department of Information Technology
 * S.Y. B.Tech Information Technology | Academic Year 2026–27
 * Course: Data Structures and Algorithms
 * 
 * INTEGRATED ASSIGNMENT:
 * SMART CAMPUS INFORMATION AND RESOURCE MANAGEMENT SYSTEM
 * ============================================================================
 * 
 * Modules Implemented:
 *  - Module 1: Student Information Management & Searching (Linear & Binary Search)
 *  - Module 2: Student Record Sorting & File Handling (Bubble, Selection, Insertion Sort)
 *  - Module 3: Sparse Matrix Representation & Grade Analysis (Triplet Form)
 *  - Module 4: Campus Services Using Linked Lists (Singly, Doubly, Circular Lists)
 *  - Module 5: Stacks, Queues & Expression Conversion (Undo Stack, Infix/Postfix/Prefix, Queues)
 *  - Module 6: Hierarchical Indexing Using Trees (BST, AVL Tree, B+ Tree)
 *  - Module 7: Campus Network Optimization (Adjacency Matrix/List, BFS, DFS, Prim MST, Dijkstra)
 *  - Module 8: Fast Record Retrieval Using Hashing (Open Addressing, Linear Probing, Tombstones)
 *  - Module 9: System Integration, Validation & Comprehensive Complexity Analysis
 * ============================================================================
 */

#include "src/Common.hpp"
#include "src/Module1_StudentSearch.hpp"
#include "src/Module2_SortingFiles.hpp"
#include "src/Module3_SparseMatrix.hpp"
#include "src/Module4_LinkedLists.hpp"
#include "src/Module5_StacksQueues.hpp"
#include "src/Module6_Trees.hpp"
#include "src/Module7_CampusGraph.hpp"
#include "src/Module8_HashTable.hpp"
#include "src/Module9_Complexity.hpp"

// Global System Coordinator
class SmartCampusSystem {
private:
    StudentManager studentMgr;
    SparseGradeMatrix sparseMatrix;
    CampusServicesManager servicesMgr;
    StackQueueManager stackQueueMgr;
    TreeIndexManager treeMgr;
    CampusNetwork campusMap;
    StudentHashTable studentHash;

public:
    SmartCampusSystem() : studentHash(11) {
        loadInitialCampusData();
    }

    // Populate initial rich demo dataset across all integrated modules
    void loadInitialCampusData() {
        // 1. Initial Students
        std::vector<Student> initialStudents = {
            Student(101, "Aarav Sharma",   "A", "09876543210", {85, 78, 92, 88}),
            Student(102, "Diya Patel",     "A", "09123456789", {92, 95, 89, 94}),
            Student(103, "Rohan Kulkarni", "B", "09822334455", {74, 82, 68, 79}),
            Student(104, "Sneha Deshmukh", "B", "09765432101", {88, 90, 85, 91}),
            Student(105, "Ishaan Joshi",   "A", "09445566778", {65, 70, 75, 72}),
            Student(106, "Ananya Roy",     "B", "09112233445", {95, 98, 94, 96})
        };

        for (const auto& s : initialStudents) {
            studentMgr.addStudent(s);
            // Record initial insertions in Undo stack
            stackQueueMgr.getUndoStack().push(ACTION_INSERT, s);
        }

        // Sync to Trees and Hash Table
        treeMgr.populateTrees(studentMgr.getStudents());
        studentHash.populateFromVector(studentMgr.getStudents());
    }

    void run() {
        while (true) {
            std::cout << "\n" << Color::CYAN << Color::BOLD
                      << "========================================================================================\n"
                      << "                    PIMPRI CHINCHWAD COLLEGE OF ENGINEERING, PUNE                       \n"
                      << "                         DEPARTMENT OF INFORMATION TECHNOLOGY                           \n"
                      << "            DATA STRUCTURES & ALGORITHMS | SMART CAMPUS MANAGEMENT SYSTEM               \n"
                      << "========================================================================================\n"
                      << Color::RESET
                      << " [MAIN SYSTEM NAVIGATION MENU]\n"
                      << "  1. Student Information Management & Searching      (Linear & Binary Search)\n"
                      << "  2. Student Record Sorting & External File Handling (Bubble, Selection, Insertion Sort)\n"
                      << "  3. Sparse Matrix Representation & Grade Analysis   (Triplet 3-Tuple Analysis)\n"
                      << "  4. Campus Services Management Using Linked Lists   (Singly, Doubly, Circular Lists)\n"
                      << "  5. Stacks, Queues & Expression Conversion          (Undo Stack, Polish Notation, Queues)\n"
                      << "  6. Hierarchical Indexing Using Trees               (BST, AVL Tree, B+ Tree)\n"
                      << "  7. Campus Network Representation & Optimization    (Graph Matrix/List, BFS, DFS, MST, Dijkstra)\n"
                      << "  8. Student Record Retrieval Using Hashing          (Open Addressing, Linear Probing)\n"
                      << "  9. Master Complexity Analysis & Theoretical Report (Time & Auxiliary Space Tables)\n"
                      << " 10. Reload / Reset Master Campus Demo Dataset\n"
                      << "  0. Exit Smart Campus System\n"
                      << "----------------------------------------------------------------------------------------\n";

            int choice = InputHelper::getInteger(" Select a module to launch (0-10): ");

            if (choice == 0) {
                std::cout << "\n" << Color::GREEN << Color::BOLD 
                          << " [!] Thank you for using the PCCOE Smart Campus Management System. Terminating cleanly.\n" 
                          << Color::RESET;
                break;
            }

            switch (choice) {
                case 1:
                    studentMgr.runMenu();
                    // Sync updated dataset to trees and hash table
                    treeMgr.populateTrees(studentMgr.getStudents());
                    break;
                case 2:
                    SortingAndFileManager::runMenu(studentMgr.getStudents());
                    break;
                case 3:
                    sparseMatrix.runMenu();
                    break;
                case 4:
                    servicesMgr.runMenu();
                    break;
                case 5:
                    stackQueueMgr.runMenu(studentMgr.getStudents());
                    break;
                case 6:
                    treeMgr.runMenu(studentMgr.getStudents());
                    break;
                case 7:
                    campusMap.runMenu();
                    break;
                case 8:
                    studentHash.runMenu(studentMgr.getStudents());
                    break;
                case 9:
                    ComplexityMaster::displayFullAnalysis();
                    InputHelper::pause();
                    break;
                case 10:
                    studentMgr.getStudents().clear();
                    loadInitialCampusData();
                    std::cout << Color::GREEN << " [Success] Campus system data reloaded to baseline demo state.\n" << Color::RESET;
                    InputHelper::pause();
                    break;
                default:
                    std::cout << Color::RED << " Invalid selection! Please enter a number between 0 and 10.\n" << Color::RESET;
                    InputHelper::pause();
            }
        }
    }
};

int main() {
    // Set standard I/O synchronization
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    SmartCampusSystem campusSystem;
    campusSystem.run();

    return 0;
}
