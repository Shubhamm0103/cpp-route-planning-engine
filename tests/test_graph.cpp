#include <iostream>
#include <cassert>
#include "../include/Graph.h"

int testsPassed = 0;
int testsFailed = 0;

void check(bool condition, const std::string& testName) {
    if (condition) {
        std::cout << "[PASS] " << testName << "\n";
        testsPassed++;
    } else {
        std::cout << "[FAIL] " << testName << "\n";
        testsFailed++;
    }
}

void testAddVertexAndEdge() {
    Graph g;
    g.addEdge("A", "B", 4);
    check(g.hasVertex("A"), "addEdge creates vertex A");
    check(g.hasVertex("B"), "addEdge creates vertex B");
    check(!g.hasVertex("Z"), "hasVertex returns false for missing vertex");
}

void testNegativeWeightRejected() {
    Graph g;
    g.addEdge("A", "B", -5);
    check(!g.hasVertex("A"), "negative weight edge is fully rejected (no vertex created)");
}

void testDuplicateEdgeOverwrites() {
    Graph g;
    g.addEdge("A", "B", 4);
    g.addEdge("A", "B", 10);
    auto [dist, parent] = g.dijkstra("A");
    check(dist["B"] == 10, "duplicate addEdge overwrites weight (10, not 4 or 14)");
}

void testBFS() {
    Graph g;
    g.addEdge("A", "B", 1);
    g.addEdge("B", "C", 1);
    g.addEdge("A", "C", 1);
    auto result = g.bfs("A");
    check(result.size() == 3, "BFS visits all 3 reachable vertices");
    check(result[0] == "A", "BFS starts at source vertex");
}

void testBFSInvalidStart() {
    Graph g;
    g.addEdge("A", "B", 1);
    auto result = g.bfs("Z");
    check(result.empty(), "BFS on invalid start returns empty result");
}

void testDFS() {
    Graph g;
    g.addEdge("A", "B", 1);
    g.addEdge("B", "C", 1);
    auto result = g.dfs("A");
    check(result.size() == 3, "DFS visits all 3 reachable vertices");
    check(result[0] == "A", "DFS starts at source vertex");
}

void testDijkstraShortestPath() {
    Graph g;
    g.addEdge("A", "B", 4);
    g.addEdge("A", "C", 2);
    g.addEdge("C", "B", 1);
    g.addEdge("B", "D", 5);
    g.addEdge("C", "D", 8);

    auto [dist, parent] = g.dijkstra("A");
    check(dist["D"] == 8, "Dijkstra finds optimal cost 8 (not naive 10)");

    auto path = g.reconstructPath(parent, "A", "D");
    std::vector<std::string> expected = {"A", "C", "B", "D"};
    check(path == expected, "reconstructPath gives A -> C -> B -> D");
}

void testDijkstraUnreachable() {
    Graph g;
    g.addEdge("A", "B", 1);
    g.addEdge("C", "D", 1);
    auto [dist, parent] = g.dijkstra("A");
    auto path = g.reconstructPath(parent, "A", "D");
    check(path.empty(), "reconstructPath returns empty for unreachable destination");
}

void testCSVLoad() {
    Graph g;
    bool result = g.loadFromCSV("../data/sample_graph.csv");
    check(result == true, "loadFromCSV returns true for valid file");
    check(g.hasVertex("A") && g.hasVertex("D"), "loadFromCSV populates expected vertices");
}

void testCSVMissingFile() {
    Graph g;
    bool result = g.loadFromCSV("../data/does_not_exist.csv");
    check(result == false, "loadFromCSV returns false for missing file");
}

int main() {
    std::cout << "Running automated test suite...\n\n";

    testAddVertexAndEdge();
    testNegativeWeightRejected();
    testDuplicateEdgeOverwrites();
    testBFS();
    testBFSInvalidStart();
    testDFS();
    testDijkstraShortestPath();
    testDijkstraUnreachable();
    testCSVLoad();
    testCSVMissingFile();

    std::cout << "\n--- Summary ---\n";
    std::cout << "Passed: " << testsPassed << "\n";
    std::cout << "Failed: " << testsFailed << "\n";

    return (testsFailed == 0) ? 0 : 1;
}