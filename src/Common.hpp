#ifndef COMMON_HPP
#define COMMON_HPP

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>
#include <limits>
#include <sstream>
#include <cmath>

// ============================================================================
// ANSI Color Escape Sequences for clear and pleasant terminal presentation
// ============================================================================
namespace Color {
    const std::string RESET   = "\033[0m";
    const std::string BOLD    = "\033[1m";
    const std::string RED     = "\033[31m";
    const std::string GREEN   = "\033[32m";
    const std::string YELLOW  = "\033[33m";
    const std::string BLUE    = "\033[34m";
    const std::string MAGENTA = "\033[35m";
    const std::string CYAN    = "\033[36m";
    const std::string WHITE   = "\033[37m";
}

// ============================================================================
// Core Student Model used across multiple modules to maintain record consistency
// ============================================================================
struct Student {
    int id;                      // Unique student integer ID
    std::string name;            // Student's full name
    std::string division;        // Assigned division (e.g., "A", "B")
    std::string contact;         // Contact number as string to preserve leading zeros
    std::vector<double> marks;   // Scores in registered subjects (Maths, Physics, Chem, CS)

    Student() : id(0), name(""), division(""), contact("") {
        marks = {0.0, 0.0, 0.0, 0.0};
    }

    Student(int sId, const std::string& sName, const std::string& sDiv, 
            const std::string& sContact, const std::vector<double>& sMarks)
        : id(sId), name(sName), division(sDiv), contact(sContact), marks(sMarks) {}

    // Computes aggregate total marks
    double getTotalMarks() const {
        double sum = 0.0;
        for (double m : marks) sum += m;
        return sum;
    }

    // Computes percentage / average marks
    double getAverageMarks() const {
        if (marks.empty()) return 0.0;
        return getTotalMarks() / marks.size();
    }

    // Formatted row representation for tables
    void printRow() const {
        std::cout << "| " << std::setw(6) << id 
                  << " | " << std::setw(18) << std::left << name.substr(0, 18) << std::right
                  << " | " << std::setw(4) << division 
                  << " | " << std::setw(12) << contact
                  << " | " << std::fixed << std::setprecision(1) << std::setw(6) << getAverageMarks() << "%"
                  << " | ";
        for (size_t i = 0; i < marks.size(); ++i) {
            std::cout << std::setw(4) << (int)marks[i] << (i + 1 < marks.size() ? "," : "");
        }
        std::cout << " |\n";
    }
};

// ============================================================================
// Safe Input Helper Functions (Defends against infinite loops on invalid input)
// ============================================================================
namespace InputHelper {
    inline void clearBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }

    inline int getInteger(const std::string& prompt) {
        int value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                clearBuffer();
                return value;
            }
            std::cout << Color::RED << " [!] Invalid input. Please enter a valid integer.\n" << Color::RESET;
            clearBuffer();
        }
    }

    inline double getDouble(const std::string& prompt) {
        double value;
        while (true) {
            std::cout << prompt;
            if (std::cin >> value) {
                clearBuffer();
                return value;
            }
            std::cout << Color::RED << " [!] Invalid input. Please enter a valid number.\n" << Color::RESET;
            clearBuffer();
        }
    }

    inline std::string getString(const std::string& prompt) {
        std::string line;
        std::cout << prompt;
        std::getline(std::cin, line);
        // Trim leading and trailing whitespaces
        size_t first = line.find_first_not_of(" \t\r\n");
        if (first == std::string::npos) return "";
        size_t last = line.find_last_not_of(" \t\r\n");
        return line.substr(first, (last - first + 1));
    }

    inline void pause() {
        std::cout << Color::YELLOW << "\nPress Enter to continue..." << Color::RESET;
        std::string dummy;
        std::getline(std::cin, dummy);
    }
}

#endif // COMMON_HPP
