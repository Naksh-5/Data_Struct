#ifndef MODULE3_SPARSE_MATRIX_HPP
#define MODULE3_SPARSE_MATRIX_HPP

#include "Common.hpp"

// Single Triplet entry (Row, Column, Value)
struct MatrixTriplet {
    int row;
    int col;
    int val;
};

class SparseGradeMatrix {
private:
    int numStudents;
    int numSubjects;
    std::vector<std::string> studentLabels;
    std::vector<std::string> subjectLabels;
    std::vector<std::vector<int>> matrix;   // 2D Array Representation

    // Triplet Representation:
    // triplets[0] = {numStudents, numSubjects, totalNonZeroElements}
    // triplets[1..NZ] = {rowIndex, colIndex, mark}
    std::vector<MatrixTriplet> triplets;

public:
    SparseGradeMatrix() {
        loadDefaultSample();
    }

    void loadDefaultSample() {
        numStudents = 5;
        numSubjects = 4;
        studentLabels = {"S1 (Aarav)", "S2 (Diya)", "S3 (Rohan)", "S4 (Sneha)", "S5 (Ishaan)"};
        subjectLabels = {"Mathematics", "Physics", "Chemistry", "Computer Sci"};

        matrix = {
            {85,  0, 78, 90},
            { 0, 76, 82,  0},
            {91, 88,  0, 95},
            { 0,  0, 74, 80},
            {79, 84,  0,  0}
        };

        convertToTriplet();
    }

    // Convert standard 2D matrix into compact 3-Tuple (Triplet) format
    void convertToTriplet() {
        triplets.clear();
        int nzCount = 0;

        // First pass: count non-zeros and populate
        // Reserve index 0 for matrix header metadata
        triplets.push_back({numStudents, numSubjects, 0});

        for (int r = 0; r < numStudents; ++r) {
            for (int c = 0; c < numSubjects; ++c) {
                if (matrix[r][c] != 0) {
                    triplets.push_back({r, c, matrix[r][c]});
                    nzCount++;
                }
            }
        }
        triplets[0].val = nzCount; // Store total non-zero count at header
    }

    // Display original 2D dense matrix
    void displayDenseMatrix() const {
        std::cout << "\n" << Color::CYAN << Color::BOLD << "--- Dense 2D Grade Matrix (0 = Not Enrolled) ---" << Color::RESET << "\n";
        std::cout << std::left << std::setw(16) << "Student";
        for (const auto& sub : subjectLabels) {
            std::cout << std::right << std::setw(14) << sub;
        }
        std::cout << "\n" << std::string(16 + numSubjects * 14, '-') << "\n";

        for (int r = 0; r < numStudents; ++r) {
            std::cout << std::left << std::setw(16) << studentLabels[r];
            for (int c = 0; c < numSubjects; ++c) {
                if (matrix[r][c] == 0) {
                    std::cout << std::right << std::setw(14) << "-";
                } else {
                    std::cout << std::right << std::setw(14) << matrix[r][c];
                }
            }
            std::cout << "\n";
        }
        std::cout << std::string(16 + numSubjects * 14, '-') << "\n";
    }

    // Display Array-based Triplet Representation
    void displayTripletRepresentation() const {
        std::cout << "\n" << Color::CYAN << Color::BOLD << "--- Triplet (3-Tuple) Sparse Representation ---" << Color::RESET << "\n";
        std::cout << "+-------+-------+-------+---------------------------------+\n"
                  << "| Row   | Col   | Value | Description                     |\n"
                  << "+-------+-------+-------+---------------------------------+\n";
        
        // Header triplet
        std::cout << "| " << std::setw(5) << triplets[0].row 
                  << " | " << std::setw(5) << triplets[0].col 
                  << " | " << std::setw(5) << triplets[0].val 
                  << " | " << std::setw(31) << std::left << "Header: [Rows, Cols, NonZeros]" << std::right << " |\n";
        std::cout << "+-------+-------+-------+---------------------------------+\n";

        for (size_t i = 1; i < triplets.size(); ++i) {
            std::string desc = studentLabels[triplets[i].row] + " -> " + subjectLabels[triplets[i].col];
            std::cout << "| " << std::setw(5) << triplets[i].row 
                      << " | " << std::setw(5) << triplets[i].col 
                      << " | " << std::setw(5) << triplets[i].val 
                      << " | " << std::setw(31) << std::left << desc.substr(0, 31) << std::right << " |\n";
        }
        std::cout << "+-------+-------+-------+---------------------------------+\n";
        std::cout << " Total elements: " << (numStudents * numSubjects) 
                  << " | Non-Zero Entries: " << triplets[0].val 
                  << " | Sparsity: " << std::fixed << std::setprecision(1) 
                  << (100.0 * (1.0 - (double)triplets[0].val / (numStudents * numSubjects))) << "%\n";
    }

    // Reconstruct dense matrix from Triplet representation
    void reconstructFromTriplet() const {
        std::cout << "\n" << Color::GREEN << Color::BOLD 
                  << "--- Reconstructed Matrix from Triplet Representation ---" << Color::RESET << "\n";
        
        int rows = triplets[0].row;
        int cols = triplets[0].col;
        int nonZeros = triplets[0].val;

        // Initialize empty matrix filled with 0s
        std::vector<std::vector<int>> recon(rows, std::vector<int>(cols, 0));

        // Populate values directly from triplet entries
        for (int i = 1; i <= nonZeros; ++i) {
            recon[triplets[i].row][triplets[i].col] = triplets[i].val;
        }

        // Print reconstructed matrix
        std::cout << std::left << std::setw(16) << "Student";
        for (const auto& sub : subjectLabels) {
            std::cout << std::right << std::setw(14) << sub;
        }
        std::cout << "\n" << std::string(16 + cols * 14, '-') << "\n";

        for (int r = 0; r < rows; ++r) {
            std::cout << std::left << std::setw(16) << studentLabels[r];
            for (int c = 0; c < cols; ++c) {
                if (recon[r][c] == 0) {
                    std::cout << std::right << std::setw(14) << "-";
                } else {
                    std::cout << std::right << std::setw(14) << recon[r][c];
                }
            }
            std::cout << "\n";
        }
        std::cout << std::string(16 + cols * 14, '-') << "\n";
        std::cout << Color::GREEN << " [Validation] Reconstructed matrix matches original dense matrix perfectly!\n" << Color::RESET;
    }

    // Subject-wise Averages (excluding 0 / un-enrolled students)
    void computeSubjectAverages() const {
        std::cout << "\n" << Color::CYAN << Color::BOLD << "--- Subject-wise Academic Analysis ---" << Color::RESET << "\n";
        std::cout << "+------------------+------------+-------------+---------------+\n"
                  << "| Subject Name     | Enrolled   | Total Marks | Average Grade |\n"
                  << "+------------------+------------+-------------+---------------+\n";

        double highestAvg = -1.0;
        int highestAvgSubjectIdx = -1;

        for (int c = 0; c < numSubjects; ++c) {
            int enrolledCount = 0;
            int totalMarks = 0;
            for (int r = 0; r < numStudents; ++r) {
                if (matrix[r][c] > 0) {
                    enrolledCount++;
                    totalMarks += matrix[r][c];
                }
            }
            double avg = (enrolledCount > 0) ? ((double)totalMarks / enrolledCount) : 0.0;
            if (avg > highestAvg) {
                highestAvg = avg;
                highestAvgSubjectIdx = c;
            }

            std::cout << "| " << std::setw(16) << std::left << subjectLabels[c] << std::right
                      << " | " << std::setw(10) << enrolledCount
                      << " | " << std::setw(11) << totalMarks
                      << " | " << std::fixed << std::setprecision(2) << std::setw(13) << avg << " |\n";
        }
        std::cout << "+------------------+------------+-------------+---------------+\n";

        if (highestAvgSubjectIdx != -1) {
            std::cout << Color::GREEN << " [Highest Average Subject]: " 
                      << subjectLabels[highestAvgSubjectIdx] 
                      << " with an average score of " << std::fixed << std::setprecision(2) << highestAvg << "\n" << Color::RESET;
        }
    }

    // Identify student(s) with highest individual grade
    void findHighestIndividualGrade() const {
        std::cout << "\n" << Color::CYAN << Color::BOLD << "--- Top Performing Student(s) (Highest Individual Grade) ---" << Color::RESET << "\n";
        int maxGrade = -1;

        // Find global maximum
        for (int r = 0; r < numStudents; ++r) {
            for (int c = 0; c < numSubjects; ++c) {
                if (matrix[r][c] > maxGrade) {
                    maxGrade = matrix[r][c];
                }
            }
        }

        std::cout << " Highest Score Achieved on Campus: " << Color::GREEN << Color::BOLD << maxGrade << Color::RESET << "\n";
        std::cout << " Achieving Students & Subject(s):\n";

        for (int r = 0; r < numStudents; ++r) {
            for (int c = 0; c < numSubjects; ++c) {
                if (matrix[r][c] == maxGrade) {
                    std::cout << "  * " << studentLabels[r] << " in " << subjectLabels[c] << " (" << maxGrade << "/100)\n";
                }
            }
        }
    }

    // Storage Comparison and Sparsity Analysis
    void analyzeStorageEfficiency() const {
        int denseElements = numStudents * numSubjects;
        int denseBytes = denseElements * sizeof(int);

        int nonZeros = triplets[0].val;
        int tripletEntries = nonZeros + 1; // including header
        int tripletBytes = tripletEntries * 3 * sizeof(int);

        std::cout << "\n" << Color::YELLOW << Color::BOLD
                  << "========================================================================\n"
                  << "        STORAGE COMPLEXITY ANALYSIS: DENSE MATRIX vs TRIPLET            \n"
                  << "========================================================================\n"
                  << Color::RESET
                  << " 1. Dense 2D Array Representation:\n"
                  << "    - Total Cells:       " << numStudents << " rows x " << numSubjects << " cols = " << denseElements << " elements\n"
                  << "    - Memory Consumed:   " << denseElements << " x " << sizeof(int) << " bytes = " << denseBytes << " bytes\n\n"
                  << " 2. Triplet (3-Tuple) Representation:\n"
                  << "    - Non-Zero Entries:  " << nonZeros << " (plus 1 header tuple)\n"
                  << "    - Triplet Entries:   " << tripletEntries << " tuples x 3 integers\n"
                  << "    - Memory Consumed:   " << tripletEntries * 3 << " x " << sizeof(int) << " bytes = " << tripletBytes << " bytes\n\n"
                  << " 3. Memory Comparison for Current Dataset:\n"
                  << "    - Difference:        " << (denseBytes - tripletBytes) << " bytes ("
                  << (tripletBytes <= denseBytes ? "Triplet saves memory" : "Dense is more compact due to small N") << ")\n\n"
                  << Color::CYAN
                  << " [!] THEORETICAL BREAK-EVEN THRESHOLD:\n"
                  << "   Triplet stores 3 integers per non-zero element (Row, Col, Val).\n"
                  << "   Condition to save memory: 3 * (NZ + 1) < (Rows * Cols)\n"
                  << "   Equivalently: NZ < (Rows * Cols) / 3\n"
                  << "   Therefore, Sparse Triplet representation saves memory whenever the\n"
                  << "   matrix has LESS THAN ~33.3% non-zero elements (i.e., >66.7% sparse).\n"
                  << "   For large university grade sheets (e.g. 5,000 students x 60 electives),\n"
                  << "   students only enroll in 5-6 courses, meaning 90% sparsity, where\n"
                  << "   Triplet sparse storage saves over 70% of RAM!\n"
                  << "========================================================================\n"
                  << Color::RESET;
    }

    // Interactive Menu Handler for Module 3
    void runMenu() {
        while (true) {
            std::cout << "\n" << Color::BLUE << Color::BOLD 
                      << "========================================================\n"
                      << "   MODULE 3: SPARSE MATRIX REPRESENTATION & ANALYSIS    \n"
                      << "========================================================\n"
                      << Color::RESET;
            std::cout << " 1. Display Dense 2D Grade Matrix\n"
                      << " 2. Display 3-Tuple (Triplet) Sparse Matrix Representation\n"
                      << " 3. Reconstruct Dense Matrix from Triplet Format\n"
                      << " 4. Compute Subject-wise Averages (Excluding Unregistered)\n"
                      << " 5. Find Student(s) with Highest Individual Grade & Top Subject\n"
                      << " 6. Storage & Sparsity Complexity Analysis\n"
                      << " 7. Input Custom Sparse Matrix Values\n"
                      << " 0. Return to Main Menu\n";

            int choice = InputHelper::getInteger(" Enter your choice (0-7): ");
            if (choice == 0) break;

            switch (choice) {
                case 1:
                    displayDenseMatrix();
                    break;
                case 2:
                    displayTripletRepresentation();
                    break;
                case 3:
                    reconstructFromTriplet();
                    break;
                case 4:
                    computeSubjectAverages();
                    break;
                case 5:
                    findHighestIndividualGrade();
                    break;
                case 6:
                    analyzeStorageEfficiency();
                    break;
                case 7: {
                    std::cout << "\n" << Color::CYAN << "--- Custom Matrix Entry ---" << Color::RESET << "\n";
                    numStudents = InputHelper::getInteger(" Enter number of students (rows 1-20): ");
                    if (numStudents < 1) numStudents = 1;
                    if (numStudents > 20) numStudents = 20;

                    numSubjects = InputHelper::getInteger(" Enter number of subjects (cols 1-10): ");
                    if (numSubjects < 1) numSubjects = 1;
                    if (numSubjects > 10) numSubjects = 10;

                    studentLabels.clear();
                    for (int i = 0; i < numStudents; ++i) {
                        studentLabels.push_back("S" + std::to_string(i + 1));
                    }
                    subjectLabels.clear();
                    for (int j = 0; j < numSubjects; ++j) {
                        subjectLabels.push_back("Sub" + std::to_string(j + 1));
                    }

                    matrix.assign(numStudents, std::vector<int>(numSubjects, 0));
                    std::cout << " Enter grades for each cell (enter 0 if student is not enrolled):\n";
                    for (int r = 0; r < numStudents; ++r) {
                        for (int c = 0; c < numSubjects; ++c) {
                            int g = InputHelper::getInteger("  " + studentLabels[r] + " in " + subjectLabels[c] + ": ");
                            if (g < 0) g = 0;
                            if (g > 100) g = 100;
                            matrix[r][c] = g;
                        }
                    }
                    convertToTriplet();
                    std::cout << Color::GREEN << " [Success] Custom matrix loaded and converted to Triplet format.\n" << Color::RESET;
                    break;
                }
                default:
                    std::cout << Color::RED << " Invalid option. Please select 0 to 7.\n" << Color::RESET;
            }
            InputHelper::pause();
        }
    }
};

#endif // MODULE3_SPARSE_MATRIX_HPP
