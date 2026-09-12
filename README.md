# C++ Route Planning & Graph Analysis Engine

A command-line application that models locations and roads as a weighted, undirected graph, and implements BFS, DFS, and Dijkstra's algorithm (with full path reconstruction) on top of an adjacency-list representation — backed by CSV-based file input, an automated test suite, and performance benchmarking.

## Features

- **Graph model**: weighted, undirected graph via adjacency list (`unordered_map<string, vector<pair<string,int>>>`)
- **Traversals**: BFS and DFS, both verified against cyclic, disconnected, single-vertex, and invalid-input cases
- **Shortest path**: Dijkstra's algorithm using a binary min-heap (`priority_queue` with a custom comparator), with lazy stale-entry handling and full path reconstruction via a parent map
- **Edge validation**: negative weights rejected outright; duplicate edges overwrite the existing weight rather than creating parallel edges
- **File input**: loads graphs from CSV, with malformed-line and missing-file handling that skips bad data without crashing
- **CLI**: interactive menu — load a graph, display it, run BFS/DFS, or query shortest paths — with input validation on every prompt
- **Automated tests**: 16 hand-rolled tests covering construction, validation, all three algorithms, and file I/O (see `tests/`)
- **Benchmarking**: `std::chrono`-based timing harness on synthetic graphs (see `benchmarks/`)

## Build

Requires CMake (3.x+) and a C++17-capable compiler (developed with Apple Clang).

```bash
mkdir -p build
cd build
cmake ..
cmake --build .
```

This produces three executables in `build/`:
- `route_planner` — the interactive CLI
- `run_tests` — the automated test suite
- `run_benchmark` — the performance benchmark

## Run

```bash
./route_planner
```

You'll be prompted for a CSV file path. Try the sample graph: `../data/sample_graph.csv`

CSV format:
from,to,weight
A,B,4
A,C,2


## Test

```bash
./run_tests
```

## Benchmark

```bash
./run_benchmark
```

## Project structure

cpp-route-planning-engine/
├── CMakeLists.txt
├── README.md
├── include/
│ └── Graph.h
├── src/
│ ├── Graph.cpp
│ └── main.cpp
├── data/
│ └── sample_graph.csv
├── tests/
│ └── test_graph.cpp
├── benchmarks/
│ └── benchmark.cpp
└── docs/
└── algorithm_analysis.md


## Complexity

See [`docs/algorithm_analysis.md`](docs/algorithm_analysis.md) for a full breakdown. Summary:

| Algorithm | Time | Space |
|---|---|---|
| BFS | O(V + E) | O(V) |
| DFS | O(V + E) | O(V) |
| Dijkstra | O((V + E) log V) | O(V) |

## Design decisions

- **Undirected graph**: models roads as bidirectional by default.
- **Duplicate edges overwrite**: re-adding an edge between the same two vertices updates its weight rather than creating a parallel edge — matches the real-world semantics of "this road's cost changed."
- **Negative weights rejected**: Dijkstra is not correct with negative weights, so `addEdge` rejects them at insertion time rather than letting them silently corrupt shortest-path results.