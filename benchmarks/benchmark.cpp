#include <iostream>
#include <chrono>
#include <random>
#include "../include/Graph.h"

Graph generateRandomGraph(int numVertices, int numEdges) {
    Graph g;
    std::mt19937 rng(42); // fixed seed for reproducibility
    std::uniform_int_distribution<int> vertexDist(0, numVertices - 1);
    std::uniform_int_distribution<int> weightDist(1, 100);

    for (int i = 0; i < numVertices; ++i) {
        g.addVertex("V" + std::to_string(i));
    }

    for (int i = 0; i < numEdges; ++i) {
        int from = vertexDist(rng);
        int to = vertexDist(rng);
        if (from != to) {
            g.addEdge("V" + std::to_string(from), "V" + std::to_string(to), weightDist(rng));
        }
    }

    return g;
}

template <typename Func>
double timeIt(Func func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    return elapsed.count();
}

int main() {
    const int numVertices = 1000;
    const int numEdges = 5000;

    std::cout << "Generating graph with " << numVertices << " vertices and "
              << numEdges << " edges...\n";
    Graph g = generateRandomGraph(numVertices, numEdges);
    std::cout << "Graph generated.\n\n";

    std::string start = "V0";

    double bfsTime = timeIt([&]() { g.bfs(start); });
    std::cout << "BFS:      " << bfsTime << " ms\n";

    double dfsTime = timeIt([&]() { g.dfs(start); });
    std::cout << "DFS:      " << dfsTime << " ms\n";

    double dijkstraTime = timeIt([&]() { g.dijkstra(start); });
    std::cout << "Dijkstra: " << dijkstraTime << " ms\n";

    return 0;
}