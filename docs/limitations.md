# Known Limitations

This project is deliberately scoped (~7/10 difficulty) to stay deep rather than broad. The following are known, intentional gaps — not bugs, but boundaries of the current design.

## Graph model
- **Undirected only.** One-way roads (directed edges) are not supported. Every `addEdge` call creates a bidirectional connection.
- **No parallel edges.** Only one edge is allowed between any pair of vertices — re-adding an edge overwrites the existing weight instead of creating a second route between the same two points.
- **No self-loops handling.** `addEdge("A", "A", weight)` is not explicitly guarded against and hasn't been tested; behavior is undefined.

## Algorithms
- **No negative weights, anywhere.** Rejected at `addEdge` time. This also means the project doesn't implement Bellman-Ford — Dijkstra alone is sufficient given the negative-weight rejection invariant.
- **No A\*.** Dijkstra explores uniformly by cost; there's no heuristic-guided search, since that would require spatial coordinates for vertices, which this model doesn't store.
- **Single-source only per call.** `dijkstra(start)` computes distances from one source to all vertices; there's no all-pairs shortest path (e.g. Floyd-Warshall) implementation.

## File input
- **CSV only**, with a fixed 3-column format (`from,to,weight`). No support for other formats (JSON, XML, GraphML, etc.).
- **No streaming/incremental load.** The entire file is read and parsed in one pass; not suited for extremely large files that wouldn't fit in memory.

## CLI
- **Single-graph session.** The CLI loads one graph at startup; there's no way to reload or merge a second file without restarting the program.
- **No graph mutation from the CLI.** You can't add/remove vertices or edges interactively — only load, display, and query.

## Testing & benchmarking
- **Hand-rolled test framework**, not GoogleTest/Catch2 — a deliberate scope decision, not an oversight.
- **Benchmark uses synthetic random graphs**, not real-world road network data, so the numbers reflect algorithmic performance in general, not real-world routing performance specifically.
