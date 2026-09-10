#include "Graph.h"
#include <iostream>
#include <fstream>
#include <sstream>
void Graph::addVertex(const std::string& name) {
    if (adjList.find(name) == adjList.end()) {
        adjList[name] = {};
    }
}

void Graph::addEdge(const std::string& from, const std::string& to, int weight) {
    if (weight < 0) {
        std::cerr << "Error: negative edge weight (" << weight
                   << ") rejected for edge " << from << " - " << to << "\n";
        return;
    }

    addVertex(from);
    addVertex(to);

    // Overwrite if edge already exists (from -> to direction)
    bool foundForward = false;
    for (auto& edge : adjList[from]) {
        if (edge.first == to) {
            edge.second = weight;
            foundForward = true;
            break;
        }
    }
    if (!foundForward) {
        adjList[from].push_back({to, weight});
    }

    // Overwrite if edge already exists (to -> from direction)
    bool foundBackward = false;
    for (auto& edge : adjList[to]) {
        if (edge.first == from) {
            edge.second = weight;
            foundBackward = true;
            break;
        }
    }
    if (!foundBackward) {
        adjList[to].push_back({from, weight});
    }
}

bool Graph::hasVertex(const std::string& name) const {
    return adjList.find(name) != adjList.end();
}

void Graph::display() const {
    for (const auto& pair : adjList) {
        std::cout << pair.first << " -> ";

        if (pair.second.empty()) {
            std::cout << "(no edges)";
        } else {
            for (size_t i = 0; i < pair.second.size(); ++i) {
                std::cout << pair.second[i].first << "(" << pair.second[i].second << ")";
                if (i != pair.second.size() - 1) {
                    std::cout << ", ";
                }
            }
        }

        std::cout << "\n";
    }
}

std::vector<std::string> Graph::bfs(const std::string& start) const {
    std::vector<std::string> result;

    if (!hasVertex(start)) {
        std::cerr << "Error: BFS start vertex '" << start << "' does not exist.\n";
        return result;
    }

    std::unordered_set<std::string> visited;
    std::queue<std::string> q;

    visited.insert(start);
    q.push(start);

    while (!q.empty()) {
        std::string current = q.front();
        q.pop();
        result.push_back(current);

        for (const auto& edge : adjList.at(current)) {
            const std::string& neighbor = edge.first;
            if (visited.find(neighbor) == visited.end()) {
                visited.insert(neighbor);
                q.push(neighbor);
            }
        }
    }

    return result;
}

void Graph::dfsHelper(const std::string& current, std::unordered_set<std::string>& visited, std::vector<std::string>& result) const {
    visited.insert(current);
    result.push_back(current);

    for (const auto& edge : adjList.at(current)) {
        const std::string& neighbor = edge.first;
        if (visited.find(neighbor) == visited.end()) {
            dfsHelper(neighbor, visited, result);
        }
    }
}

std::vector<std::string> Graph::dfs(const std::string& start) const {
    std::vector<std::string> result;

    if (!hasVertex(start)) {
        std::cerr << "Error: DFS start vertex '" << start << "' does not exist.\n";
        return result;
    }

    std::unordered_set<std::string> visited;
    dfsHelper(start, visited, result);

    return result;
}

std::pair<std::unordered_map<std::string, int>, std::unordered_map<std::string, std::string>>
Graph::dijkstra(const std::string& start) const {
    std::unordered_map<std::string, int> dist;
    std::unordered_map<std::string, std::string> parent;

    if (!hasVertex(start)) {
        std::cerr << "Error: Dijkstra start vertex '" << start << "' does not exist.\n";
        return {dist, parent};
    }

    for (const auto& pair : adjList) {
        dist[pair.first] = std::numeric_limits<int>::max();
    }
    dist[start] = 0;

    using PQItem = std::pair<int, std::string>;
    std::priority_queue<PQItem, std::vector<PQItem>, std::greater<PQItem>> pq;

    pq.push({0, start});

    while (!pq.empty()) {
        auto currentPair = pq.top();
        pq.pop();
        int currentDist = currentPair.first;
        std::string current = currentPair.second;

        if (currentDist > dist[current]) {
            continue;
        }

        for (const auto& edge : adjList.at(current)) {
            const std::string& neighbor = edge.first;
            int weight = edge.second;

            int newDist = currentDist + weight;
            if (newDist < dist[neighbor]) {
                dist[neighbor] = newDist;
                parent[neighbor] = current;
                pq.push({newDist, neighbor});
            }
        }
    }

    return {dist, parent};
}

std::vector<std::string> Graph::reconstructPath(
    const std::unordered_map<std::string, std::string>& parent,
    const std::string& start,
    const std::string& end) const {

    std::vector<std::string> path;

    // Unreachable: no parent entry, and end isn't the start itself
    if (end != start && parent.find(end) == parent.end()) {
        return path; // empty
    }

    std::string current = end;
    path.push_back(current);

    while (current != start) {
        current = parent.at(current);
        path.push_back(current);
    }

    std::reverse(path.begin(), path.end());
    return path;
}

bool Graph::loadFromCSV(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        std::cerr << "Error: could not open file '" << filepath << "'\n";
        return false;
    }

    std::string line;
    bool isHeader = true;
    int lineNumber = 0;

    while (std::getline(file, line)) {
        lineNumber++;

        if (isHeader) {
            isHeader = false;
            continue; // skip "from,to,weight"
        }

        if (line.empty()) {
            continue; // skip blank lines
        }

        std::stringstream ss(line);
        std::string from, to, weightStr;

        if (!std::getline(ss, from, ',') ||
            !std::getline(ss, to, ',') ||
            !std::getline(ss, weightStr, ',')) {
            std::cerr << "Warning: malformed line " << lineNumber
                      << " ('" << line << "') skipped\n";
            continue;
        }

        int weight;
        try {
            weight = std::stoi(weightStr);
        } catch (...) {
            std::cerr << "Warning: invalid weight on line " << lineNumber
                      << " ('" << line << "') skipped\n";
            continue;
        }

        addEdge(from, to, weight);
    }

    file.close();
    return true;
}