# Algorithm Complexity Analysis

Graph representation: adjacency list, `unordered_map<string, vector<pair<string,int>>>`.
Let V = number of vertices, E = number of edges.

## BFS — `Graph::bfs`
- **Time: O(V + E)**
  Each vertex is enqueued and dequeued at most once (O(V)). Each vertex's adjacency list is scanned exactly once across the whole run, and the total size of all adjacency lists combined is O(E) (O(2E) for an undirected graph, still O(E)).
- **Space: O(V)**
  The `visited` set and the queue can each hold up to V vertices in the worst case (e.g. a star graph where BFS enqueues nearly everything from the source in one level).

## DFS — `Graph::dfs` (recursive)
- **Time: O(V + E)**
  Same reasoning as BFS — every vertex is visited once, every edge examined once.
- **Space: O(V)**
  Dominated by the `visited` set, plus the recursion call stack, which in the worst case (a long path graph) can also reach O(V) depth.

## Dijkstra — `Graph::dijkstra`
- **Time: O((V + E) log V)**
  Every vertex is popped from the priority queue at most once for real processing (stale entries are skipped in O(1) via the `currentDist > dist[current]` check). Each edge can trigger one push to the priority queue (a relaxation), and each push/pop on a binary heap costs O(log V) (heap size is bounded by O(V + E), but since we only push improvements, log of that is still O(log V) asymptotically once E is polynomial in V). Total: O(V log V + E log V) = O((V + E) log V).
- **Space: O(V)**
  `dist` map and `parent` map are both sized O(V). The priority queue can hold up to O(E) entries in the worst case (if every relaxation pushes a new entry before being invalidated), so technically O(V + E) space for the queue — worth mentioning if pressed, but O(V) is the standard headline answer for the maps.

## reconstructPath — `Graph::reconstructPath`
- **Time: O(path length)**, which is bounded by O(V) in the worst case (a path visiting every vertex once).
- **Space: O(path length)**, same bound, for the output vector.

## loadFromCSV — `Graph::loadFromCSV`
- **Time: O(L × D)** where L = number of lines in the file and D = average degree at insertion time (because `addEdge`'s overwrite check does a linear scan of the existing adjacency list for that vertex). For a sparse graph (D small relative to V), this is close to O(L).
- **Space: O(V + E)** for the resulting graph structure.

## addEdge (overwrite policy)
- **Time: O(degree(from) + degree(to))** per call, due to the linear scan for an existing edge (Phase 24/25 decision). Acceptable at this project's scale — a hash-based edge lookup would add complexity without meaningful benefit for typical road-network vertex degrees.
- **Space: O(1)** amortized per call (one new entry per direction, or an in-place update).