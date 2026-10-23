#ifndef MODULE1_STUDENT_SEARCH_HPP
#define MODULE1_STUDENT_SEARCH_HPP

#include "Common.hpp"
#include <algorithm>
#include <cctype>

class StudentManager {
private:
    std::vector<Student> students;

    // Helper: Case-insensitive string search
    static bool containsIgnoreCase(const std::string& str, const std::string& query) {
        if (query.empty()) return true;
        auto it = std::search(
            str.begin(), str.end(),
            query.begin(), query.end(),
            [](char ch1, char ch2) { return std::toupper(ch1) == std::toupper(ch2); }
        );
        return (it != str.end());
    }

public:
    StudentManager() {}

    // Direct access for integration with other modules
    std::vector<Student>& getStudents() { return students; }
    const std::vector<Student>& getStudents() const { return students; }

    // Check if a student ID already exists
    bool exists(int id) const {
        for (const auto& s : students) {
            if (s.id == id) return true;
        }
        return false;
    }

    // 1. Add Student Record with uniqueness check
    bool addStudent(const Student& s) {
        if (exists(s.id)) {
            std::cout << Color::RED << " [Error] Student ID " << s.id << " already exists! Duplicates are not allowed.\n" << Color::RESET;
            return false;
        }
        students.push_back(s);
        std::cout << Color::GREEN << " [Success] Student record for \"" << s.name << "\" (ID: " << s.id << ") added successfully.\n" << Color::RESET;
        return true;
    }

    // 2. Display all registered students in a formatted table
    void displayAll() const {
        if (students.empty()) {
            std::cout << Color::YELLOW << " [Notice] No student records found. The database is empty.\n" << Color::RESET;
            return;
        }
        std::cout << "\n" << Color::CYAN << Color::BOLD 
                  << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                  << "| ID     | Name               | Div  | Contact      | Average | Marks (M, P, C, CS) |\n"
                  << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                  << Color::RESET;
        for (const auto& s : students) {
            s.printRow();
        }
        std::cout << Color::CYAN 
                  << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                  << " Total Students Registered: " << students.size() << Color::RESET << "\n";
    }

    // 3. Linear Search by Student ID
    // Time Complexity: Best: O(1) [First item], Worst/Average: O(N) [Last item or not found]
    int linearSearchById(int targetId, int& comparisons) const {
        comparisons = 0;
        for (size_t i = 0; i < students.size(); ++i) {
            comparisons++;
            if (students[i].id == targetId) {
                return static_cast<int>(i);
            }
        }
        return -1; // Not found
    }

    // 4. Binary Search by Student ID
    // Requirement: Array MUST be sorted by ID first.
    // Time Complexity: Best: O(1) [Middle item], Worst/Average: O(log N)
    int binarySearchById(int targetId, int& comparisons) {
        comparisons = 0;
        if (students.empty()) return -1;

        // Ensure records are sorted by ID before binary search
        std::sort(students.begin(), students.end(), [](const Student& a, const Student& b) {
            return a.id < b.id;
        });

        int low = 0;
        int high = static_cast<int>(students.size()) - 1;

        while (low <= high) {
            comparisons++;
            int mid = low + (high - low) / 2;

            if (students[mid].id == targetId) {
                return mid; // Found
            } else if (students[mid].id < targetId) {
                low = mid + 1; // Discard left half
            } else {
                high = mid - 1; // Discard right half
            }
        }
        return -1; // Target not present
    }

    // 5. Search by Name (supports partial string matching)
    void searchByName(const std::string& nameQuery) const {
        if (students.empty()) {
            std::cout << Color::YELLOW << " [Notice] Student list is empty.\n" << Color::RESET;
            return;
        }

        std::vector<Student> matches;
        for (const auto& s : students) {
            if (containsIgnoreCase(s.name, nameQuery)) {
                matches.push_back(s);
            }
        }

        if (matches.empty()) {
            std::cout << Color::RED << " [Result] No student found matching \"" << nameQuery << "\".\n" << Color::RESET;
        } else {
            std::cout << Color::GREEN << " [Result] Found " << matches.size() << " match(es) for \"" << nameQuery << "\":\n" << Color::RESET;
            std::cout << Color::CYAN
                      << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                      << "| ID     | Name               | Div  | Contact      | Average | Marks (M, P, C, CS) |\n"
                      << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                      << Color::RESET;
            for (const auto& s : matches) {
                s.printRow();
            }
            std::cout << Color::CYAN << "+--------+--------------------+------+--------------+---------+---------------------+\n" << Color::RESET;
        }
    }

    // 6. Update an existing student's details
    bool updateStudent(int id) {
        int dummyComps = 0;
        int index = linearSearchById(id, dummyComps);
        if (index == -1) {
            std::cout << Color::RED << " [Error] Student with ID " << id << " not found in the database.\n" << Color::RESET;
            return false;
        }

        Student& s = students[index];
        std::cout << Color::CYAN << " [Editing Student: " << s.name << " (ID: " << s.id << ")]\n" << Color::RESET;
        std::cout << " 1. Update Name\n"
                  << " 2. Update Division\n"
                  << " 3. Update Contact Number\n"
                  << " 4. Update Subject Marks\n"
                  << " 5. Cancel\n";
        int choice = InputHelper::getInteger(" Select field to update (1-5): ");

        switch (choice) {
            case 1: {
                std::string newName = InputHelper::getString(" Enter new Name: ");
                if (!newName.empty()) {
                    s.name = newName;
                    std::cout << Color::GREEN << " [Updated] Student name updated.\n" << Color::RESET;
                }
                break;
            }
            case 2: {
                std::string newDiv = InputHelper::getString(" Enter new Division: ");
                if (!newDiv.empty()) {
                    s.division = newDiv;
                    std::cout << Color::GREEN << " [Updated] Division updated.\n" << Color::RESET;
                }
                break;
            }
            case 3: {
                std::string newContact = InputHelper::getString(" Enter new Contact Number: ");
                if (!newContact.empty()) {
                    s.contact = newContact;
                    std::cout << Color::GREEN << " [Updated] Contact updated.\n" << Color::RESET;
                }
                break;
            }
            case 4: {
                std::cout << " Enter marks for 4 subjects (Maths, Physics, Chemistry, Computer Science):\n";
                for (int i = 0; i < 4; ++i) {
                    double mark = InputHelper::getDouble("  Subject " + std::to_string(i + 1) + " (0-100): ");
                    if (mark < 0) mark = 0;
                    if (mark > 100) mark = 100;
                    s.marks[i] = mark;
                }
                std::cout << Color::GREEN << " [Updated] Subject marks updated.\n" << Color::RESET;
                break;
            }
            default:
                std::cout << Color::YELLOW << " [Info] Update cancelled.\n" << Color::RESET;
                return false;
        }
        return true;
    }

    // 7. Delete a student record by ID (returns deleted student for undo stack tracking)
    bool deleteStudent(int id, Student& deletedRecord) {
        for (auto it = students.begin(); it != students.end(); ++it) {
            if (it->id == id) {
                deletedRecord = *it;
                students.erase(it);
                std::cout << Color::GREEN << " [Success] Student ID " << id << " deleted successfully.\n" << Color::RESET;
                return true;
            }
        }
        std::cout << Color::RED << " [Error] Student with ID " << id << " does not exist.\n" << Color::RESET;
        return false;
    }

    // Interactive Menu Handler for Module 1
    void runMenu() {
        while (true) {
            std::cout << "\n" << Color::BLUE << Color::BOLD 
                      << "========================================================\n"
                      << "  MODULE 1: STUDENT INFORMATION MANAGEMENT & SEARCHING  \n"
                      << "========================================================\n"
                      << Color::RESET;
            std::cout << " 1. Add New Student Record\n"
                      << " 2. Display All Student Records\n"
                      << " 3. Search Student by ID (Linear Search)\n"
                      << " 4. Search Student by ID (Binary Search - Requires Sorted Array)\n"
                      << " 5. Search Student by Name\n"
                      << " 6. Update Student Details\n"
                      << " 7. Delete Student Record\n"
                      << " 8. Algorithmic Complexity Comparison (Linear vs Binary Search)\n"
                      << " 0. Return to Main Menu\n";

            int choice = InputHelper::getInteger(" Enter your choice (0-8): ");

            if (choice == 0) break;

            switch (choice) {
                case 1: {
                    std::cout << "\n" << Color::CYAN << "--- Add New Student Record ---" << Color::RESET << "\n";
                    int id = InputHelper::getInteger(" Enter Student ID (Unique Integer): ");
                    if (exists(id)) {
                        std::cout << Color::RED << " [Error] Student ID " << id << " already exists!\n" << Color::RESET;
                        break;
                    }
                    std::string name = InputHelper::getString(" Enter Student Full Name: ");
                    std::string div = InputHelper::getString(" Enter Division: ");
                    std::string contact = InputHelper::getString(" Enter Contact Number: ");
                    std::vector<double> marks(4);
                    std::cout << " Enter Marks for 4 Subjects (0-100):\n";
                    std::string subNames[4] = {"Mathematics", "Physics", "Chemistry", "Computer Science"};
                    for (int i = 0; i < 4; ++i) {
                        marks[i] = InputHelper::getDouble("  " + subNames[i] + ": ");
                        if (marks[i] < 0) marks[i] = 0;
                        if (marks[i] > 100) marks[i] = 100;
                    }
                    addStudent(Student(id, name, div, contact, marks));
                    break;
                }
                case 2:
                    displayAll();
                    break;
                case 3: {
                    if (students.empty()) {
                        std::cout << Color::YELLOW << " [Notice] Database is empty.\n" << Color::RESET;
                        break;
                    }
                    int targetId = InputHelper::getInteger(" Enter Student ID to search (Linear Search): ");
                    int comps = 0;
                    int idx = linearSearchById(targetId, comps);
                    if (idx != -1) {
                        std::cout << Color::GREEN << " [Found] Student details located at index " << idx << ":\n" << Color::RESET;
                        std::cout << Color::CYAN
                                  << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                                  << "| ID     | Name               | Div  | Contact      | Average | Marks (M, P, C, CS) |\n"
                                  << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                                  << Color::RESET;
                        students[idx].printRow();
                        std::cout << Color::CYAN << "+--------+--------------------+------+--------------+---------+---------------------+\n" << Color::RESET;
                    } else {
                        std::cout << Color::RED << " [Not Found] Student ID " << targetId << " not present.\n" << Color::RESET;
                    }
                    std::cout << Color::MAGENTA << " -> Linear Search Comparisons made: " << comps << " (O(N) search)\n" << Color::RESET;
                    break;
                }
                case 4: {
                    if (students.empty()) {
                        std::cout << Color::YELLOW << " [Notice] Database is empty.\n" << Color::RESET;
                        break;
                    }
                    int targetId = InputHelper::getInteger(" Enter Student ID to search (Binary Search): ");
                    int comps = 0;
                    int idx = binarySearchById(targetId, comps);
                    if (idx != -1) {
                        std::cout << Color::GREEN << " [Found] Record located via Binary Search (after ID sort) at index " << idx << ":\n" << Color::RESET;
                        std::cout << Color::CYAN
                                  << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                                  << "| ID     | Name               | Div  | Contact      | Average | Marks (M, P, C, CS) |\n"
                                  << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                                  << Color::RESET;
                        students[idx].printRow();
                        std::cout << Color::CYAN << "+--------+--------------------+------+--------------+---------+---------------------+\n" << Color::RESET;
                    } else {
                        std::cout << Color::RED << " [Not Found] Student ID " << targetId << " not present in database.\n" << Color::RESET;
                    }
                    std::cout << Color::MAGENTA << " -> Binary Search Comparisons made: " << comps << " (O(log N) search on sorted data)\n" << Color::RESET;
                    break;
                }
                case 5: {
                    std::string q = InputHelper::getString(" Enter Student Name (or substring) to search: ");
                    searchByName(q);
                    break;
                }
                case 6: {
                    int id = InputHelper::getInteger(" Enter Student ID to update: ");
                    updateStudent(id);
                    break;
                }
                case 7: {
                    int id = InputHelper::getInteger(" Enter Student ID to delete: ");
                    Student dummy;
                    deleteStudent(id, dummy);
                    break;
                }
                case 8: {
                    std::cout << "\n" << Color::YELLOW << Color::BOLD
                              << "========================================================================\n"
                              << "         THEORETICAL ANALYSIS: LINEAR SEARCH vs BINARY SEARCH           \n"
                              << "========================================================================\n"
                              << Color::RESET
                              << " 1. LINEAR SEARCH:\n"
                              << "    - Best Case Time:    O(1) (Key found at first index)\n"
                              << "    - Average Case Time: O(N) (Key found in middle, N/2 comparisons)\n"
                              << "    - Worst Case Time:   O(N) (Key at last index or absent)\n"
                              << "    - Auxiliary Space:   O(1) (Iterative)\n"
                              << "    - Requirement:       None. Works on unsorted, random datasets.\n\n"
                              << " 2. BINARY SEARCH:\n"
                              << "    - Best Case Time:    O(1) (Key located directly at mid)\n"
                              << "    - Average Case Time: O(log2 N)\n"
                              << "    - Worst Case Time:   O(log2 N)\n"
                              << "    - Auxiliary Space:   O(1) (Iterative)\n"
                              << "    - Requirement:       STRICT PRE-CONDITION: Data MUST be sorted.\n"
                              << "    - Why Sorted?:       Binary search relies on monotonicity to discard\n"
                              << "                         half the search space in every comparison. If\n"
                              << "                         data is unsorted, determining whether target is\n"
                              << "                         to the left or right of mid is impossible.\n"
                              << "========================================================================\n";
                    break;
                }
                default:
                    std::cout << Color::RED << " Invalid option. Please select 0 to 8.\n" << Color::RESET;
            }
            InputHelper::pause();
        }
    }
};

#endif // MODULE1_STUDENT_SEARCH_HPP
