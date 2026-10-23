#ifndef MODULE6_TREES_HPP
#define MODULE6_TREES_HPP

#include "Common.hpp"
#include <queue>
#include <algorithm>

// ============================================================================
// PART A: BINARY SEARCH TREE (BST)
// ============================================================================
struct BSTNode {
    int studentId;
    Student data;
    BSTNode* left;
    BSTNode* right;

    BSTNode(const Student& s) : studentId(s.id), data(s), left(nullptr), right(nullptr) {}
};

class BinarySearchTree {
private:
    BSTNode* root;

    // Helper: Recursive Insertion
    BSTNode* insertHelper(BSTNode* node, const Student& s, bool& inserted) {
        if (!node) {
            inserted = true;
            return new BSTNode(s);
        }
        if (s.id == node->studentId) {
            inserted = false; // Duplicate rejected
            return node;
        } else if (s.id < node->studentId) {
            node->left = insertHelper(node->left, s, inserted);
        } else {
            node->right = insertHelper(node->right, s, inserted);
        }
        return node;
    }

    // Helper: Find minimum node (Inorder Successor)
    BSTNode* findMin(BSTNode* node) const {
        while (node && node->left) node = node->left;
        return node;
    }

    // Helper: Recursive Deletion (Handles Leaf, 1-Child, 2-Children)
    BSTNode* deleteHelper(BSTNode* node, int id, bool& deleted) {
        if (!node) return nullptr;

        if (id < node->studentId) {
            node->left = deleteHelper(node->left, id, deleted);
        } else if (id > node->studentId) {
            node->right = deleteHelper(node->right, id, deleted);
        } else {
            // Node found!
            deleted = true;
            // Case 1 & 2: Leaf or Single Child
            if (!node->left) {
                BSTNode* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                BSTNode* temp = node->left;
                delete node;
                return temp;
            }
            // Case 3: Two Children -> Replace with Inorder Successor
            BSTNode* successor = findMin(node->right);
            node->studentId = successor->studentId;
            node->data = successor->data;
            node->right = deleteHelper(node->right, successor->studentId, deleted);
        }
        return node;
    }

    // Helper: Recursive Search
    BSTNode* searchHelper(BSTNode* node, int id, int& comparisons) const {
        if (!node) return nullptr;
        comparisons++;
        if (node->studentId == id) return node;
        if (id < node->studentId) return searchHelper(node->left, id, comparisons);
        return searchHelper(node->right, id, comparisons);
    }

    // Traversals
    void inorderHelper(BSTNode* node) const {
        if (!node) return;
        inorderHelper(node->left);
        std::cout << "[" << node->studentId << ": " << node->data.name << "] ";
        inorderHelper(node->right);
    }

    void preorderHelper(BSTNode* node) const {
        if (!node) return;
        std::cout << "[" << node->studentId << ": " << node->data.name << "] ";
        preorderHelper(node->left);
        preorderHelper(node->right);
    }

    void postorderHelper(BSTNode* node) const {
        if (!node) return;
        postorderHelper(node->left);
        postorderHelper(node->right);
        std::cout << "[" << node->studentId << ": " << node->data.name << "] ";
    }

    // Tree Height
    int heightHelper(BSTNode* node) const {
        if (!node) return 0;
        return 1 + std::max(heightHelper(node->left), heightHelper(node->right));
    }

    // Deep Copy helper
    BSTNode* copyHelper(BSTNode* node) const {
        if (!node) return nullptr;
        BSTNode* newNode = new BSTNode(node->data);
        newNode->left = copyHelper(node->left);
        newNode->right = copyHelper(node->right);
        return newNode;
    }

    // Mirror Image helper (swaps left and right recursively)
    BSTNode* mirrorHelper(BSTNode* node) const {
        if (!node) return nullptr;
        BSTNode* mirrored = new BSTNode(node->data);
        mirrored->left = mirrorHelper(node->right);
        mirrored->right = mirrorHelper(node->left);
        return mirrored;
    }

    // Parents with Children
    void printParentsWithChildren(BSTNode* node) const {
        if (!node) return;
        if (node->left || node->right) {
            std::cout << " Parent [" << node->studentId << ": " << node->data.name << "] -> ";
            if (node->left) std::cout << "Left: [" << node->left->studentId << "] ";
            else std::cout << "Left: [None] ";
            if (node->right) std::cout << "Right: [" << node->right->studentId << "]";
            else std::cout << "Right: [None]";
            std::cout << "\n";
        }
        printParentsWithChildren(node->left);
        printParentsWithChildren(node->right);
    }

    // Leaf nodes
    void printLeaves(BSTNode* node, int& count) const {
        if (!node) return;
        if (!node->left && !node->right) {
            std::cout << " Leaf Node: [" << node->studentId << ": " << node->data.name << "]\n";
            count++;
            return;
        }
        printLeaves(node->left, count);
        printLeaves(node->right, count);
    }

    // Memory clean up
    void destroy(BSTNode* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    BinarySearchTree() : root(nullptr) {}
    ~BinarySearchTree() { destroy(root); }

    bool insert(const Student& s) {
        bool inserted = false;
        root = insertHelper(root, s, inserted);
        return inserted;
    }

    bool remove(int id) {
        bool deleted = false;
        root = deleteHelper(root, id, deleted);
        return deleted;
    }

    BSTNode* search(int id, int& comparisons) const {
        comparisons = 0;
        return searchHelper(root, id, comparisons);
    }

    void inorder() const {
        std::cout << " Inorder (Sorted L-Root-R):   ";
        inorderHelper(root);
        std::cout << "\n";
    }

    void preorder() const {
        std::cout << " Preorder (Root-L-R):          ";
        preorderHelper(root);
        std::cout << "\n";
    }

    void postorder() const {
        std::cout << " Postorder (L-R-Root):         ";
        postorderHelper(root);
        std::cout << "\n";
    }

    int getHeight() const {
        return heightHelper(root);
    }

    void displayParentsAndChildren() const {
        if (!root) {
            std::cout << Color::YELLOW << " [Notice] BST is empty.\n" << Color::RESET;
            return;
        }
        std::cout << Color::CYAN << "--- BST Parent Nodes & Their Respective Children ---" << Color::RESET << "\n";
        printParentsWithChildren(root);
    }

    void displayLeaves() const {
        if (!root) {
            std::cout << Color::YELLOW << " [Notice] BST is empty.\n" << Color::RESET;
            return;
        }
        std::cout << Color::CYAN << "--- BST Leaf Nodes (External Nodes) ---" << Color::RESET << "\n";
        int count = 0;
        printLeaves(root, count);
        std::cout << " Total Leaf Nodes: " << count << "\n";
    }

    // Level-Order Traversal using Queue
    void levelOrder() const {
        if (!root) {
            std::cout << Color::YELLOW << " [Notice] BST is empty.\n" << Color::RESET;
            return;
        }
        std::cout << Color::CYAN << "--- BST Level-Order (Breadth-First) Traversal ---" << Color::RESET << "\n";
        std::queue<BSTNode*> q;
        q.push(root);
        int level = 0;

        while (!q.empty()) {
            int levelSize = static_cast<int>(q.size());
            std::cout << " Level " << level++ << ": ";
            for (int i = 0; i < levelSize; ++i) {
                BSTNode* curr = q.front();
                q.pop();
                std::cout << "[" << curr->studentId << "] ";
                if (curr->left) q.push(curr->left);
                if (curr->right) q.push(curr->right);
            }
            std::cout << "\n";
        }
    }

    // Mirror image creation and traversal
    void displayMirror() const {
        if (!root) {
            std::cout << Color::YELLOW << " [Notice] BST is empty.\n" << Color::RESET;
            return;
        }
        BSTNode* mirroredRoot = mirrorHelper(root);
        std::cout << Color::MAGENTA << " Mirror Image Inorder Traversal (R-Root-L descending): " << Color::RESET;
        inorderHelper(mirroredRoot);
        std::cout << "\n";
        destroy(mirroredRoot); // Clean temporary mirror copy
    }

    // Deep independent copy verification
    void verifyDeepCopy() const {
        if (!root) {
            std::cout << Color::YELLOW << " [Notice] BST is empty.\n" << Color::RESET;
            return;
        }
        BSTNode* copyRoot = copyHelper(root);
        std::cout << Color::GREEN << " Independent copy created successfully in separate heap memory!\n"
                  << " Original Root Address: " << root << " | Copy Root Address: " << copyRoot << "\n"
                  << " Copy Inorder Traversal: ";
        inorderHelper(copyRoot);
        std::cout << "\n" << Color::RESET;
        destroy(copyRoot);
    }
};

// ============================================================================
// PART B: AVL TREE (SELF-BALANCING BINARY SEARCH TREE)
// ============================================================================
struct AVLNode {
    int studentId;
    Student data;
    int height;
    AVLNode* left;
    AVLNode* right;

    AVLNode(const Student& s) : studentId(s.id), data(s), height(1), left(nullptr), right(nullptr) {}
};

class AVLTree {
private:
    AVLNode* root;

    int getHeight(AVLNode* node) const {
        return node ? node->height : 0;
    }

    int getBalanceFactor(AVLNode* node) const {
        return node ? (getHeight(node->left) - getHeight(node->right)) : 0;
    }

    // LL Case: Right Rotation
    AVLNode* rightRotate(AVLNode* y) {
        AVLNode* x = y->left;
        AVLNode* T2 = x->right;

        // Perform rotation
        x->right = y;
        y->left = T2;

        // Update heights
        y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));
        x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));

        return x; // New root
    }

    // RR Case: Left Rotation
    AVLNode* leftRotate(AVLNode* x) {
        AVLNode* y = x->right;
        AVLNode* T2 = y->left;

        // Perform rotation
        y->left = x;
        x->right = T2;

        // Update heights
        x->height = 1 + std::max(getHeight(x->left), getHeight(x->right));
        y->height = 1 + std::max(getHeight(y->left), getHeight(y->right));

        return y; // New root
    }

    // Recursive AVL Insertion with Automatic Rotations
    AVLNode* insertHelper(AVLNode* node, const Student& s, bool& inserted) {
        if (!node) {
            inserted = true;
            return new AVLNode(s);
        }

        if (s.id < node->studentId) {
            node->left = insertHelper(node->left, s, inserted);
        } else if (s.id > node->studentId) {
            node->right = insertHelper(node->right, s, inserted);
        } else {
            inserted = false; // Duplicates rejected
            return node;
        }

        // Update node height
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

        // Compute Balance Factor to check if node became unbalanced
        int balance = getBalanceFactor(node);

        // 4 Balancing Rotation Cases:
        // Case 1: Left-Left (LL) -> Right Rotation
        if (balance > 1 && s.id < node->left->studentId) {
            return rightRotate(node);
        }
        // Case 2: Right-Right (RR) -> Left Rotation
        if (balance < -1 && s.id > node->right->studentId) {
            return leftRotate(node);
        }
        // Case 3: Left-Right (LR) -> Left Rotate left child, then Right Rotate node
        if (balance > 1 && s.id > node->left->studentId) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        // Case 4: Right-Left (RL) -> Right Rotate right child, then Left Rotate node
        if (balance < -1 && s.id < node->right->studentId) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    AVLNode* findMin(AVLNode* node) const {
        while (node && node->left) node = node->left;
        return node;
    }

    // Recursive AVL Deletion with Automatic Rebalancing
    AVLNode* deleteHelper(AVLNode* node, int id, bool& deleted) {
        if (!node) return nullptr;

        if (id < node->studentId) {
            node->left = deleteHelper(node->left, id, deleted);
        } else if (id > node->studentId) {
            node->right = deleteHelper(node->right, id, deleted);
        } else {
            deleted = true;
            if (!node->left || !node->right) {
                AVLNode* temp = node->left ? node->left : node->right;
                if (!temp) {
                    temp = node;
                    node = nullptr;
                } else {
                    *node = *temp;
                }
                delete temp;
            } else {
                AVLNode* temp = findMin(node->right);
                node->studentId = temp->studentId;
                node->data = temp->data;
                node->right = deleteHelper(node->right, temp->studentId, deleted);
            }
        }

        if (!node) return nullptr;

        // Update height
        node->height = 1 + std::max(getHeight(node->left), getHeight(node->right));

        // Rebalance
        int balance = getBalanceFactor(node);

        if (balance > 1 && getBalanceFactor(node->left) >= 0) {
            return rightRotate(node);
        }
        if (balance > 1 && getBalanceFactor(node->left) < 0) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (balance < -1 && getBalanceFactor(node->right) <= 0) {
            return leftRotate(node);
        }
        if (balance < -1 && getBalanceFactor(node->right) > 0) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    void inorderDetailed(AVLNode* node) const {
        if (!node) return;
        inorderDetailed(node->left);
        std::cout << " [ID: " << std::setw(4) << node->studentId 
                  << " | " << std::setw(15) << std::left << node->data.name.substr(0, 15) << std::right
                  << " | H: " << node->height 
                  << " | BF: " << std::setw(2) << getBalanceFactor(node) << "]\n";
        inorderDetailed(node->right);
    }

    void destroy(AVLNode* node) {
        if (!node) return;
        destroy(node->left);
        destroy(node->right);
        delete node;
    }

public:
    AVLTree() : root(nullptr) {}
    ~AVLTree() { destroy(root); }

    bool insert(const Student& s) {
        bool inserted = false;
        root = insertHelper(root, s, inserted);
        return inserted;
    }

    bool remove(int id) {
        bool deleted = false;
        root = deleteHelper(root, id, deleted);
        return deleted;
    }

    void displayInorderWithBalance() const {
        if (!root) {
            std::cout << Color::YELLOW << " [Notice] AVL Tree is empty.\n" << Color::RESET;
            return;
        }
        std::cout << "\n" << Color::CYAN << Color::BOLD 
                  << "--- AVL Tree Inorder Traversal (Height & Balance Factors) ---" << Color::RESET << "\n";
        inorderDetailed(root);
        std::cout << " Overall AVL Tree Height: " << getHeight(root) << "\n";
    }

    AVLNode* search(int id, int& comparisons) const {
        comparisons = 0;
        AVLNode* curr = root;
        while (curr) {
            comparisons++;
            if (curr->studentId == id) return curr;
            if (id < curr->studentId) curr = curr->left;
            else curr = curr->right;
        }
        return nullptr;
    }
};

// ============================================================================
// PART C: B+ TREE (ORDER 3 HIERARCHICAL INDEXING FOR RANGE QUERIES)
// In a B+ Tree:
// - Internal nodes hold routing keys and child pointers.
// - Leaf nodes hold actual data records and a linked 'next' pointer.
// ============================================================================
const int BPLUS_ORDER = 3; // Max keys in node = 2, max children = 3

struct BPlusNode {
    bool isLeaf;
    int numKeys;
    int keys[BPLUS_ORDER];               // Space for overflow during split
    Student records[BPLUS_ORDER];        // Used in leaf nodes
    BPlusNode* children[BPLUS_ORDER + 1];// Used in internal nodes
    BPlusNode* next;                     // Linked leaf pointer for range queries

    BPlusNode(bool leaf = true) : isLeaf(leaf), numKeys(0), next(nullptr) {
        for (int i = 0; i <= BPLUS_ORDER; ++i) children[i] = nullptr;
    }
};

class BPlusTree {
private:
    BPlusNode* root;

    // Search leaf containing the key
    BPlusNode* findLeaf(BPlusNode* curr, int id) const {
        if (!curr) return nullptr;
        while (!curr->isLeaf) {
            int i = 0;
            while (i < curr->numKeys && id >= curr->keys[i]) {
                i++;
            }
            curr = curr->children[i];
        }
        return curr;
    }

    // Split child of an internal node
    void splitChild(BPlusNode* parent, int idx, BPlusNode* fullChild) {
        BPlusNode* rightSibling = new BPlusNode(fullChild->isLeaf);
        int mid = fullChild->numKeys / 2;

        if (fullChild->isLeaf) {
            // Leaf Split: keep mid element in right sibling, copy up to parent
            rightSibling->numKeys = fullChild->numKeys - mid;
            for (int j = 0; j < rightSibling->numKeys; ++j) {
                rightSibling->keys[j] = fullChild->keys[mid + j];
                rightSibling->records[j] = fullChild->records[mid + j];
            }
            fullChild->numKeys = mid;

            // Link leaves
            rightSibling->next = fullChild->next;
            fullChild->next = rightSibling;

            // Insert promoted key into parent
            for (int j = parent->numKeys; j > idx; --j) {
                parent->keys[j] = parent->keys[j - 1];
                parent->children[j + 1] = parent->children[j];
            }
            parent->keys[idx] = rightSibling->keys[0];
            parent->children[idx + 1] = rightSibling;
            parent->numKeys++;
        } else {
            // Internal Node Split: push mid element up to parent
            int promotedKey = fullChild->keys[mid];
            rightSibling->numKeys = fullChild->numKeys - mid - 1;

            for (int j = 0; j < rightSibling->numKeys; ++j) {
                rightSibling->keys[j] = fullChild->keys[mid + 1 + j];
                rightSibling->children[j] = fullChild->children[mid + 1 + j];
            }
            rightSibling->children[rightSibling->numKeys] = fullChild->children[fullChild->numKeys];
            fullChild->numKeys = mid;

            for (int j = parent->numKeys; j > idx; --j) {
                parent->keys[j] = parent->keys[j - 1];
                parent->children[j + 1] = parent->children[j];
            }
            parent->keys[idx] = promotedKey;
            parent->children[idx + 1] = rightSibling;
            parent->numKeys++;
        }
    }

    void insertNonFull(BPlusNode* node, const Student& s) {
        int i = node->numKeys - 1;

        if (node->isLeaf) {
            // Insert into sorted leaf
            while (i >= 0 && s.id < node->keys[i]) {
                node->keys[i + 1] = node->keys[i];
                node->records[i + 1] = node->records[i];
                i--;
            }
            node->keys[i + 1] = s.id;
            node->records[i + 1] = s;
            node->numKeys++;
        } else {
            // Find child to traverse
            while (i >= 0 && s.id < node->keys[i]) {
                i--;
            }
            i++;

            if (node->children[i]->numKeys == (BPLUS_ORDER - 1)) {
                splitChild(node, i, node->children[i]);
                if (s.id >= node->keys[i]) {
                    i++;
                }
            }
            insertNonFull(node->children[i], s);
        }
    }

    void displayLevelHelper(BPlusNode* node) const {
        if (!node) return;
        std::queue<BPlusNode*> q;
        q.push(node);
        int level = 0;

        while (!q.empty()) {
            int sz = static_cast<int>(q.size());
            std::cout << " Level " << level++ << ": ";
            for (int k = 0; k < sz; ++k) {
                BPlusNode* curr = q.front();
                q.pop();
                std::cout << "[";
                for (int i = 0; i < curr->numKeys; ++i) {
                    std::cout << curr->keys[i] << (i + 1 < curr->numKeys ? "," : "");
                }
                std::cout << (curr->isLeaf ? "*L" : "") << "] ";

                if (!curr->isLeaf) {
                    for (int i = 0; i <= curr->numKeys; ++i) {
                        if (curr->children[i]) q.push(curr->children[i]);
                    }
                }
            }
            std::cout << "\n";
        }
    }

    void destroy(BPlusNode* node) {
        if (!node) return;
        if (!node->isLeaf) {
            for (int i = 0; i <= node->numKeys; ++i) {
                destroy(node->children[i]);
            }
        }
        delete node;
    }

public:
    BPlusTree() : root(nullptr) {}
    ~BPlusTree() { destroy(root); }

    void insert(const Student& s) {
        if (!root) {
            root = new BPlusNode(true);
            root->keys[0] = s.id;
            root->records[0] = s;
            root->numKeys = 1;
            return;
        }

        // If root is full, split root and create new root
        if (root->numKeys == (BPLUS_ORDER - 1)) {
            BPlusNode* newRoot = new BPlusNode(false);
            newRoot->children[0] = root;
            splitChild(newRoot, 0, root);
            root = newRoot;
        }
        insertNonFull(root, s);
    }

    // Exact search by ID
    bool search(int id, Student& result) const {
        BPlusNode* leaf = findLeaf(root, id);
        if (!leaf) return false;
        for (int i = 0; i < leaf->numKeys; ++i) {
            if (leaf->keys[i] == id) {
                result = leaf->records[i];
                return true;
            }
        }
        return false;
    }

    // Range query [minId, maxId] using linked leaves
    void rangeQuery(int minId, int maxId) const {
        if (!root) {
            std::cout << Color::YELLOW << " [Notice] B+ Tree is empty.\n" << Color::RESET;
            return;
        }

        std::cout << "\n" << Color::GREEN << Color::BOLD 
                  << "--- Range Query Results for Student IDs in [" << minId << " to " << maxId << "] ---" 
                  << Color::RESET << "\n";

        BPlusNode* curr = findLeaf(root, minId);
        int matchCount = 0;

        std::cout << Color::CYAN
                  << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                  << "| ID     | Name               | Div  | Contact      | Average | Marks (M, P, C, CS) |\n"
                  << "+--------+--------------------+------+--------------+---------+---------------------+\n"
                  << Color::RESET;

        while (curr) {
            for (int i = 0; i < curr->numKeys; ++i) {
                if (curr->keys[i] >= minId && curr->keys[i] <= maxId) {
                    curr->records[i].printRow();
                    matchCount++;
                } else if (curr->keys[i] > maxId) {
                    curr = nullptr; // Stop traversal immediately!
                    break;
                }
            }
            if (curr) curr = curr->next; // Advance to next linked leaf block
        }

        std::cout << Color::CYAN << "+--------+--------------------+------+--------------+---------+---------------------+\n" << Color::RESET;
        std::cout << " Found " << matchCount << " student records within range [" << minId << ", " << maxId << "].\n";
    }

    // Display level by level
    void displayLevels() const {
        if (!root) {
            std::cout << Color::YELLOW << " [Notice] B+ Tree is empty.\n" << Color::RESET;
            return;
        }
        std::cout << "\n" << Color::CYAN << Color::BOLD 
                  << "--- B+ Tree Level-by-Level Hierarchical Structure (*L = Leaf Node) ---" 
                  << Color::RESET << "\n";
        displayLevelHelper(root);
    }

    // Display all leaf records in sorted order by traversing linked leaves
    void displayLeafSequence() const {
        if (!root) {
            std::cout << Color::YELLOW << " [Notice] B+ Tree is empty.\n" << Color::RESET;
            return;
        }
        std::cout << "\n" << Color::MAGENTA << Color::BOLD 
                  << "--- Sequential Leaf-Level Scan (Linked Leaves) ---" << Color::RESET << "\n";
        
        // Find leftmost leaf
        BPlusNode* curr = root;
        while (!curr->isLeaf) curr = curr->children[0];

        int leafBlock = 1;
        while (curr) {
            std::cout << " Leaf Block #" << leafBlock++ << ": ";
            for (int i = 0; i < curr->numKeys; ++i) {
                std::cout << "[" << curr->keys[i] << ": " << curr->records[i].name << "] ";
            }
            std::cout << (curr->next ? "--> " : "--> [END]\n");
            curr = curr->next;
        }
    }
};

// ============================================================================
// Module 6 Manager & Menu Handler
// ============================================================================
class TreeIndexManager {
private:
    BinarySearchTree bst;
    AVLTree avl;
    BPlusTree bPlus;

public:
    TreeIndexManager() {}

    // Synchronize trees with a list of students
    void populateTrees(const std::vector<Student>& students) {
        for (const auto& s : students) {
            bst.insert(s);
            avl.insert(s);
            bPlus.insert(s);
        }
    }

    void insertStudentAcrossTrees(const Student& s) {
        bst.insert(s);
        avl.insert(s);
        bPlus.insert(s);
    }

    void deleteStudentAcrossTrees(int id) {
        bst.remove(id);
        avl.remove(id);
    }

    void runMenu(const std::vector<Student>& sharedStudents) {
        while (true) {
            std::cout << "\n" << Color::BLUE << Color::BOLD 
                      << "========================================================\n"
                      << "       MODULE 6: HIERARCHICAL INDEXING USING TREES      \n"
                      << "========================================================\n"
                      << Color::RESET;
            std::cout << " [SECTION 1: Binary Search Tree (BST)]\n"
                      << "   1. BST Traversals (Inorder, Preorder, Postorder)\n"
                      << "   2. BST Height & Level-Order Traversal\n"
                      << "   3. Display BST Parent Nodes with Children & Leaf Nodes\n"
                      << "   4. Display BST Mirror Image & Deep Copy Test\n"
                      << "   5. Delete a Student Key from BST\n"
                      << " [SECTION 2: AVL Tree (Self-Balancing)]\n"
                      << "   6. Display AVL Tree (Height & Balance Factors)\n"
                      << "   7. Insert Test Keys Triggering LL, RR, LR, RL Rotations\n"
                      << "   8. Search in AVL Tree\n"
                      << " [SECTION 3: B+ Tree Indexing]\n"
                      << "   9. Display B+ Tree Level-by-Level\n"
                      << "  10. Display B+ Tree Sequential Linked Leaves\n"
                      << "  11. Perform Range Query [MinID to MaxID] on B+ Tree\n"
                      << " [SECTION 4: Comparative Complexity]\n"
                      << "  12. Comprehensive Comparison: BST vs AVL vs B+ Tree\n"
                      << "   0. Return to Main Menu\n";

            int choice = InputHelper::getInteger(" Enter your choice (0-12): ");
            if (choice == 0) break;

            switch (choice) {
                case 1:
                    bst.inorder();
                    bst.preorder();
                    bst.postorder();
                    break;
                case 2:
                    std::cout << Color::GREEN << " BST Height: " << bst.getHeight() << Color::RESET << "\n";
                    bst.levelOrder();
                    break;
                case 3:
                    bst.displayParentsAndChildren();
                    bst.displayLeaves();
                    break;
                case 4:
                    bst.displayMirror();
                    bst.verifyDeepCopy();
                    break;
                case 5: {
                    int id = InputHelper::getInteger(" Enter Student ID to delete from BST: ");
                    if (bst.remove(id)) {
                        std::cout << Color::GREEN << " [Success] Node ID " << id << " deleted from BST.\n" << Color::RESET;
                    } else {
                        std::cout << Color::RED << " [Error] ID " << id << " not found in BST.\n" << Color::RESET;
                    }
                    break;
                }
                case 6:
                    avl.displayInorderWithBalance();
                    break;
                case 7: {
                    std::cout << "\n" << Color::CYAN << "--- AVL Rotation Verification Demo ---" << Color::RESET << "\n";
                    AVLTree demoAVL;
                    std::cout << " 1. Demonstrating RR Rotation (Inserting 10, 20, 30)...\n";
                    demoAVL.insert(Student(10, "Demo 10", "A", "00", {}));
                    demoAVL.insert(Student(20, "Demo 20", "A", "00", {}));
                    demoAVL.insert(Student(30, "Demo 30", "A", "00", {}));
                    demoAVL.displayInorderWithBalance();

                    std::cout << " 2. Demonstrating LL Rotation (Inserting 5, 2 into fresh tree)...\n";
                    AVLTree demoLL;
                    demoLL.insert(Student(30, "Demo 30", "A", "00", {}));
                    demoLL.insert(Student(20, "Demo 20", "A", "00", {}));
                    demoLL.insert(Student(10, "Demo 10", "A", "00", {}));
                    demoLL.displayInorderWithBalance();
                    break;
                }
                case 8: {
                    int id = InputHelper::getInteger(" Enter Student ID to search in AVL: ");
                    int comps = 0;
                    AVLNode* node = avl.search(id, comps);
                    if (node) {
                        std::cout << Color::GREEN << " [Found] Student ID " << id << " (" << node->data.name 
                                  << ") located in AVL Tree with " << comps << " comparisons.\n" << Color::RESET;
                    } else {
                        std::cout << Color::RED << " [Not Found] Student ID " << id << " not in AVL Tree.\n" << Color::RESET;
                    }
                    break;
                }
                case 9:
                    bPlus.displayLevels();
                    break;
                case 10:
                    bPlus.displayLeafSequence();
                    break;
                case 11: {
                    int minId = InputHelper::getInteger(" Enter Minimum Student ID: ");
                    int maxId = InputHelper::getInteger(" Enter Maximum Student ID: ");
                    bPlus.rangeQuery(minId, maxId);
                    break;
                }
                case 12: {
                    std::cout << "\n" << Color::YELLOW << Color::BOLD
                              << "========================================================================================\n"
                              << "             COMPARATIVE ANALYSIS: BST vs AVL TREE vs B+ TREE                           \n"
                              << "========================================================================================\n"
                              << Color::RESET
                              << " Criterion             | Binary Search Tree (BST) | AVL Tree (Balanced)      | B+ Tree (Multi-way)\n"
                              << "-----------------------+--------------------------+--------------------------+--------------------------\n"
                              << " Search Best Time      | O(1)                     | O(1)                     | O(1)\n"
                              << " Search Worst Time     | O(N) [Skewed Tree]       | O(log2 N) [Guaranteed]   | O(log_M N) [Guaranteed]\n"
                              << " Insertion Worst Time  | O(N)                     | O(log2 N) (1-2 rotations)| O(log_M N) (node splits)\n"
                              << " Deletion Worst Time   | O(N)                     | O(log2 N)                | O(log_M N)\n"
                              << " Tree Height           | Up to N                  | ~ 1.44 log2 N            | log_M N (Very shallow!)\n"
                              << " Range Query Efficiency| Poor (Inorder recursion) | Moderate (O(K + log N))  | EXCELLENT (Linked leaves)\n"
                              << " Disk / Database Fit   | Unsuitable               | High pointer overhead    | INDUSTRY STANDARD\n"
                              << "========================================================================================\n"
                              << Color::CYAN
                              << " [!] KEY INSIGHTS:\n"
                              << "   1. BST degenerates into a singly linked list O(N) on sorted insertions.\n"
                              << "   2. AVL Tree strictly enforces balance factor in {-1, 0, +1}, ensuring O(log N).\n"
                              << "   3. B+ Tree keeps ALL data records in leaves and connects them in a singly linked list.\n"
                              << "      For range queries [A to B], B+ Tree searches once for A, then simply walks the\n"
                              << "      leaf pointers sequentially without re-traversing the tree levels!\n"
                              << "========================================================================================\n"
                              << Color::RESET;
                    break;
                }
                default:
                    std::cout << Color::RED << " Invalid option. Please select 0 to 12.\n" << Color::RESET;
            }
            InputHelper::pause();
        }
    }
};

#endif // MODULE6_TREES_HPP
