#ifndef MODULE7_CAMPUS_GRAPH_HPP
#define MODULE7_CAMPUS_GRAPH_HPP

#include "Common.hpp"
#include <queue>
#include <stack>
#include <vector>

const int INF = 1e9; // Representation for infinity / no direct edge

struct GraphEdge {
    int to;
    int weight;
    GraphEdge* next;
    GraphEdge(int t, int w, GraphEdge* n = nullptr) : to(t), weight(w), next(n) {}
};

class CampusNetwork {
private:
    int numVertices;
    std::vector<std::string> buildingNames;
    std::vector<std::vector<int>> adjMatrix; // Adjacency Matrix representation
    std::vector<GraphEdge*> adjList;         // Adjacency List representation

    void clearAdjList() {
        for (int i = 0; i < numVertices; ++i) {
            GraphEdge* curr = adjList[i];
            while (curr) {
                GraphEdge* temp = curr;
                curr = curr->next;
                delete temp;
            }
            adjList[i] = nullptr;
        }
    }

public:
    CampusNetwork(int vertices = 7) : numVertices(vertices) {
        init(vertices);
        loadDefaultCampusMap();
    }

    ~CampusNetwork() {
        clearAdjList();
    }

    void init(int v) {
        clearAdjList();
        numVertices = v;
        buildingNames.resize(numVertices);
        for (int i = 0; i < numVertices; ++i) {
            buildingNames[i] = "Building " + std::to_string(i);
        }
        adjMatrix.assign(numVertices, std::vector<int>(numVertices, 0));
        adjList.assign(numVertices, nullptr);
    }

    void loadDefaultCampusMap() {
        init(7);
        buildingNames = {
            "Main Gate / Admin",       // 0
            "IT & CS Department",      // 1
            "Central Library",         // 2
            "Central Auditorium",      // 3
            "Student Hostels",         // 4
            "Sports Arena / Ground",   // 5
            "Cafeteria & Student Mess" // 6
        };

        // Undirected weighted roads between campus buildings (distances in meters)
        addEdge(0, 1, 150);
        addEdge(0, 2, 200);
        addEdge(1, 2, 80);
        addEdge(1, 3, 220);
        addEdge(2, 4, 300);
        addEdge(3, 5, 180);
        addEdge(4, 5, 250);
        addEdge(4, 6, 120);
        addEdge(5, 6, 160);
    }

    // Add an undirected weighted edge to both Adjacency Matrix and Adjacency List
    bool addEdge(int u, int v, int weight) {
        if (u < 0 || u >= numVertices || v < 0 || v >= numVertices) {
            std::cout << Color::RED << " [Error] Vertex indices must be between 0 and " << (numVertices - 1) << ".\n" << Color::RESET;
            return false;
        }
        if (weight < 0) {
            std::cout << Color::RED << " [Error] Edge weight (distance) cannot be negative (" << weight << ").\n" << Color::RESET;
            return false;
        }
        if (u == v) {
            std::cout << Color::RED << " [Error] Self loops are not permitted.\n" << Color::RESET;
            return false;
        }

        // 1. Update Adjacency Matrix
        adjMatrix[u][v] = weight;
        adjMatrix[v][u] = weight;

        // 2. Update Adjacency List
        adjList[u] = new GraphEdge(v, weight, adjList[u]);
        adjList[v] = new GraphEdge(u, weight, adjList[v]);

        return true;
    }

    // Display Adjacency Matrix
    void displayAdjacencyMatrix() const {
        std::cout << "\n" << Color::CYAN << Color::BOLD << "--- Campus Graph Adjacency Matrix (Distances in Meters) ---" << Color::RESET << "\n";
        std::cout << "      ";
        for (int i = 0; i < numVertices; ++i) {
            std::cout << std::setw(6) << ("[" + std::to_string(i) + "]");
        }
        std::cout << "\n      " << std::string(numVertices * 6, '-') << "\n";

        for (int i = 0; i < numVertices; ++i) {
            std::cout << "[" << i << "] |";
            for (int j = 0; j < numVertices; ++j) {
                if (adjMatrix[i][j] == 0) {
                    std::cout << std::setw(6) << "-";
                } else {
                    std::cout << std::setw(6) << adjMatrix[i][j];
                }
            }
            std::cout << "  (" << buildingNames[i] << ")\n";
        }
    }

    // Display Adjacency List
    void displayAdjacencyList() const {
        std::cout << "\n" << Color::CYAN << Color::BOLD << "--- Campus Graph Adjacency List ---" << Color::RESET << "\n";
        for (int i = 0; i < numVertices; ++i) {
            std::cout << " [" << i << "] " << std::setw(25) << std::left << buildingNames[i] << std::right << " -> ";
            GraphEdge* curr = adjList[i];
            if (!curr) {
                std::cout << "[Isolated / No Roads]";
            }
            while (curr) {
                std::cout << "[" << curr->to << " (" << buildingNames[curr->to] << ") : " << curr->weight << "m] ";
                curr = curr->next;
            }
            std::cout << "\n";
        }
    }

    // 1. Breadth-First Search (BFS) Traversal
    void BFS(int startVertex) const {
        if (startVertex < 0 || startVertex >= numVertices) {
            std::cout << Color::RED << " [Error] Invalid start vertex ID.\n" << Color::RESET;
            return;
        }

        std::vector<bool> visited(numVertices, false);
        std::queue<int> q;

        visited[startVertex] = true;
        q.push(startVertex);

        std::cout << "\n" << Color::GREEN << Color::BOLD 
                  << "--- BFS Traversal starting from [" << startVertex << ": " << buildingNames[startVertex] << "] ---" 
                  << Color::RESET << "\n Path: ";

        int visitedCount = 0;
        while (!q.empty()) {
            int u = q.front();
            q.pop();
            visitedCount++;
            std::cout << "[" << u << ": " << buildingNames[u] << "] ";

            // Traverse neighbors using Adjacency List
            GraphEdge* curr = adjList[u];
            while (curr) {
                if (!visited[curr->to]) {
                    visited[curr->to] = true;
                    q.push(curr->to);
                }
                curr = curr->next;
            }
            if (!q.empty()) std::cout << "-> ";
        }
        std::cout << "\n";

        if (visitedCount < numVertices) {
            std::cout << Color::YELLOW << " [Warning] Graph is DISCONNECTED! Only " 
                      << visitedCount << " out of " << numVertices << " buildings were reached.\n" << Color::RESET;
        } else {
            std::cout << Color::GREEN << " [Connected] All campus buildings reached via BFS.\n" << Color::RESET;
        }
    }

    // 2. Depth-First Search (DFS) Traversal
    void dfsHelper(int u, std::vector<bool>& visited, int& count) const {
        visited[u] = true;
        count++;
        std::cout << "[" << u << ": " << buildingNames[u] << "] ";

        // Traverse neighbors
        GraphEdge* curr = adjList[u];
        while (curr) {
            if (!visited[curr->to]) {
                std::cout << "-> ";
                dfsHelper(curr->to, visited, count);
            }
            curr = curr->next;
        }
    }

    void DFS(int startVertex) const {
        if (startVertex < 0 || startVertex >= numVertices) {
            std::cout << Color::RED << " [Error] Invalid start vertex ID.\n" << Color::RESET;
            return;
        }

        std::vector<bool> visited(numVertices, false);
        std::cout << "\n" << Color::MAGENTA << Color::BOLD 
                  << "--- DFS Traversal starting from [" << startVertex << ": " << buildingNames[startVertex] << "] ---" 
                  << Color::RESET << "\n Path: ";
        
        int count = 0;
        dfsHelper(startVertex, visited, count);
        std::cout << "\n";

        if (count < numVertices) {
            std::cout << Color::YELLOW << " [Warning] Disconnected graph! " 
                      << (numVertices - count) << " building(s) unreachable from source.\n" << Color::RESET;
        }
    }

    // 3. Minimum Spanning Tree (MST) using Prim's Algorithm
    void primsMST() const {
        std::cout << "\n" << Color::GREEN << Color::BOLD 
                  << "--- Prim's Minimum Spanning Tree (Optimal Campus Interconnect) ---" << Color::RESET << "\n";

        std::vector<int> key(numVertices, INF);
        std::vector<int> parent(numVertices, -1);
        std::vector<bool> inMST(numVertices, false);

        // Start from vertex 0
        key[0] = 0;

        for (int count = 0; count < numVertices; ++count) {
            // Find vertex with minimum key value not yet in MST
            int minKey = INF;
            int u = -1;

            for (int v = 0; v < numVertices; ++v) {
                if (!inMST[v] && key[v] < minKey) {
                    minKey = key[v];
                    u = v;
                }
            }

            if (u == -1) break; // Graph is disconnected

            inMST[u] = true;

            // Update keys of adjacent vertices
            GraphEdge* edge = adjList[u];
            while (edge) {
                int v = edge->to;
                int weight = edge->weight;
                if (!inMST[v] && weight < key[v]) {
                    key[v] = weight;
                    parent[v] = u;
                }
                edge = edge->next;
            }
        }

        // Display selected MST edges and compute total cost
        int totalCost = 0;
        int edgeCount = 0;

        std::cout << "+---------------------------------------+---------------------------------------+----------+\n"
                  << "| Building A                            | Building B                            | Distance |\n"
                  << "+---------------------------------------+---------------------------------------+----------+\n";

        for (int i = 1; i < numVertices; ++i) {
            if (parent[i] != -1) {
                totalCost += key[i];
                edgeCount++;
                std::cout << "| " << std::setw(37) << std::left << ("[" + std::to_string(parent[i]) + "] " + buildingNames[parent[i]]) << std::right
                          << " | " << std::setw(37) << std::left << ("[" + std::to_string(i) + "] " + buildingNames[i]) << std::right
                          << " | " << std::setw(6) << key[i] << "m |\n";
            }
        }
        std::cout << "+---------------------------------------+---------------------------------------+----------+\n";

        if (edgeCount < numVertices - 1) {
            std::cout << Color::RED << " [Error] Spanning tree could not be formed because graph is disconnected!\n" << Color::RESET;
        } else {
            std::cout << Color::GREEN << Color::BOLD 
                      << " Minimum Total Road / Cabling Cost: " << totalCost << " meters (" << edgeCount << " edges connecting " << numVertices << " buildings)\n" 
                      << Color::RESET;
        }
    }

    // 4. Dijkstra's Algorithm for Shortest Path with Full Path Reconstruction
    void dijkstraShortestPath(int src) const {
        if (src < 0 || src >= numVertices) {
            std::cout << Color::RED << " [Error] Invalid source building ID.\n" << Color::RESET;
            return;
        }

        std::vector<int> dist(numVertices, INF);
        std::vector<int> parent(numVertices, -1);
        std::vector<bool> visited(numVertices, false);

        dist[src] = 0;

        for (int count = 0; count < numVertices; ++count) {
            int minDist = INF;
            int u = -1;

            for (int v = 0; v < numVertices; ++v) {
                if (!visited[v] && dist[v] < minDist) {
                    minDist = dist[v];
                    u = v;
                }
            }

            if (u == -1 || dist[u] == INF) break;

            visited[u] = true;

            // Relaxation of neighbors
            GraphEdge* edge = adjList[u];
            while (edge) {
                int v = edge->to;
                int weight = edge->weight;
                if (!visited[v] && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    parent[v] = u;
                }
                edge = edge->next;
            }
        }

        // Display results with full reconstructed paths
        std::cout << "\n" << Color::GREEN << Color::BOLD 
                  << "--- Dijkstra's Shortest Path from Source: [" << src << ": " << buildingNames[src] << "] ---" 
                  << Color::RESET << "\n";
        std::cout << "+----+---------------------------+----------+---------------------------------------------------+\n"
                  << "| ID | Destination Building       | Distance | Shortest Route Path Sequence                      |\n"
                  << "+----+---------------------------+----------+---------------------------------------------------+\n";

        for (int i = 0; i < numVertices; ++i) {
            std::cout << "| " << std::setw(2) << i 
                      << " | " << std::setw(25) << std::left << buildingNames[i] << std::right << " | ";

            if (dist[i] == INF) {
                std::cout << std::setw(8) << "INF" << " | " << Color::RED << std::setw(49) << std::left << "UNREACHABLE (Disconnected)" << std::right << Color::RESET << " |\n";
            } else {
                std::cout << std::setw(6) << dist[i] << "m | ";
                // Reconstruct path using parent pointers and stack
                std::stack<int> pathStack;
                int curr = i;
                while (curr != -1) {
                    pathStack.push(curr);
                    curr = parent[curr];
                }

                std::string pathStr = "";
                while (!pathStack.empty()) {
                    int p = pathStack.top();
                    pathStack.pop();
                    pathStr += "[" + std::to_string(p) + "]";
                    if (!pathStack.empty()) pathStr += "->";
                }
                std::cout << std::setw(49) << std::left << pathStr << std::right << " |\n";
            }
        }
        std::cout << "+----+---------------------------+----------+---------------------------------------------------+\n";
    }

    // Interactive Menu Handler for Module 7
    void runMenu() {
        while (true) {
            std::cout << "\n" << Color::BLUE << Color::BOLD 
                      << "========================================================\n"
                      << "     MODULE 7: CAMPUS NETWORK & GRAPH OPTIMIZATION      \n"
                      << "========================================================\n"
                      << Color::RESET;
            std::cout << " 1. Display Campus Adjacency Matrix\n"
                      << " 2. Display Campus Adjacency List\n"
                      << " 3. Breadth-First Search (BFS) Traversal\n"
                      << " 4. Depth-First Search (DFS) Traversal\n"
                      << " 5. Prim's Minimum Spanning Tree (MST)\n"
                      << " 6. Dijkstra's Shortest Path from Source Building\n"
                      << " 7. Add Custom Road (Edge) between Buildings\n"
                      << " 8. Reset to Default PCCOE Campus Map\n"
                      << " 9. Algorithmic Complexity Comparison (Matrix vs List & MST vs Dijkstra)\n"
                      << " 0. Return to Main Menu\n";

            int choice = InputHelper::getInteger(" Enter your choice (0-9): ");
            if (choice == 0) break;

            switch (choice) {
                case 1:
                    displayAdjacencyMatrix();
                    break;
                case 2:
                    displayAdjacencyList();
                    break;
                case 3: {
                    int src = InputHelper::getInteger(" Enter Starting Building ID (0 to " + std::to_string(numVertices - 1) + "): ");
                    BFS(src);
                    break;
                }
                case 4: {
                    int src = InputHelper::getInteger(" Enter Starting Building ID (0 to " + std::to_string(numVertices - 1) + "): ");
                    DFS(src);
                    break;
                }
                case 5:
                    primsMST();
                    break;
                case 6: {
                    int src = InputHelper::getInteger(" Enter Source Building ID (0 to " + std::to_string(numVertices - 1) + "): ");
                    dijkstraShortestPath(src);
                    break;
                }
                case 7: {
                    int u = InputHelper::getInteger(" Enter First Building ID (0-" + std::to_string(numVertices - 1) + "): ");
                    int v = InputHelper::getInteger(" Enter Second Building ID (0-" + std::to_string(numVertices - 1) + "): ");
                    int w = InputHelper::getInteger(" Enter Road Distance in meters: ");
                    if (addEdge(u, v, w)) {
                        std::cout << Color::GREEN << " [Success] Road added between [" << u << "] and [" << v << "] (" << w << "m).\n" << Color::RESET;
                    }
                    break;
                }
                case 8:
                    loadDefaultCampusMap();
                    std::cout << Color::GREEN << " [Success] Reset to default PCCOE Campus layout (7 buildings, 9 connecting roads).\n" << Color::RESET;
                    break;
                case 9: {
                    std::cout << "\n" << Color::YELLOW << Color::BOLD
                              << "========================================================================================\n"
                              << "                       CAMPUS GRAPH ALGORITHMIC COMPLEXITY                              \n"
                              << "========================================================================================\n"
                              << Color::RESET
                              << " Operation              | Adjacency Matrix | Adjacency List   | Auxiliary Space\n"
                              << "------------------------+------------------+------------------+-------------------------\n"
                              << " Storage Space          | O(V^2)           | O(V + E)         | Matrix: V^2, List: V+E\n"
                              << " Edge Lookup (u, v)     | O(1)             | O(degree(u))     | O(1)\n"
                              << " BFS Traversal          | O(V^2)           | O(V + E)         | O(V) (Queue)\n"
                              << " DFS Traversal          | O(V^2)           | O(V + E)         | O(V) (Call stack)\n"
                              << " Prim's MST (Array)     | O(V^2)           | O(V^2)           | O(V) (Key, Parent)\n"
                              << " Dijkstra Shortest Path | O(V^2)           | O(E + V log V)*  | O(V) (Dist, Visited)\n"
                              << "------------------------+------------------+------------------+-------------------------\n"
                              << Color::CYAN
                              << " [!] MST vs SHORTEST PATH TREE (DIJKSTRA):\n"
                              << "   * MST (Prim's): Minimizes the TOTAL SUM of all edge weights required to connect\n"
                              << "     every building together without cycles. Does NOT guarantee shortest path\n"
                              << "     between any specific pair of buildings!\n"
                              << "   * Shortest-Path Tree (Dijkstra): Minimizes the INDIVIDUAL distance from one single\n"
                              << "     source to every other building. Total combined weight of tree is often higher\n"
                              << "     than MST, but optimal for student navigation between two points.\n"
                              << "========================================================================================\n"
                              << Color::RESET;
                    break;
                }
                default:
                    std::cout << Color::RED << " Invalid option. Please select 0 to 9.\n" << Color::RESET;
            }
            InputHelper::pause();
        }
    }
};

#endif // MODULE7_CAMPUS_GRAPH_HPP
