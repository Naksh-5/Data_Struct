#ifndef MODULE8_HASH_TABLE_HPP
#define MODULE8_HASH_TABLE_HPP

#include "Common.hpp"

enum SlotState { EMPTY, OCCUPIED, DELETED };

struct HashSlot {
    int key;
    Student data;
    SlotState state;

    HashSlot() : key(-1), state(EMPTY) {}
};

class StudentHashTable {
private:
    int capacity;
    int occupiedCount;
    std::vector<HashSlot> table;

    // Primary Hash Function: Division Method
    int hashFunction(int key) const {
        return (key % capacity + capacity) % capacity;
    }

public:
    StudentHashTable(int cap = 11) : capacity(cap), occupiedCount(0) {
        table.resize(capacity);
    }

    // Load factor alpha = n / m
    double getLoadFactor() const {
        return static_cast<double>(occupiedCount) / capacity;
    }

    // 1. Insert Student Record with Linear Probing
    // Prevents duplicates. Skips or reuses DELETED slots.
    bool insert(const Student& s, int& probes) {
        probes = 0;
        if (occupiedCount >= capacity) {
            std::cout << Color::RED << " [Error] Hash Table is completely full! Cannot insert ID " << s.id << ".\n" << Color::RESET;
            return false;
        }

        // First check if key already exists to prevent duplicate IDs
        int dummyProbes = 0;
        if (search(s.id, dummyProbes) != -1) {
            std::cout << Color::RED << " [Error] Duplicate Student ID " << s.id << " rejected by Hash Table!\n" << Color::RESET;
            return false;
        }

        int homeIndex = hashFunction(s.id);
        int firstDeletedIndex = -1;

        for (int i = 0; i < capacity; ++i) {
            probes++;
            int idx = (homeIndex + i) % capacity;

            if (table[idx].state == EMPTY) {
                // If we previously found a deleted slot, reuse it!
                int targetIdx = (firstDeletedIndex != -1) ? firstDeletedIndex : idx;
                table[targetIdx].key = s.id;
                table[targetIdx].data = s;
                table[targetIdx].state = OCCUPIED;
                occupiedCount++;
                return true;
            } else if (table[idx].state == DELETED) {
                if (firstDeletedIndex == -1) {
                    firstDeletedIndex = idx; // Remember first tombstone
                }
            }
        }

        // If loop finished, but we found a deleted tombstone
        if (firstDeletedIndex != -1) {
            table[firstDeletedIndex].key = s.id;
            table[firstDeletedIndex].data = s;
            table[firstDeletedIndex].state = OCCUPIED;
            occupiedCount++;
            return true;
        }

        return false;
    }

    // 2. Search for Record by ID using Linear Probing
    // MUST NOT stop at DELETED slots! Probing continues until EMPTY slot or key found.
    int search(int key, int& probes) const {
        probes = 0;
        int homeIndex = hashFunction(key);

        for (int i = 0; i < capacity; ++i) {
            probes++;
            int idx = (homeIndex + i) % capacity;

            if (table[idx].state == EMPTY) {
                // Unoccupied slot encountered -> key definitely not present
                return -1;
            }
            if (table[idx].state == OCCUPIED && table[idx].key == key) {
                return idx; // Found
            }
            // If DELETED, continue probing!
        }
        return -1;
    }

    // 3. Update Record
    bool update(int key) {
        int probes = 0;
        int idx = search(key, probes);
        if (idx == -1) {
            std::cout << Color::RED << " [Error] Student ID " << key << " not found in Hash Table.\n" << Color::RESET;
            return false;
        }

        Student& s = table[idx].data;
        std::cout << Color::CYAN << " [Updating Student in Hash Table: " << s.name << "]\n" << Color::RESET;
        std::string newContact = InputHelper::getString(" Enter new Contact Number: ");
        if (!newContact.empty()) s.contact = newContact;
        std::string newDiv = InputHelper::getString(" Enter new Division: ");
        if (!newDiv.empty()) s.division = newDiv;

        std::cout << Color::GREEN << " [Success] Hash table record updated successfully.\n" << Color::RESET;
        return true;
    }

    // 4. Delete Record (Marks slot as DELETED / Tombstone)
    bool remove(int key) {
        int probes = 0;
        int idx = search(key, probes);
        if (idx == -1) {
            std::cout << Color::RED << " [Error] Student ID " << key << " does not exist in Hash Table.\n" << Color::RESET;
            return false;
        }

        table[idx].state = DELETED;
        occupiedCount--;
        std::cout << Color::GREEN << " [Success] Student ID " << key << " marked as DELETED (Tombstone set at index " 
                  << idx << " to preserve probing chain integrity).\n" << Color::RESET;
        return true;
    }

    // 5. Display Full Hash Table Map
    void display() const {
        std::cout << "\n" << Color::CYAN << Color::BOLD 
                  << "--- Open Addressing Hash Table (Linear Probing | Size: " << capacity << ") ---" 
                  << Color::RESET << "\n";
        std::cout << "+-------+-----------+---------+--------------------+--------------------+\n"
                  << "| Slot  | Status    | Key/ID  | Home Hash Index    | Student Name       |\n"
                  << "+-------+-----------+---------+--------------------+--------------------+\n";

        for (int i = 0; i < capacity; ++i) {
            std::cout << "| " << std::setw(5) << i << " | ";
            if (table[i].state == OCCUPIED) {
                int home = hashFunction(table[i].key);
                std::string probeDist = (home == i) ? "Home [0]" : ("Probed [+" + std::to_string((i - home + capacity) % capacity) + "]");
                std::cout << Color::GREEN << std::setw(9) << "OCCUPIED" << Color::RESET
                          << " | " << std::setw(7) << table[i].key
                          << " | " << std::setw(18) << (std::to_string(home) + " (" + probeDist + ")")
                          << " | " << std::setw(18) << std::left << table[i].data.name.substr(0, 18) << std::right
                          << " |\n";
            } else if (table[i].state == DELETED) {
                std::cout << Color::MAGENTA << std::setw(9) << "DELETED" << Color::RESET
                          << " | " << std::setw(7) << "-"
                          << " | " << std::setw(18) << "Tombstone Marker"
                          << " | " << std::setw(18) << "-"
                          << " |\n";
            } else {
                std::cout << Color::YELLOW << std::setw(9) << "EMPTY" << Color::RESET
                          << " | " << std::setw(7) << "-"
                          << " | " << std::setw(18) << "-"
                          << " | " << std::setw(18) << "-"
                          << " |\n";
            }
        }
        std::cout << "+-------+-----------+---------+--------------------+--------------------+\n";
        std::cout << " Active Records: " << occupiedCount << " / " << capacity 
                  << " | Current Load Factor (alpha): " << std::fixed << std::setprecision(2) << getLoadFactor() 
                  << " (" << (getLoadFactor() * 100.0) << "%)\n";
    }

    // Populate hash table from shared student vector
    void populateFromVector(const std::vector<Student>& students) {
        for (const auto& s : students) {
            int probes = 0;
            insert(s, probes);
        }
    }

    // Interactive Menu Handler for Module 8
    void runMenu(const std::vector<Student>& sharedStudents) {
        while (true) {
            std::cout << "\n" << Color::BLUE << Color::BOLD 
                      << "========================================================\n"
                      << "      MODULE 8: FAST RECORD RETRIEVAL USING HASHING     \n"
                      << "========================================================\n"
                      << Color::RESET;
            std::cout << " 1. Display Hash Table & Load Factor\n"
                      << " 2. Insert Student into Hash Table\n"
                      << " 3. Search Student by ID (Displays Probes & Time)\n"
                      << " 4. Update Student Record in Hash Table\n"
                      << " 5. Delete Student Record (Demonstrates Tombstones)\n"
                      << " 6. Sync / Rehash from Global Student Database\n"
                      << " 7. Theoretical Analysis: Hash Function, Load Factor & Collisions\n"
                      << " 0. Return to Main Menu\n";

            int choice = InputHelper::getInteger(" Enter your choice (0-7): ");
            if (choice == 0) break;

            switch (choice) {
                case 1:
                    display();
                    break;
                case 2: {
                    int id = InputHelper::getInteger(" Enter Student ID: ");
                    std::string name = InputHelper::getString(" Enter Student Name: ");
                    std::string div = InputHelper::getString(" Enter Division: ");
                    std::string contact = InputHelper::getString(" Enter Contact: ");
                    Student s(id, name, div, contact, {80, 85, 90, 88});
                    int probes = 0;
                    if (insert(s, probes)) {
                        std::cout << Color::GREEN << " [Success] Inserted ID " << id << " with " << probes << " probe(s).\n" << Color::RESET;
                    }
                    break;
                }
                case 3: {
                    int id = InputHelper::getInteger(" Enter Student ID to search: ");
                    int probes = 0;
                    int idx = search(id, probes);
                    if (idx != -1) {
                        std::cout << Color::GREEN << " [Found] Record located at Slot " << idx 
                                  << " with " << probes << " probe(s):\n" << Color::RESET;
                        std::cout << Color::CYAN
                                  << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                                  << "| ID     | Name               | Div  | Contact      | Average | Marks (M, P, C, CS) |\n"
                                  << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                                  << Color::RESET;
                        table[idx].data.printRow();
                        std::cout << Color::CYAN << "+--------+--------------------+------+--------------+---------+---------------------+\n" << Color::RESET;
                    } else {
                        std::cout << Color::RED << " [Not Found] Student ID " << id << " not present (" << probes << " probes).\n" << Color::RESET;
                    }
                    break;
                }
                case 4: {
                    int id = InputHelper::getInteger(" Enter Student ID to update: ");
                    update(id);
                    break;
                }
                case 5: {
                    int id = InputHelper::getInteger(" Enter Student ID to delete: ");
                    remove(id);
                    break;
                }
                case 6: {
                    populateFromVector(sharedStudents);
                    std::cout << Color::GREEN << " [Synced] Loaded " << sharedStudents.size() << " student records into Hash Table.\n" << Color::RESET;
                    display();
                    break;
                }
                case 7: {
                    std::cout << "\n" << Color::YELLOW << Color::BOLD
                              << "========================================================================================\n"
                              << "                       HASHING ALGORITHMIC & THEORETICAL ANALYSIS                       \n"
                              << "========================================================================================\n"
                              << Color::RESET
                              << " 1. HASH FUNCTION (Division Method):\n"
                              << "    h(k) = k mod M (where M = table size, chosen as a prime number like 11 or 13\n"
                              << "    to minimize clustering caused by common divisors in key patterns).\n\n"
                              << " 2. COLLISION RESOLUTION (Linear Probing Open Addressing):\n"
                              << "    h(k, i) = (h(k) + i) mod M for i = 0, 1, 2, ... M-1.\n"
                              << "    - Pro: Excellent cache locality and zero pointer overhead.\n"
                              << "    - Con: Subject to Primary Clustering (blocks of contiguous occupied slots grow,\n"
                              << "      increasing probe times for future collisions).\n\n"
                              << " 3. TOMBSTONES / DELETED SLOTS:\n"
                              << "    - When a slot is cleared, resetting it to EMPTY would break the linear probe\n"
                              << "      chain, causing future searches for colliding elements to falsely report NOT FOUND.\n"
                              << "    - Setting the status to DELETED ensures searches continue probing through the slot,\n"
                              << "      while insertions can freely overwrite it.\n\n"
                              << " 4. LOAD FACTOR (alpha = N / M):\n"
                              << "    - As alpha approaches 1.0 (or >0.7 in open addressing), clustering escalates rapidly.\n"
                              << "    - Average Search Cost ~ (1 + 1 / (1 - alpha)) / 2.\n"
                              << "    - Time Complexity:\n"
                              << "        * Best Case:    O(1) [Zero collisions, direct home slot]\n"
                              << "        * Average Case: O(1) [Assuming alpha <= 0.7]\n"
                              << "        * Worst Case:   O(N) [All keys hash to same slot or table nearly full]\n"
                              << "========================================================================================\n";
                    break;
                }
                default:
                    std::cout << Color::RED << " Invalid option. Please select 0 to 7.\n" << Color::RESET;
            }
            InputHelper::pause();
        }
    }
};

#endif // MODULE8_HASH_TABLE_HPP
