#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <queue>
#include <limits>

class Graph {
public:
    void addVertex(const std::string& name);
    void addEdge(const std::string& from, const std::string& to, int weight);
    bool hasVertex(const std::string& name) const;
    bool loadFromCSV(const std::string& filepath);
    void display() const;
    std::vector<std::string> bfs(const std::string& start) const;
    std::vector<std::string> dfs(const std::string& start) const;
    std::pair<std::unordered_map<std::string, int>, std::unordered_map<std::string, std::string>>
        dijkstra(const std::string& start) const;
    std::vector<std::string> reconstructPath(
    const std::unordered_map<std::string, std::string>& parent,
    const std::string& start,
    const std::string& end) const;

private:
    std::unordered_map<std::string, std::vector<std::pair<std::string, int>>> adjList;
    void dfsHelper(const std::string& current, std::unordered_set<std::string>& visited, std::vector<std::string>& result) const;
};