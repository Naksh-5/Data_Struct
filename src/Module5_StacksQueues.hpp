#ifndef MODULE5_STACKS_QUEUES_HPP
#define MODULE5_STACKS_QUEUES_HPP

#include "Common.hpp"
#include <algorithm>
#include <cctype>

// ============================================================================
// PART A: CUSTOM STACK FOR STUDENT RECORD UNDO FUNCTIONALITY
// ============================================================================
enum ActionType { ACTION_INSERT, ACTION_DELETE };

struct UndoRecord {
    ActionType action;
    Student studentData;

    UndoRecord(ActionType act, const Student& s) : action(act), studentData(s) {}
};

// Node for dynamic linked-stack implementation
struct StackNode {
    UndoRecord record;
    StackNode* next;
    StackNode(const UndoRecord& rec) : record(rec), next(nullptr) {}
};

class UndoStack {
private:
    StackNode* topNode;
    int sizeCount;

public:
    UndoStack() : topNode(nullptr), sizeCount(0) {}
    ~UndoStack() { clear(); }

    void clear() {
        while (topNode) {
            StackNode* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
        sizeCount = 0;
    }

    bool isEmpty() const { return topNode == nullptr; }
    int size() const { return sizeCount; }

    // Push action onto undo stack
    void push(ActionType act, const Student& s) {
        StackNode* newNode = new StackNode(UndoRecord(act, s));
        newNode->next = topNode;
        topNode = newNode;
        sizeCount++;
    }

    // Pop and retrieve the most recent action
    bool pop(UndoRecord& outRecord) {
        if (isEmpty()) return false;
        StackNode* temp = topNode;
        outRecord = temp->record;
        topNode = topNode->next;
        delete temp;
        sizeCount--;
        return true;
    }

    // Perform Undo on the student dataset
    bool performUndo(std::vector<Student>& students) {
        if (isEmpty()) {
            std::cout << Color::YELLOW << " [Notice] Undo stack is empty! No actions to reverse.\n" << Color::RESET;
            return false;
        }

        UndoRecord lastAction(ACTION_INSERT, Student());
        pop(lastAction);

        if (lastAction.action == ACTION_INSERT) {
            // Reversing an INSERTION -> Delete the inserted student
            int idToRemove = lastAction.studentData.id;
            for (auto it = students.begin(); it != students.end(); ++it) {
                if (it->id == idToRemove) {
                    students.erase(it);
                    std::cout << Color::GREEN << " [UNDO SUCCESS] Reverted INSERTION of Student ID " 
                              << idToRemove << " (" << lastAction.studentData.name << "). Record removed.\n" << Color::RESET;
                    return true;
                }
            }
        } else if (lastAction.action == ACTION_DELETE) {
            // Reversing a DELETION -> Restore the saved student record
            students.push_back(lastAction.studentData);
            std::cout << Color::GREEN << " [UNDO SUCCESS] Reverted DELETION of Student ID " 
                      << lastAction.studentData.id << " (" << lastAction.studentData.name << "). Record restored!\n" << Color::RESET;
            return true;
        }
        return false;
    }
};

// ============================================================================
// PART B: INFIX TO POSTFIX AND PREFIX EXPRESSION CONVERTER (CUSTOM STACK)
// ============================================================================
template <typename T>
class CustomArrayStack {
private:
    T* data;
    int capacity;
    int topIdx;

public:
    CustomArrayStack(int cap = 200) : capacity(cap), topIdx(-1) {
        data = new T[capacity];
    }
    ~CustomArrayStack() {
        delete[] data;
    }

    bool isEmpty() const { return topIdx == -1; }
    bool isFull() const { return topIdx == capacity - 1; }

    bool push(const T& val) {
        if (isFull()) return false;
        data[++topIdx] = val;
        return true;
    }

    bool pop(T& val) {
        if (isEmpty()) return false;
        val = data[topIdx--];
        return true;
    }

    T top() const {
        if (isEmpty()) return T();
        return data[topIdx];
    }
};

class ExpressionConverter {
private:
    // Operator precedence: ^ has 3, * / has 2, + - has 1
    static int getPrecedence(char op) {
        if (op == '^') return 3;
        if (op == '*' || op == '/') return 2;
        if (op == '+' || op == '-') return 1;
        return 0;
    }

    // Associativity: '^' is Right-to-Left (true), others are Left-to-Right (false)
    static bool isRightAssociative(char op) {
        return (op == '^');
    }

    static bool isOperator(char ch) {
        return (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^');
    }

    static bool isOperand(char ch) {
        return std::isalnum(static_cast<unsigned char>(ch));
    }

public:
    // Validate matching parentheses and basic expression syntax
    static bool validateInfix(const std::string& expr, std::string& errorMsg) {
        CustomArrayStack<char> parenStack(100);
        int operandCount = 0;
        int operatorCount = 0;

        for (size_t i = 0; i < expr.length(); ++i) {
            char ch = expr[i];
            if (std::isspace(static_cast<unsigned char>(ch))) continue;

            if (ch == '(') {
                parenStack.push(ch);
            } else if (ch == ')') {
                char dummy;
                if (!parenStack.pop(dummy)) {
                    errorMsg = "Mismatched closing parenthesis ')' at position " + std::to_string(i + 1);
                    return false;
                }
            } else if (isOperator(ch)) {
                operatorCount++;
            } else if (isOperand(ch)) {
                operandCount++;
            } else {
                errorMsg = "Invalid character '" + std::string(1, ch) + "' in expression";
                return false;
            }
        }

        if (!parenStack.isEmpty()) {
            errorMsg = "Mismatched opening parenthesis '(' without closing pair";
            return false;
        }

        if (operandCount == 0) {
            errorMsg = "Expression contains no operands";
            return false;
        }

        return true;
    }

    // Convert Infix -> Postfix
    static std::string infixToPostfix(const std::string& infix) {
        CustomArrayStack<char> opStack(200);
        std::string postfix = "";

        for (char ch : infix) {
            if (std::isspace(static_cast<unsigned char>(ch))) continue;

            if (isOperand(ch)) {
                postfix += ch;
            } else if (ch == '(') {
                opStack.push(ch);
            } else if (ch == ')') {
                char popped;
                while (!opStack.isEmpty() && opStack.top() != '(') {
                    opStack.pop(popped);
                    postfix += popped;
                }
                opStack.pop(popped); // Remove '('
            } else if (isOperator(ch)) {
                while (!opStack.isEmpty() && opStack.top() != '(') {
                    char topOp = opStack.top();
                    int precTop = getPrecedence(topOp);
                    int precCurr = getPrecedence(ch);

                    if (precTop > precCurr || (precTop == precCurr && !isRightAssociative(ch))) {
                        char popped;
                        opStack.pop(popped);
                        postfix += popped;
                    } else {
                        break;
                    }
                }
                opStack.push(ch);
            }
        }

        while (!opStack.isEmpty()) {
            char popped;
            opStack.pop(popped);
            postfix += popped;
        }
        return postfix;
    }

    // Convert Infix -> Prefix
    // Algorithm:
    // 1. Reverse infix string (swapping '(' and ')')
    // 2. Obtain postfix expression with modified operator precedence check
    // 3. Reverse the resulting postfix expression -> Prefix!
    static std::string infixToPrefix(const std::string& infix) {
        std::string revInfix = "";
        for (int i = static_cast<int>(infix.length()) - 1; i >= 0; --i) {
            char ch = infix[i];
            if (ch == '(') revInfix += ')';
            else if (ch == ')') revInfix += '(';
            else revInfix += ch;
        }

        CustomArrayStack<char> opStack(200);
        std::string revPostfix = "";

        for (char ch : revInfix) {
            if (std::isspace(static_cast<unsigned char>(ch))) continue;

            if (isOperand(ch)) {
                revPostfix += ch;
            } else if (ch == '(') {
                opStack.push(ch);
            } else if (ch == ')') {
                char popped;
                while (!opStack.isEmpty() && opStack.top() != '(') {
                    opStack.pop(popped);
                    revPostfix += popped;
                }
                opStack.pop(popped); // Discard '('
            } else if (isOperator(ch)) {
                while (!opStack.isEmpty() && opStack.top() != '(') {
                    char topOp = opStack.top();
                    int precTop = getPrecedence(topOp);
                    int precCurr = getPrecedence(ch);

                    // For reversed expression: if operator is right-associative (^), pop on >=;
                    // if left-associative (+,-,*,/), pop only on strictly >
                    if (precTop > precCurr || (precTop == precCurr && isRightAssociative(ch))) {
                        char popped;
                        opStack.pop(popped);
                        revPostfix += popped;
                    } else {
                        break;
                    }
                }
                opStack.push(ch);
            }
        }

        while (!opStack.isEmpty()) {
            char popped;
            opStack.pop(popped);
            revPostfix += popped;
        }

        // Reverse the modified postfix string to obtain the true Prefix expression
        std::string prefix = revPostfix;
        std::reverse(prefix.begin(), prefix.end());
        return prefix;
    }
};

// ============================================================================
// PART C: ARRAY-BASED LINEAR QUEUE (Student Service Requests)
// ============================================================================
struct ServiceTicket {
    int ticketId;
    std::string studentName;
    std::string serviceType; // e.g., "Transcript Issue", "Bonafide Certificate"

    ServiceTicket(int id = 0, const std::string& name = "", const std::string& type = "")
        : ticketId(id), studentName(name), serviceType(type) {}
};

class LinearQueue {
private:
    ServiceTicket* arr;
    int capacity;
    int frontIdx;
    int rearIdx;

public:
    LinearQueue(int cap = 6) : capacity(cap), frontIdx(-1), rearIdx(-1) {
        arr = new ServiceTicket[capacity];
    }
    ~LinearQueue() { delete[] arr; }

    bool isEmpty() const {
        return (frontIdx == -1 || frontIdx > rearIdx);
    }

    bool isFull() const {
        return (rearIdx == capacity - 1);
    }

    // Enqueue at rear
    bool enqueue(const ServiceTicket& ticket) {
        if (isFull()) {
            std::cout << Color::RED << " [QUEUE OVERFLOW] Linear queue is full at index " 
                      << rearIdx << "! Cannot enqueue Ticket #" << ticket.ticketId 
                      << "\n (Notice: Linear queue cannot reuse freed slots at front without shifting)\n" << Color::RESET;
            return false;
        }
        if (frontIdx == -1) frontIdx = 0;
        arr[++rearIdx] = ticket;
        std::cout << Color::GREEN << " [Enqueued] Ticket #" << ticket.ticketId 
                  << " added at rear position " << rearIdx << ".\n" << Color::RESET;
        return true;
    }

    // Dequeue from front
    bool dequeue(ServiceTicket& servedTicket) {
        if (isEmpty()) {
            std::cout << Color::RED << " [QUEUE UNDERFLOW] Linear queue is empty! No service requests to process.\n" << Color::RESET;
            return false;
        }
        servedTicket = arr[frontIdx++];
        std::cout << Color::GREEN << " [Processed] Ticket #" << servedTicket.ticketId 
                  << " (" << servedTicket.studentName << " - " << servedTicket.serviceType 
                  << ") processed from front position " << (frontIdx - 1) << ".\n" << Color::RESET;
        return true;
    }

    // Display all pending requests
    void display() const {
        if (isEmpty()) {
            std::cout << Color::YELLOW << " [Notice] Linear queue has no pending tickets.\n" << Color::RESET;
            return;
        }
        std::cout << "\n" << Color::CYAN << Color::BOLD 
                  << "--- Pending Service Requests in Linear Queue (Front: " << frontIdx << ", Rear: " << rearIdx << ") ---" 
                  << Color::RESET << "\n";
        for (int i = frontIdx; i <= rearIdx; ++i) {
            std::cout << " Slot [" << i << "] -> Ticket #" << arr[i].ticketId 
                      << " | " << arr[i].studentName 
                      << " | Service: " << arr[i].serviceType << "\n";
        }
    }

    // Search by ID
    bool searchById(int id) const {
        if (isEmpty()) return false;
        for (int i = frontIdx; i <= rearIdx; ++i) {
            if (arr[i].ticketId == id) {
                std::cout << Color::GREEN << " [Found] Ticket #" << id 
                          << " found at slot " << i << " (" << arr[i].studentName << ").\n" << Color::RESET;
                return true;
            }
        }
        std::cout << Color::RED << " [Not Found] Ticket #" << id << " not in pending queue.\n" << Color::RESET;
        return false;
    }
};

// ============================================================================
// PART D: ARRAY-BASED CIRCULAR QUEUE (Recurring Campus Service Scheduling)
// ============================================================================
class CircularQueue {
private:
    ServiceTicket* arr;
    int capacity;
    int front;
    int rear;

public:
    CircularQueue(int cap = 6) : capacity(cap), front(-1), rear(-1) {
        arr = new ServiceTicket[capacity];
    }
    ~CircularQueue() { delete[] arr; }

    bool isEmpty() const {
        return (front == -1);
    }

    bool isFull() const {
        return ((rear + 1) % capacity == front);
    }

    // Enqueue in Circular Queue
    bool enqueue(const ServiceTicket& item) {
        if (isFull()) {
            std::cout << Color::RED << " [CIRCULAR QUEUE FULL] Capacity (" << capacity << ") reached! Cannot add Ticket #" 
                      << item.ticketId << ".\n" << Color::RESET;
            return false;
        }

        if (isEmpty()) {
            front = rear = 0;
        } else {
            rear = (rear + 1) % capacity;
        }

        arr[rear] = item;
        std::cout << Color::GREEN << " [Enqueued] Ticket #" << item.ticketId 
                  << " entered circular queue at physical index " << rear << ".\n" << Color::RESET;
        return true;
    }

    // Dequeue in Circular Queue
    bool dequeue(ServiceTicket& item) {
        if (isEmpty()) {
            std::cout << Color::RED << " [CIRCULAR QUEUE EMPTY] No requests available.\n" << Color::RESET;
            return false;
        }

        item = arr[front];
        std::cout << Color::GREEN << " [Dispatched] Ticket #" << item.ticketId 
                  << " processed from physical index " << front << ".\n" << Color::RESET;

        if (front == rear) {
            // Queue had only one element, now reset to empty
            front = rear = -1;
        } else {
            front = (front + 1) % capacity;
        }
        return true;
    }

    // Display pending requests in logical circular sequence
    void display() const {
        if (isEmpty()) {
            std::cout << Color::YELLOW << " [Notice] Circular queue is empty.\n" << Color::RESET;
            return;
        }

        std::cout << "\n" << Color::CYAN << Color::BOLD 
                  << "--- Recurring Service Circular Queue (Front: " << front << ", Rear: " << rear << ") ---" 
                  << Color::RESET << "\n";
        
        int i = front;
        int count = 1;
        while (true) {
            std::cout << " " << count++ << ". [Array Pos " << i << "] Ticket #" << arr[i].ticketId 
                      << " | Student: " << arr[i].studentName 
                      << " | Type: " << arr[i].serviceType << "\n";
            if (i == rear) break;
            i = (i + 1) % capacity;
        }
    }
};

// ============================================================================
// Module 5 Manager & Menu Handler
// ============================================================================
class StackQueueManager {
private:
    UndoStack undoHistory;
    LinearQueue serviceQueue;
    CircularQueue circularSchedule;

public:
    StackQueueManager() : serviceQueue(5), circularSchedule(5) {
        // Pre-fill sample tickets
        serviceQueue.enqueue(ServiceTicket(201, "Neha Patil", "Hostel Leave Approval"));
        serviceQueue.enqueue(ServiceTicket(202, "Amit Joshi", "Identity Card Duplicate"));
        serviceQueue.enqueue(ServiceTicket(203, "Kavya Nair", "Fee Clearance Receipt"));

        circularSchedule.enqueue(ServiceTicket(301, "Campus Security", "Night Patrol Sweep A"));
        circularSchedule.enqueue(ServiceTicket(302, "Facility Team", "Library Sanitization"));
        circularSchedule.enqueue(ServiceTicket(303, "Sports Desk", "Gymnasium Equipment Inspection"));
    }

    UndoStack& getUndoStack() { return undoHistory; }

    void runMenu(std::vector<Student>& sharedStudents) {
        while (true) {
            std::cout << "\n" << Color::BLUE << Color::BOLD 
                      << "========================================================\n"
                      << "   MODULE 5: STACKS, QUEUES & EXPRESSION CONVERSION     \n"
                      << "========================================================\n"
                      << Color::RESET;
            std::cout << " [PART A: Stack - Student Record Undo]\n"
                      << "   1. Undo Most Recent Student Modification (" << undoHistory.size() << " action(s) in stack)\n"
                      << "   2. Display Current Student Records\n"
                      << " [PART B: Stack - Infix Expression Conversion]\n"
                      << "   3. Convert Infix to Postfix and Prefix\n"
                      << "   4. Test Standard Example (A+B*C)\n"
                      << " [PART C: Linear Queue - Student Service Desk]\n"
                      << "   5. Enqueue Student Service Ticket (Linear)\n"
                      << "   6. Process Service Ticket (Linear Dequeue)\n"
                      << "   7. Display Linear Queue Tickets\n"
                      << "   8. Search Ticket by ID in Linear Queue\n"
                      << " [PART D: Circular Queue - Recurring Campus Services]\n"
                      << "   9. Enqueue Recurring Task (Circular)\n"
                      << "  10. Process Next Task (Circular Dequeue)\n"
                      << "  11. Display Circular Queue in Logical Sequence\n"
                      << " [PART E: Complexity Comparison]\n"
                      << "  12. LIFO vs FIFO & Linear vs Circular Queue Analysis\n"
                      << "   0. Return to Main Menu\n";

            int choice = InputHelper::getInteger(" Enter your choice (0-12): ");
            if (choice == 0) break;

            switch (choice) {
                case 1: {
                    undoHistory.performUndo(sharedStudents);
                    break;
                }
                case 2: {
                    std::cout << "\n" << Color::CYAN << Color::BOLD 
                              << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                              << "| ID     | Name               | Div  | Contact      | Average | Marks (M, P, C, CS) |\n"
                              << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                              << Color::RESET;
                    for (const auto& s : sharedStudents) s.printRow();
                    std::cout << Color::CYAN << "+--------+--------------------+------+--------------+---------+---------------------+\n" << Color::RESET;
                    break;
                }
                case 3: {
                    std::string infix = InputHelper::getString(" Enter valid Infix expression (e.g. (A+B)*(C-D)^E): ");
                    std::string errorMsg;
                    if (!ExpressionConverter::validateInfix(infix, errorMsg)) {
                        std::cout << Color::RED << " [Syntax Error] " << errorMsg << "\n" << Color::RESET;
                    } else {
                        std::string postfix = ExpressionConverter::infixToPostfix(infix);
                        std::string prefix = ExpressionConverter::infixToPrefix(infix);
                        std::cout << "\n" << Color::GREEN << Color::BOLD << " Expression Results:" << Color::RESET << "\n"
                                  << "   Infix:   " << Color::CYAN << infix << Color::RESET << "\n"
                                  << "   Postfix: " << Color::YELLOW << postfix << Color::RESET << "\n"
                                  << "   Prefix:  " << Color::MAGENTA << prefix << Color::RESET << "\n";
                    }
                    break;
                }
                case 4: {
                    std::string testExpr = "A+B*C";
                    std::string postfix = ExpressionConverter::infixToPostfix(testExpr);
                    std::string prefix = ExpressionConverter::infixToPrefix(testExpr);
                    std::cout << "\n" << Color::GREEN << Color::BOLD << " Verified Specification Benchmark:" << Color::RESET << "\n"
                              << "   Infix:   " << Color::CYAN << testExpr << Color::RESET << "\n"
                              << "   Postfix: " << Color::YELLOW << postfix << Color::RESET << " (Expected: ABC*+)\n"
                              << "   Prefix:  " << Color::MAGENTA << prefix << Color::RESET << " (Expected: +A*BC)\n";
                    break;
                }
                case 5: {
                    int id = InputHelper::getInteger(" Enter Ticket ID: ");
                    std::string name = InputHelper::getString(" Enter Student Name: ");
                    std::string type = InputHelper::getString(" Enter Service Requested: ");
                    serviceQueue.enqueue(ServiceTicket(id, name, type));
                    break;
                }
                case 6: {
                    ServiceTicket served;
                    serviceQueue.dequeue(served);
                    break;
                }
                case 7:
                    serviceQueue.display();
                    break;
                case 8: {
                    int id = InputHelper::getInteger(" Enter Ticket ID to search: ");
                    serviceQueue.searchById(id);
                    break;
                }
                case 9: {
                    int id = InputHelper::getInteger(" Enter Task ID: ");
                    std::string dept = InputHelper::getString(" Enter Facility/Team: ");
                    std::string desc = InputHelper::getString(" Enter Recurring Task Description: ");
                    circularSchedule.enqueue(ServiceTicket(id, dept, desc));
                    break;
                }
                case 10: {
                    ServiceTicket task;
                    circularSchedule.dequeue(task);
                    break;
                }
                case 11:
                    circularSchedule.display();
                    break;
                case 12: {
                    std::cout << "\n" << Color::YELLOW << Color::BOLD
                              << "========================================================================================\n"
                              << "         THEORETICAL ANALYSIS: LIFO vs FIFO & QUEUE ARCHITECTURES                       \n"
                              << "========================================================================================\n"
                              << Color::RESET
                              << " 1. LIFO (Last-In, First-Out) - Stacks:\n"
                              << "    - Operations: Push O(1), Pop O(1), Top O(1).\n"
                              << "    - Applications: Function call stacks, Expression parsing, Reverse/Undo chains.\n\n"
                              << " 2. FIFO (First-In, First-Out) - Queues:\n"
                              << "    - Operations: Enqueue O(1), Dequeue O(1).\n"
                              << "    - Applications: CPU process scheduling, Printer spoolers, Student ticket processing.\n\n"
                              << " 3. LINEAR QUEUE vs CIRCULAR QUEUE COMPARISON:\n"
                              << "    - Linear Queue Limitation: 'False Overflow'. When rear reaches max index, no more\n"
                              << "      items can be added even if front has moved ahead, leaving empty slots wasted.\n"
                              << "    - Circular Queue Solution: Uses modulo arithmetic ((rear + 1) % MAX) to wrap around\n"
                              << "      and reuse vacated memory cells at the beginning of the buffer.\n"
                              << "    - Full Condition: (rear + 1) % Capacity == front\n"
                              << "    - Empty Condition: front == -1\n"
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

#endif // MODULE5_STACKS_QUEUES_HPP
