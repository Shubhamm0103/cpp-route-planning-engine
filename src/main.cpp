#include <iostream>
#include "Graph.h"

void printPath(const std::vector<std::string>& path) {
    if (path.empty()) {
        std::cout << "  (no path exists)\n";
        return;
    }
    for (size_t i = 0; i < path.size(); ++i) {
        std::cout << path[i];
        if (i != path.size() - 1) std::cout << " -> ";
    }
    std::cout << "\n";
}

void printMenu() {
    std::cout << "\n--- Route Planning Engine ---\n";
    std::cout << "1. Display graph\n";
    std::cout << "2. Run BFS\n";
    std::cout << "3. Run DFS\n";
    std::cout << "4. Run Dijkstra (shortest path)\n";
    std::cout << "5. Quit\n";
    std::cout << "Choose an option: ";
}

int main() {
    Graph g;

    std::string filepath;
    std::cout << "Enter path to graph CSV file: ";
    std::getline(std::cin, filepath);

    if (filepath.empty()) {
        std::cerr << "No file path entered. Exiting.\n";
        return 1;
    }

    if (!g.loadFromCSV(filepath)) {
        std::cerr << "Failed to load graph. Exiting.\n";
        return 1;
    }
    std::cout << "Graph loaded successfully.\n";

    while (true) {
        printMenu();
        std::string choice;
        std::getline(std::cin, choice);

        if (choice == "1") {
            g.display();

        } else if (choice == "2") {
            std::string start;
            std::cout << "Start vertex: ";
            std::getline(std::cin, start);

            if (start.empty()) {
                std::cout << "No vertex entered.\n";
                continue;
            }
            if (!g.hasVertex(start)) {
                std::cout << "Vertex '" << start << "' not found in graph.\n";
                continue;
            }

            auto result = g.bfs(start);
            std::cout << "BFS order: ";
            printPath(result);

        } else if (choice == "3") {
            std::string start;
            std::cout << "Start vertex: ";
            std::getline(std::cin, start);

            if (start.empty()) {
                std::cout << "No vertex entered.\n";
                continue;
            }
            if (!g.hasVertex(start)) {
                std::cout << "Vertex '" << start << "' not found in graph.\n";
                continue;
            }

            auto result = g.dfs(start);
            std::cout << "DFS order: ";
            printPath(result);

        } else if (choice == "4") {
            std::string start, end;
            std::cout << "Start vertex: ";
            std::getline(std::cin, start);

            if (start.empty()) {
                std::cout << "No vertex entered.\n";
                continue;
            }
            if (!g.hasVertex(start)) {
                std::cout << "Vertex '" << start << "' not found in graph.\n";
                continue;
            }

            std::cout << "End vertex: ";
            std::getline(std::cin, end);

            if (end.empty()) {
                std::cout << "No vertex entered.\n";
                continue;
            }
            if (!g.hasVertex(end)) {
                std::cout << "Vertex '" << end << "' not found in graph.\n";
                continue;
            }

            auto [dist, parent] = g.dijkstra(start);
            if (dist.find(end) == dist.end() || dist[end] == std::numeric_limits<int>::max()) {
                std::cout << "No path exists from " << start << " to " << end << "\n";
            } else {
                std::cout << "Shortest path (cost " << dist[end] << "): ";
                printPath(g.reconstructPath(parent, start, end));
            }

        } else if (choice == "5") {
            std::cout << "Goodbye.\n";
            break;

        } else {
            std::cout << "Invalid option, try again.\n";
        }
    }

    return 0;
}