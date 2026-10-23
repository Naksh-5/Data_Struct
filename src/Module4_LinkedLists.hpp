#ifndef MODULE4_LINKED_LISTS_HPP
#define MODULE4_LINKED_LISTS_HPP

#include "Common.hpp"

// ============================================================================
// PART A: SINGLY LINKED LIST (Campus Maintenance Requests)
// ============================================================================
struct MaintenanceRequest {
    int requestId;
    std::string requesterName;
    std::string location;
    std::string description;
    std::string status; // "Pending", "In Progress", "Completed"

    MaintenanceRequest(int id = 0, const std::string& name = "", const std::string& loc = "", 
                       const std::string& desc = "", const std::string& stat = "Pending")
        : requestId(id), requesterName(name), location(loc), description(desc), status(stat) {}
};

struct SLLNode {
    MaintenanceRequest data;
    SLLNode* next;
    SLLNode(const MaintenanceRequest& req) : data(req), next(nullptr) {}
};

class SinglyLinkedList {
private:
    SLLNode* head;

public:
    SinglyLinkedList() : head(nullptr) {}
    ~SinglyLinkedList() { clear(); }

    void clear() {
        SLLNode* curr = head;
        while (curr) {
            SLLNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
        head = nullptr;
    }

    bool exists(int id) const {
        SLLNode* curr = head;
        while (curr) {
            if (curr->data.requestId == id) return true;
            curr = curr->next;
        }
        return false;
    }

    // Add request at the end of the list
    void addRequest(const MaintenanceRequest& req) {
        if (exists(req.requestId)) {
            std::cout << Color::RED << " [Error] Request ID " << req.requestId << " already exists!\n" << Color::RESET;
            return;
        }
        SLLNode* newNode = new SLLNode(req);
        if (!head) {
            head = newNode;
        } else {
            SLLNode* curr = head;
            while (curr->next) {
                curr = curr->next;
            }
            curr->next = newNode;
        }
        std::cout << Color::GREEN << " [Success] Maintenance request #" << req.requestId << " logged.\n" << Color::RESET;
    }

    // Delete request by ID
    bool deleteRequest(int id) {
        if (!head) {
            std::cout << Color::YELLOW << " [Notice] Request list is empty. Nothing to delete.\n" << Color::RESET;
            return false;
        }

        // Case 1: Head node deletion
        if (head->data.requestId == id) {
            SLLNode* temp = head;
            head = head->next;
            delete temp;
            std::cout << Color::GREEN << " [Success] Request #" << id << " deleted.\n" << Color::RESET;
            return true;
        }

        // Case 2: Intermediate or tail node deletion
        SLLNode* curr = head;
        while (curr->next && curr->next->data.requestId != id) {
            curr = curr->next;
        }

        if (curr->next) {
            SLLNode* temp = curr->next;
            curr->next = curr->next->next;
            delete temp;
            std::cout << Color::GREEN << " [Success] Request #" << id << " deleted.\n" << Color::RESET;
            return true;
        }

        std::cout << Color::RED << " [Error] Request ID " << id << " not found.\n" << Color::RESET;
        return false;
    }

    // Search for a request by ID
    SLLNode* searchRequest(int id) const {
        SLLNode* curr = head;
        while (curr) {
            if (curr->data.requestId == id) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    // Update status of an existing request
    bool updateStatus(int id, const std::string& newStatus) {
        SLLNode* node = searchRequest(id);
        if (!node) {
            std::cout << Color::RED << " [Error] Request ID " << id << " not found.\n" << Color::RESET;
            return false;
        }
        node->data.status = newStatus;
        std::cout << Color::GREEN << " [Success] Request #" << id << " status updated to \"" << newStatus << "\".\n" << Color::RESET;
        return true;
    }

    // Display all requests in formatted table
    void displayAll() const {
        if (!head) {
            std::cout << Color::YELLOW << " [Notice] No maintenance requests logged.\n" << Color::RESET;
            return;
        }
        std::cout << "\n" << Color::CYAN << Color::BOLD
                  << "+-----+--------------------+--------------------+---------------------------+--------------+\n"
                  << "| ID  | Requester          | Location           | Description               | Status       |\n"
                  << "+-----+--------------------+--------------------+---------------------------+--------------+\n"
                  << Color::RESET;
        SLLNode* curr = head;
        while (curr) {
            std::cout << "| " << std::setw(3) << curr->data.requestId
                      << " | " << std::setw(18) << std::left << curr->data.requesterName.substr(0, 18) << std::right
                      << " | " << std::setw(18) << std::left << curr->data.location.substr(0, 18) << std::right
                      << " | " << std::setw(25) << std::left << curr->data.description.substr(0, 25) << std::right
                      << " | " << std::setw(12) << std::left << curr->data.status << std::right
                      << " |\n";
            curr = curr->next;
        }
        std::cout << Color::CYAN << "+-----+--------------------+--------------------+---------------------------+--------------+\n" << Color::RESET;
    }

    // Count pending requests
    int countPending() const {
        int count = 0;
        SLLNode* curr = head;
        while (curr) {
            if (curr->data.status == "Pending") {
                count++;
            }
            curr = curr->next;
        }
        return count;
    }
};

// ============================================================================
// PART B: DOUBLY LINKED LIST (Campus Shuttle Route)
// ============================================================================
struct ShuttleStation {
    int stationId;
    std::string stationName;

    ShuttleStation(int id = 0, const std::string& name = "") : stationId(id), stationName(name) {}
};

struct DLLNode {
    ShuttleStation data;
    DLLNode* prev;
    DLLNode* next;
    DLLNode(const ShuttleStation& st) : data(st), prev(nullptr), next(nullptr) {}
};

class DoublyLinkedList {
private:
    DLLNode* head;
    DLLNode* tail;

public:
    DoublyLinkedList() : head(nullptr), tail(nullptr) {}
    ~DoublyLinkedList() { clear(); }

    void clear() {
        DLLNode* curr = head;
        while (curr) {
            DLLNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
        head = tail = nullptr;
    }

    bool exists(int id) const {
        DLLNode* curr = head;
        while (curr) {
            if (curr->data.stationId == id) return true;
            curr = curr->next;
        }
        return false;
    }

    // 1. Insert at Beginning
    void insertAtBeginning(const ShuttleStation& st) {
        if (exists(st.stationId)) {
            std::cout << Color::RED << " [Error] Station ID " << st.stationId << " already exists!\n" << Color::RESET;
            return;
        }
        DLLNode* newNode = new DLLNode(st);
        if (!head) {
            head = tail = newNode;
        } else {
            newNode->next = head;
            head->prev = newNode;
            head = newNode;
        }
        std::cout << Color::GREEN << " [Success] Station \"" << st.stationName << "\" inserted at beginning.\n" << Color::RESET;
    }

    // 2. Insert at End
    void insertAtEnd(const ShuttleStation& st) {
        if (exists(st.stationId)) {
            std::cout << Color::RED << " [Error] Station ID " << st.stationId << " already exists!\n" << Color::RESET;
            return;
        }
        DLLNode* newNode = new DLLNode(st);
        if (!tail) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
        std::cout << Color::GREEN << " [Success] Station \"" << st.stationName << "\" inserted at end.\n" << Color::RESET;
    }

    // 3. Insert at Specified Position (1-based index)
    void insertAtPosition(const ShuttleStation& st, int pos) {
        if (pos <= 1) {
            insertAtBeginning(st);
            return;
        }
        if (exists(st.stationId)) {
            std::cout << Color::RED << " [Error] Station ID " << st.stationId << " already exists!\n" << Color::RESET;
            return;
        }

        DLLNode* curr = head;
        for (int i = 1; i < pos - 1 && curr; ++i) {
            curr = curr->next;
        }

        if (!curr || !curr->next) {
            // Append at end if position is beyond end
            insertAtEnd(st);
            return;
        }

        DLLNode* newNode = new DLLNode(st);
        newNode->next = curr->next;
        newNode->prev = curr;
        curr->next->prev = newNode;
        curr->next = newNode;
        std::cout << Color::GREEN << " [Success] Station \"" << st.stationName << "\" inserted at position " << pos << ".\n" << Color::RESET;
    }

    // 4. Delete Station by ID
    bool deleteStation(int id) {
        if (!head) {
            std::cout << Color::YELLOW << " [Notice] Route is empty.\n" << Color::RESET;
            return false;
        }

        DLLNode* curr = head;
        while (curr && curr->data.stationId != id) {
            curr = curr->next;
        }

        if (!curr) {
            std::cout << Color::RED << " [Error] Station ID " << id << " not found.\n" << Color::RESET;
            return false;
        }

        if (curr == head && curr == tail) {
            head = tail = nullptr;
        } else if (curr == head) {
            head = head->next;
            head->prev = nullptr;
        } else if (curr == tail) {
            tail = tail->prev;
            tail->next = nullptr;
        } else {
            curr->prev->next = curr->next;
            curr->next->prev = curr->prev;
        }

        delete curr;
        std::cout << Color::GREEN << " [Success] Station ID " << id << " removed from route.\n" << Color::RESET;
        return true;
    }

    // 5. Search for a Station
    DLLNode* searchStation(int id) const {
        DLLNode* curr = head;
        while (curr) {
            if (curr->data.stationId == id) return curr;
            curr = curr->next;
        }
        return nullptr;
    }

    // 6. Display in Forward Order (Head to Tail)
    void displayForward() const {
        if (!head) {
            std::cout << Color::YELLOW << " [Notice] Shuttle route is empty.\n" << Color::RESET;
            return;
        }
        std::cout << "\n" << Color::CYAN << Color::BOLD << " Forward Shuttle Route (Outbound):" << Color::RESET << "\n [START] ";
        DLLNode* curr = head;
        while (curr) {
            std::cout << "-> [" << curr->data.stationId << ": " << curr->data.stationName << "] ";
            curr = curr->next;
        }
        std::cout << "-> [TERMINUS]\n";
    }

    // 7. Display in Reverse Order (Tail to Head)
    void displayReverse() const {
        if (!tail) {
            std::cout << Color::YELLOW << " [Notice] Shuttle route is empty.\n" << Color::RESET;
            return;
        }
        std::cout << "\n" << Color::MAGENTA << Color::BOLD << " Reverse Shuttle Route (Inbound):" << Color::RESET << "\n [TERMINUS] ";
        DLLNode* curr = tail;
        while (curr) {
            std::cout << "-> [" << curr->data.stationId << ": " << curr->data.stationName << "] ";
            curr = curr->prev;
        }
        std::cout << "-> [START]\n";
    }
};

// ============================================================================
// PART C: SINGLY CIRCULAR LINKED LIST (Campus Announcement Playlist)
// ============================================================================
struct Announcement {
    int announcementId;
    std::string title;

    Announcement(int id = 0, const std::string& t = "") : announcementId(id), title(t) {}
};

struct CLLNode {
    Announcement data;
    CLLNode* next;
    CLLNode(const Announcement& ann) : data(ann), next(nullptr) {}
};

class CircularLinkedList {
private:
    CLLNode* last; // Tail pointer (last->next is head)

public:
    CircularLinkedList() : last(nullptr) {}
    ~CircularLinkedList() { clear(); }

    void clear() {
        if (!last) return;
        CLLNode* curr = last->next;
        last->next = nullptr; // Break the cycle to avoid infinite loop
        while (curr) {
            CLLNode* temp = curr;
            curr = curr->next;
            delete temp;
        }
        last = nullptr;
    }

    bool exists(int id) const {
        if (!last) return false;
        CLLNode* curr = last->next;
        do {
            if (curr->data.announcementId == id) return true;
            curr = curr->next;
        } while (curr != last->next);
        return false;
    }

    // Add Announcement to playlist
    void addAnnouncement(const Announcement& ann) {
        if (exists(ann.announcementId)) {
            std::cout << Color::RED << " [Error] Announcement ID " << ann.announcementId << " already exists!\n" << Color::RESET;
            return;
        }
        CLLNode* newNode = new CLLNode(ann);
        if (!last) {
            last = newNode;
            last->next = last; // Self-loop
        } else {
            newNode->next = last->next;
            last->next = newNode;
            last = newNode;
        }
        std::cout << Color::GREEN << " [Success] Announcement added to circular playlist: \"" << ann.title << "\"\n" << Color::RESET;
    }

    // Delete Announcement by ID
    bool deleteAnnouncement(int id) {
        if (!last) {
            std::cout << Color::YELLOW << " [Notice] Playlist is empty.\n" << Color::RESET;
            return false;
        }

        CLLNode* head = last->next;

        // Case 1: Only one node in circular list
        if (head == last && head->data.announcementId == id) {
            delete head;
            last = nullptr;
            std::cout << Color::GREEN << " [Success] Sole announcement #" << id << " deleted. Playlist is now empty.\n" << Color::RESET;
            return true;
        }

        // Case 2: Deleting the head node
        if (head->data.announcementId == id) {
            last->next = head->next;
            delete head;
            std::cout << Color::GREEN << " [Success] First announcement #" << id << " deleted.\n" << Color::RESET;
            return true;
        }

        // Case 3: Deleting an intermediate or last node
        CLLNode* prev = head;
        CLLNode* curr = head->next;
        while (curr != head) {
            if (curr->data.announcementId == id) {
                prev->next = curr->next;
                if (curr == last) {
                    last = prev; // Update tail pointer
                }
                delete curr;
                std::cout << Color::GREEN << " [Success] Announcement #" << id << " deleted.\n" << Color::RESET;
                return true;
            }
            prev = curr;
            curr = curr->next;
        }

        std::cout << Color::RED << " [Error] Announcement ID " << id << " not found in playlist.\n" << Color::RESET;
        return false;
    }

    // Search announcement
    bool searchAnnouncement(int id) const {
        if (!last) return false;
        CLLNode* curr = last->next;
        do {
            if (curr->data.announcementId == id) {
                std::cout << Color::GREEN << " [Found] #" << curr->data.announcementId 
                          << ": \"" << curr->data.title << "\"\n" << Color::RESET;
                return true;
            }
            curr = curr->next;
        } while (curr != last->next);
        std::cout << Color::RED << " [Not Found] Announcement ID " << id << " is not in playlist.\n" << Color::RESET;
        return false;
    }

    // Display all announcements once
    void displayOnce() const {
        if (!last) {
            std::cout << Color::YELLOW << " [Notice] Playlist is empty.\n" << Color::RESET;
            return;
        }
        std::cout << "\n" << Color::CYAN << Color::BOLD << "--- Campus Announcements (Single Pass) ---" << Color::RESET << "\n";
        CLLNode* curr = last->next;
        int count = 1;
        do {
            std::cout << " " << count++ << ". [ID: " << curr->data.announcementId << "] " << curr->data.title << "\n";
            curr = curr->next;
        } while (curr != last->next);
    }

    // Cyclic Traversal: Simulate continuous looping (e.g. 2 full cycles)
    void displayCyclic(int cycles = 2) const {
        if (!last) {
            std::cout << Color::YELLOW << " [Notice] Playlist is empty.\n" << Color::RESET;
            return;
        }
        std::cout << "\n" << Color::MAGENTA << Color::BOLD 
                  << "--- Cyclic Broadcast Simulation (" << cycles << " Full Loops) ---" << Color::RESET << "\n";
        
        CLLNode* curr = last->next;
        int loopCount = 0;
        int step = 1;

        while (loopCount < cycles) {
            std::cout << " Step " << std::setw(2) << step++ << ": Broadcasting [" 
                      << curr->data.announcementId << "] \"" << curr->data.title << "\"";
            if (curr == last) {
                loopCount++;
                std::cout << Color::YELLOW << " [Finished Loop " << loopCount << " -> Looping back to Start]" << Color::RESET;
            }
            std::cout << "\n";
            curr = curr->next;
        }
    }
};

// ============================================================================
// Module 4 Manager & Menu Handler
// ============================================================================
class CampusServicesManager {
private:
    SinglyLinkedList maintenanceList;
    DoublyLinkedList shuttleRoute;
    CircularLinkedList announcementPlaylist;

public:
    CampusServicesManager() {
        loadSampleData();
    }

    void loadSampleData() {
        // Sample Maintenance Requests
        maintenanceList.addRequest(MaintenanceRequest(101, "Prof. Sharma", "Room 302, IT Dept", "Projector bulb replacement", "Pending"));
        maintenanceList.addRequest(MaintenanceRequest(102, "Dr. Kulkarni", "Central Library", "AC unit temperature malfunction", "In Progress"));
        maintenanceList.addRequest(MaintenanceRequest(103, "Aditi Roy (Student)", "Girls Hostel B", "Wi-Fi access point reboot required", "Pending"));
        maintenanceList.addRequest(MaintenanceRequest(104, "Security Desk", "Main Gate", "Barrier arm sensor calibration", "Completed"));

        // Sample Shuttle Route Stations
        shuttleRoute.insertAtEnd(ShuttleStation(1, "Main Campus Gate"));
        shuttleRoute.insertAtEnd(ShuttleStation(2, "Administrative Building"));
        shuttleRoute.insertAtEnd(ShuttleStation(3, "Central Library & IT Dept"));
        shuttleRoute.insertAtEnd(ShuttleStation(4, "Auditorium & Sports Arena"));
        shuttleRoute.insertAtEnd(ShuttleStation(5, "Student Hostels & Mess"));

        // Sample Announcements
        announcementPlaylist.addAnnouncement(Announcement(1, "Annual Smart Campus Hackathon registrations close tonight!"));
        announcementPlaylist.addAnnouncement(Announcement(2, "Blood Donation Camp tomorrow at Central Auditorium (9 AM - 4 PM)"));
        announcementPlaylist.addAnnouncement(Announcement(3, "Mid-Semester exam timetable published on college portal"));
        announcementPlaylist.addAnnouncement(Announcement(4, "Guest lecture on Distributed Systems by Dr. Rao this Friday"));
    }

    void runMenu() {
        while (true) {
            std::cout << "\n" << Color::BLUE << Color::BOLD 
                      << "========================================================\n"
                      << "    MODULE 4: CAMPUS SERVICES USING LINKED LISTS        \n"
                      << "========================================================\n"
                      << Color::RESET;
            std::cout << " [PART A: Singly Linked List - Maintenance Requests]\n"
                      << "   1. Add Maintenance Request\n"
                      << "   2. Display All Maintenance Requests\n"
                      << "   3. Update Request Status\n"
                      << "   4. Delete Request by ID\n"
                      << "   5. Count Pending Requests\n"
                      << " [PART B: Doubly Linked List - Campus Shuttle Route]\n"
                      << "   6. Insert Shuttle Station (Beginning / End / Position)\n"
                      << "   7. Delete Station by ID\n"
                      << "   8. Display Route Forward & Reverse\n"
                      << " [PART C: Circular Linked List - Announcements]\n"
                      << "   9. Add Announcement\n"
                      << "  10. Delete Announcement\n"
                      << "  11. Display Announcements (Single Pass & Cyclic Broadcast)\n"
                      << " [PART D: Complexity Comparison]\n"
                      << "  12. Singly vs Doubly vs Circular Linked Lists Comparison\n"
                      << "   0. Return to Main Menu\n";

            int choice = InputHelper::getInteger(" Enter your choice (0-12): ");
            if (choice == 0) break;

            switch (choice) {
                case 1: {
                    int id = InputHelper::getInteger(" Enter Request ID: ");
                    std::string name = InputHelper::getString(" Enter Requester Name: ");
                    std::string loc = InputHelper::getString(" Enter Campus Location: ");
                    std::string desc = InputHelper::getString(" Enter Issue Description: ");
                    maintenanceList.addRequest(MaintenanceRequest(id, name, loc, desc, "Pending"));
                    break;
                }
                case 2:
                    maintenanceList.displayAll();
                    break;
                case 3: {
                    int id = InputHelper::getInteger(" Enter Request ID to update: ");
                    std::cout << " Choose Status: 1. Pending  2. In Progress  3. Completed\n";
                    int sChoice = InputHelper::getInteger(" Select status (1-3): ");
                    std::string statusStr = "Pending";
                    if (sChoice == 2) statusStr = "In Progress";
                    else if (sChoice == 3) statusStr = "Completed";
                    maintenanceList.updateStatus(id, statusStr);
                    break;
                }
                case 4: {
                    int id = InputHelper::getInteger(" Enter Request ID to delete: ");
                    maintenanceList.deleteRequest(id);
                    break;
                }
                case 5:
                    std::cout << Color::GREEN << " Total Pending Maintenance Requests: " 
                              << maintenanceList.countPending() << Color::RESET << "\n";
                    break;
                case 6: {
                    std::cout << "\n Station Insertion: 1. Beginning  2. End  3. Specific Position\n";
                    int posChoice = InputHelper::getInteger(" Select insertion mode (1-3): ");
                    int id = InputHelper::getInteger(" Enter Station ID: ");
                    std::string name = InputHelper::getString(" Enter Station Name: ");
                    if (posChoice == 1) {
                        shuttleRoute.insertAtBeginning(ShuttleStation(id, name));
                    } else if (posChoice == 2) {
                        shuttleRoute.insertAtEnd(ShuttleStation(id, name));
                    } else {
                        int pos = InputHelper::getInteger(" Enter 1-based position: ");
                        shuttleRoute.insertAtPosition(ShuttleStation(id, name), pos);
                    }
                    break;
                }
                case 7: {
                    int id = InputHelper::getInteger(" Enter Station ID to delete: ");
                    shuttleRoute.deleteStation(id);
                    break;
                }
                case 8:
                    shuttleRoute.displayForward();
                    shuttleRoute.displayReverse();
                    break;
                case 9: {
                    int id = InputHelper::getInteger(" Enter Announcement ID: ");
                    std::string title = InputHelper::getString(" Enter Announcement Title: ");
                    announcementPlaylist.addAnnouncement(Announcement(id, title));
                    break;
                }
                case 10: {
                    int id = InputHelper::getInteger(" Enter Announcement ID to delete: ");
                    announcementPlaylist.deleteAnnouncement(id);
                    break;
                }
                case 11:
                    announcementPlaylist.displayOnce();
                    announcementPlaylist.displayCyclic(2);
                    break;
                case 12: {
                    std::cout << "\n" << Color::YELLOW << Color::BOLD
                              << "========================================================================================\n"
                              << "       LINKED LIST VARIATIONS: ARCHITECTURAL & COMPLEXITY COMPARISON                    \n"
                              << "========================================================================================\n"
                              << Color::RESET
                              << " Feature               | Singly Linked List (SLL) | Doubly Linked List (DLL) | Circular Linked List (CLL)\n"
                              << "-----------------------+--------------------------+--------------------------+--------------------------\n"
                              << " Node Pointers         | 1 (next)                 | 2 (prev, next)           | 1 (next, loops to head)\n"
                              << " Memory Overhead       | Minimal (1 ptr/node)     | Highest (2 ptrs/node)    | Minimal (1 ptr/node)\n"
                              << " Traversal Direction   | Forward only             | Bidirectional (Fwd & Rev)| Cyclic (Continuous loop)\n"
                              << " Insert at Head        | O(1)                     | O(1)                     | O(1) (with tail pointer)\n"
                              << " Insert at Tail        | O(N) or O(1) with tail   | O(1) with tail           | O(1) (with tail pointer)\n"
                              << " Delete Given Node*    | O(N) (needs prev node)   | O(1) (via node->prev)    | O(N) (needs prev node)\n"
                              << " Reversal Complexity   | O(N)                     | O(1) (read via tail)     | O(N)\n"
                              << " Real-World Campus Use | Linear Task History      | Shuttle Two-way Route    | Continuous Digital Signage\n"
                              << "========================================================================================\n";
                    break;
                }
                default:
                    std::cout << Color::RED << " Invalid option. Please select 0 to 12.\n" << Color::RESET;
            }
            InputHelper::pause();
        }
    }
};

#endif // MODULE4_LINKED_LISTS_HPP
