#ifndef MODULE2_SORTING_FILES_HPP
#define MODULE2_SORTING_FILES_HPP

#include "Common.hpp"
#include <fstream>
#include <sstream>

class SortingAndFileManager {
public:
    // ========================================================================
    // 1. Bubble Sort: Sort student records by Aggregate Percentage/Marks
    // Stable sort algorithm. Compares adjacent elements and bubbles largest.
    // Optimized with 'swapped' flag for O(N) best case when already sorted.
    // ========================================================================
    static void bubbleSortByMarks(std::vector<Student>& arr, long long& comparisons, long long& swaps) {
        comparisons = 0;
        swaps = 0;
        size_t n = arr.size();
        if (n <= 1) return;

        for (size_t i = 0; i < n - 1; ++i) {
            bool swapped = false;
            for (size_t j = 0; j < n - i - 1; ++j) {
                comparisons++;
                // Descending order (highest marks first)
                if (arr[j].getAverageMarks() < arr[j + 1].getAverageMarks()) {
                    std::swap(arr[j], arr[j + 1]);
                    swaps++;
                    swapped = true;
                }
            }
            // If no elements were swapped, array is already sorted!
            if (!swapped) break;
        }
    }

    // ========================================================================
    // 2. Selection Sort: Sort student records by Student ID (Ascending)
    // Selects minimum element in remaining unsorted portion and swaps with current.
    // Comparisons: O(N^2) always. Swaps: O(N) maximum.
    // ========================================================================
    static void selectionSortById(std::vector<Student>& arr, long long& comparisons, long long& swaps) {
        comparisons = 0;
        swaps = 0;
        size_t n = arr.size();
        if (n <= 1) return;

        for (size_t i = 0; i < n - 1; ++i) {
            size_t minIdx = i;
            for (size_t j = i + 1; j < n; ++j) {
                comparisons++;
                if (arr[j].id < arr[minIdx].id) {
                    minIdx = j;
                }
            }
            if (minIdx != i) {
                std::swap(arr[i], arr[minIdx]);
                swaps++;
            }
        }
    }

    // ========================================================================
    // 3. Insertion Sort: Sort records Alphabetically by Name (Ascending)
    // Builds sorted array one element at a time by shifting larger elements.
    // Best Case: O(N) if already sorted. Worst Case: O(N^2) if reverse sorted.
    // ========================================================================
    static void insertionSortByName(std::vector<Student>& arr, long long& comparisons, long long& shifts) {
        comparisons = 0;
        shifts = 0;
        size_t n = arr.size();
        if (n <= 1) return;

        for (size_t i = 1; i < n; ++i) {
            Student key = arr[i];
            int j = static_cast<int>(i) - 1;

            // Shift elements of arr[0..i-1] that are greater than key.name
            while (j >= 0) {
                comparisons++;
                if (arr[j].name > key.name) {
                    arr[j + 1] = arr[j];
                    shifts++;
                    j--;
                } else {
                    break;
                }
            }
            arr[j + 1] = key;
        }
    }

    // ========================================================================
    // Insertion Sort for integer grades vector
    // ========================================================================
    static void insertionSortGrades(std::vector<int>& grades, long long& comparisons, long long& shifts) {
        comparisons = 0;
        shifts = 0;
        size_t n = grades.size();
        for (size_t i = 1; i < n; ++i) {
            int key = grades[i];
            int j = static_cast<int>(i) - 1;
            while (j >= 0) {
                comparisons++;
                if (grades[j] > key) {
                    grades[j + 1] = grades[j];
                    shifts++;
                    j--;
                } else {
                    break;
                }
            }
            grades[j + 1] = key;
        }
    }

    // ========================================================================
    // 4. File Handling: Read grades.txt, sort using insertion sort, write to sorted_grades.txt
    // ========================================================================
    static void processGradeFiles(const std::string& inputFilename = "grades.txt", 
                                  const std::string& outputFilename = "sorted_grades.txt") {
        std::cout << "\n" << Color::CYAN << "--- Grade File Processing via Insertion Sort ---" << Color::RESET << "\n";
        std::ifstream inFile(inputFilename);
        if (!inFile.is_open()) {
            std::cout << Color::RED << " [Error] Could not open file \"" << inputFilename << "\" for reading.\n"
                      << " Generating a sample \"" << inputFilename << "\" with test data now...\n" << Color::RESET;
            // Create a sample file so the user doesn't get blocked
            std::ofstream sampleFile(inputFilename);
            sampleFile << "85\n42\n99\n76\n58\n91\n63\n88\n70\n100\n35\n";
            sampleFile.close();
            inFile.open(inputFilename);
            if (!inFile.is_open()) {
                std::cout << Color::RED << " [Fatal] Unable to create or read \"" << inputFilename << "\".\n" << Color::RESET;
                return;
            }
            std::cout << Color::GREEN << " [Success] Sample \"" << inputFilename << "\" created successfully.\n" << Color::RESET;
        }

        std::vector<int> grades;
        std::string line;
        int lineNum = 0;
        int invalidCount = 0;

        while (std::getline(inFile, line)) {
            lineNum++;
            // Trim whitespace
            size_t first = line.find_first_not_of(" \t\r\n");
            if (first == std::string::npos) continue; // Skip blank lines
            size_t last = line.find_last_not_of(" \t\r\n");
            std::string trimmed = line.substr(first, (last - first + 1));

            std::stringstream ss(trimmed);
            int gradeVal;
            if (ss >> gradeVal && ss.eof()) {
                if (gradeVal >= 0 && gradeVal <= 100) {
                    grades.push_back(gradeVal);
                } else {
                    std::cout << Color::YELLOW << " [Warning] Line " << lineNum << ": Grade " << gradeVal 
                              << " out of bounds (0-100). Ignored.\n" << Color::RESET;
                    invalidCount++;
                }
            } else {
                std::cout << Color::YELLOW << " [Warning] Line " << lineNum << ": Invalid entry \"" << trimmed << "\" ignored.\n" << Color::RESET;
                invalidCount++;
            }
        }
        inFile.close();

        if (grades.empty()) {
            std::cout << Color::RED << " [Error] File \"" << inputFilename << "\" contained no valid integer grades.\n" << Color::RESET;
            return;
        }

        std::cout << Color::GREEN << " [Success] Read " << grades.size() << " valid grades from \"" 
                  << inputFilename << "\" (" << invalidCount << " invalid ignored).\n" << Color::RESET;
        
        std::cout << " Raw Unsorted Grades: ";
        for (int g : grades) std::cout << g << " ";
        std::cout << "\n";

        // Sort using manual Insertion Sort
        long long comps = 0, shifts = 0;
        insertionSortGrades(grades, comps, shifts);

        std::cout << Color::GREEN << " Sorted Grades (Ascending): ";
        for (int g : grades) std::cout << g << " ";
        std::cout << Color::RESET << "\n";
        std::cout << Color::MAGENTA << " -> Insertion Sort Metric: " << comps << " comparisons, " << shifts << " shifts.\n" << Color::RESET;

        // Write output
        std::ofstream outFile(outputFilename);
        if (!outFile.is_open()) {
            std::cout << Color::RED << " [Error] Could not open \"" << outputFilename << "\" for writing.\n" << Color::RESET;
            return;
        }

        for (int g : grades) {
            outFile << g << "\n";
        }
        outFile.close();
        std::cout << Color::GREEN << " [Success] Sorted grades written to \"" << outputFilename << "\" successfully.\n" << Color::RESET;
    }

    // Interactive Menu Handler for Module 2
    static void runMenu(std::vector<Student>& sharedStudents) {
        while (true) {
            std::cout << "\n" << Color::BLUE << Color::BOLD 
                      << "========================================================\n"
                      << "      MODULE 2: SORTING ALGORITHMS & FILE HANDLING      \n"
                      << "========================================================\n"
                      << Color::RESET;
            std::cout << " 1. Bubble Sort: Sort Students by Marks/Percentage (Descending)\n"
                      << " 2. Selection Sort: Sort Students by Student ID (Ascending)\n"
                      << " 3. Insertion Sort: Sort Students Alphabetically by Name (A-Z)\n"
                      << " 4. File Handling: Read 'grades.txt', Sort via Insertion Sort & Save\n"
                      << " 5. Display Current Student List Order\n"
                      << " 6. Algorithmic Complexity Comparison (Bubble vs Selection vs Insertion)\n"
                      << " 0. Return to Main Menu\n";

            int choice = InputHelper::getInteger(" Enter your choice (0-6): ");
            if (choice == 0) break;

            switch (choice) {
                case 1: {
                    if (sharedStudents.empty()) {
                        std::cout << Color::YELLOW << " [Notice] No student records available to sort.\n" << Color::RESET;
                        break;
                    }
                    long long comps = 0, swaps = 0;
                    bubbleSortByMarks(sharedStudents, comps, swaps);
                    std::cout << Color::GREEN << " [Sorted] Student records sorted by Average Marks (Bubble Sort):\n" << Color::RESET;
                    std::cout << Color::MAGENTA << " -> Comparisons: " << comps << " | Swaps: " << swaps << "\n" << Color::RESET;
                    displayTable(sharedStudents);
                    break;
                }
                case 2: {
                    if (sharedStudents.empty()) {
                        std::cout << Color::YELLOW << " [Notice] No student records available to sort.\n" << Color::RESET;
                        break;
                    }
                    long long comps = 0, swaps = 0;
                    selectionSortById(sharedStudents, comps, swaps);
                    std::cout << Color::GREEN << " [Sorted] Student records sorted by Student ID (Selection Sort):\n" << Color::RESET;
                    std::cout << Color::MAGENTA << " -> Comparisons: " << comps << " | Swaps: " << swaps << "\n" << Color::RESET;
                    displayTable(sharedStudents);
                    break;
                }
                case 3: {
                    if (sharedStudents.empty()) {
                        std::cout << Color::YELLOW << " [Notice] No student records available to sort.\n" << Color::RESET;
                        break;
                    }
                    long long comps = 0, shifts = 0;
                    insertionSortByName(sharedStudents, comps, shifts);
                    std::cout << Color::GREEN << " [Sorted] Student records sorted Alphabetically by Name (Insertion Sort):\n" << Color::RESET;
                    std::cout << Color::MAGENTA << " -> Comparisons: " << comps << " | Shifts: " << shifts << "\n" << Color::RESET;
                    displayTable(sharedStudents);
                    break;
                }
                case 4:
                    processGradeFiles();
                    break;
                case 5:
                    displayTable(sharedStudents);
                    break;
                case 6: {
                    std::cout << "\n" << Color::YELLOW << Color::BOLD
                              << "========================================================================================\n"
                              << "                       SORTING ALGORITHMS COMPLEXITY COMPARISON                         \n"
                              << "========================================================================================\n"
                              << Color::RESET
                              << " Algorithm      | Best Time | Avg Time  | Worst Time | Aux Space | Stability | Main Advantage\n"
                              << "----------------+-----------+-----------+------------+-----------+-----------+---------------------------\n"
                              << " Bubble Sort    | O(N)      | O(N^2)    | O(N^2)     | O(1)      | Stable    | Detects already sorted data\n"
                              << " Selection Sort | O(N^2)    | O(N^2)    | O(N^2)     | O(1)      | Unstable  | Minimizes swaps (at most N)\n"
                              << " Insertion Sort | O(N)      | O(N^2)    | O(N^2)     | O(1)      | Stable    | Optimal for nearly sorted\n"
                              << "----------------+-----------+-----------+------------+-----------+-----------+---------------------------\n"
                              << Color::CYAN
                              << " [!] EFFECT OF INPUT ON INSERTION SORT:\n"
                              << "   1. Already Sorted: Each element compared once with left predecessor, zero shifts.\n"
                              << "      Total Time = O(N) [Optimal linear benchmark].\n"
                              << "   2. Reverse Sorted: Every element must shift all predecessors to the right.\n"
                              << "      Total Comparisons & Shifts = N*(N-1)/2 = O(N^2) [Worst case scenario].\n"
                              << "========================================================================================\n"
                              << Color::RESET;
                    break;
                }
                default:
                    std::cout << Color::RED << " Invalid option. Please select 0 to 6.\n" << Color::RESET;
            }
            InputHelper::pause();
        }
    }

private:
    static void displayTable(const std::vector<Student>& students) {
        if (students.empty()) {
            std::cout << Color::YELLOW << " [Notice] Student list is empty.\n" << Color::RESET;
            return;
        }
        std::cout << Color::CYAN << Color::BOLD 
                  << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                  << "| ID     | Name               | Div  | Contact      | Average | Marks (M, P, C, CS) |\n"
                  << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                  << Color::RESET;
        for (const auto& s : students) {
            s.printRow();
        }
        std::cout << Color::CYAN << "+--------+--------------------+------+--------------+---------+---------------------+\n" << Color::RESET;
    }
};

#endif // MODULE2_SORTING_FILES_HPP
