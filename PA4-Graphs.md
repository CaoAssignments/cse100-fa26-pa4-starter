# PA4: Your Campus, Your Algorithms

Implement **depth-first search (DFS), breadth-first search (BFS), Dijkstra's shortest-path algorithm, and a disjoint-set union (DSU)** in C++14. These algorithms answer different questions: Which vertices can I reach? What is the fewest-edge route? What is the cheapest route? Which elements belong to the same connected component?

![UCSD and La Jolla walking graph](media/network.png)

[Watch the 84-second dataset walkthrough](media/campus-algorithms.mp4) (download the MP4 if GitHub does not play it inline).

You are building the algorithmic core of a campus walking-network explorer. The supplied frozen OpenStreetMap dataset has **10,350 vertices and 13,239 undirected edges** around UC San Diego and La Jolla. You do not need an API key, internet access, or a map-download library to run it.

| Question | Algorithm | Your implementation |
|---|---|---|
| Which places remain reachable when selected corridors close? | DFS | Visit the reachable component in a specified order. |
| Which route uses the fewest graph corridors? | BFS | Return a minimum-hop path. |
| Which route has the shortest modeled walking distance? | Dijkstra | Return a minimum-weight path. |
| As corridors reopen, when do disconnected regions reconnect? | DSU | Maintain components under edge additions. |

**A graph edge is a simplified walking corridor, not necessarily a turn or a street.** Intermediate geometry points have been contracted. Weights are integer **centimeters**, measured along retained corridor geometry. BFS ignores those weights; Dijkstra uses them. Coordinates and street names are optional context, not inputs to your algorithm.

The data includes disconnected components. Do not assume the graph is connected. The supplied closure scenario is fictional and exists to exercise the algorithms; it is not a report of current campus construction. Landmark IDs are nearby network anchors, not guaranteed building entrances. See [dataset details](data/walk/README.md) and [OpenStreetMap attribution](data/walk/ATTRIBUTION.md).

## Dates and submission

All times are **Pacific time**. Release: **November 20, 2026, at 12:00 AM**. Checkpoint: **December 1, 2026, at 11:59 PM**. Final: **December 6, 2026, at 11:59 PM**.

Submit `submission.zip` to the corresponding **PA4 Checkpoint** or **PA4** assignment in Gradescope. Include your implementation **and your own testers** in both submissions. The checkpoint covers DFS and BFS; Dijkstra and DSU may remain compilable stubs then. The final requires everything. The checkpoint is a 40-point progress check; the final is assessed out of 100. Do not interpret this as a 140-point final assignment.

Only the **0-point Compile Check** is visible during submission. All scored tests, test outputs, and autograder scores are hidden from students. A successful compile does not establish correctness. Develop and run your own tests. Gradescope grades your active submission (the most recent by default); you may select a previous submission before the deadline. Late submissions are not accepted in the regular window. Any redo window will be announced separately.

## What to implement

| File | Your work |
|---|---|
| `src/Graph.cpp` | Implement `dfs`, `bfs`, and `dijkstra`. Graph construction and storage are provided separately in `GraphStorage.cpp`. |
| `src/Graph.hpp` | Preserve the public interface. Add private helpers if needed. |
| `src/DisjointSet.cpp` and `src/DisjointSet.hpp` | Implement DSU and its private state. Preserve public signatures. |
| `test/test_graph.cpp` | Add your own GoogleTest tests for DFS, BFS, and Dijkstra. |
| `test/test_dsu.cpp` | Add your own GoogleTest tests for DSU. |

We provide `src/GraphStorage.cpp` for graph construction and sorted adjacency lists, `src/graph_tool.cpp` for file parsing and command-line output, `meson.build`, a small graph in `data/example.txt`, the complete walking dataset in `data/walk/`, a Dockerfile, and `make_submission.py`. You do not need to implement file parsing. The starter deliberately compiles while its example tests fail until you implement the algorithms.

You may add helper `.cpp`, `.hpp`, or `.h` files under `src/` and additional GoogleTest `.cpp` files under `test/`. Add them to the appropriate source lists in `meson.build` for local builds. Leave `src/GraphStorage.cpp` and `src/graph_tool.cpp` unchanged. Do not add another `main`: the driver and GoogleTest supply the executable entry points. The grader compiles all submitted implementation/test source files using a trusted Meson configuration; changing your local build options does not change the grader.

## Graph model and provided operations

The graph is **undirected**. Its vertices have IDs `0` through `n - 1`. A graph may have zero vertices; then no vertex ID is valid. Graded inputs use at most 100,000 vertices and 200,000 input edge rows. Edge weights are integers from 0 through 1,000,000,000, inclusive. **Zero-weight edges and self-loops are allowed.** Repeated edges between the same endpoints are stored once with the smallest supplied weight.

| Provided operation | Behavior |
|---|---|
| `Graph(n)` | Creates `n` vertices with no edges. |
| `vertexCount()` | Returns the number of vertices. |
| `addEdge(u, v, weight)` | Adds the undirected edge or reduces an existing edge's weight. Invalid vertex IDs throw `std::out_of_range`; invalid weights throw `std::invalid_argument`. |
| `neighbors(v)` | Returns a read-only vector of `Edge{to, weight}`, sorted by increasing destination vertex ID. Invalid `v` throws `std::out_of_range`. |

Use the adjacency lists already provided. Do not replace them with an adjacency matrix. Query methods must not change the graph, and running one query must not affect later queries.

### DFS: `std::vector<std::size_t> dfs(std::size_t start) const`

Return vertices in **preorder**: visit a vertex when first entering it, then completely explore each unvisited neighbor in increasing ID order. Return only the component reachable from `start`, with each vertex exactly once. An isolated valid vertex returns `{start}`. Invalid `start` throws `std::out_of_range`.

The result must match recursive DFS with ascending neighbor order. An iterative implementation is recommended: a chain can contain 100,000 vertices, which can overflow the C++ call stack. Be careful about when vertices are marked visited; marking every sibling before descending does not always reproduce recursive preorder in graphs with cross edges.

### BFS: `PathResult bfs(std::size_t source, std::size_t target) const`

Use BFS to return a path with the **fewest edges**, ignoring the stored weights. `PathResult.distance` is the number of edges, and `PathResult.path` contains all vertices in order, including both endpoints.

- If both IDs are valid and equal, return `{0, {source}}`.
- If `target` cannot be reached, return `{Graph::INF, {}}`.
- If either ID is invalid, throw `std::out_of_range`. Validate both IDs before handling special cases.
- If several shortest paths exist, **any valid simple shortest path is accepted**. The path must not repeat a vertex.

### Dijkstra: `PathResult dijkstra(std::size_t source, std::size_t target) const`

Use Dijkstra's algorithm to minimize the **sum of edge weights**. Return that sum in `distance`, with the corresponding endpoint-inclusive vertex sequence in `path`. Same-vertex, unreachable, invalid-ID, and tie behavior are the same as BFS.

Store distances in **`long long`**, not `int`: a path can exceed 2,147,483,647. `Graph::INF` is the unreachable sentinel. Nonnegative weights make Dijkstra applicable. A priority queue with stale-entry checks or an ordered set with decrease-key updates is acceptable. A zero-weight cycle must not create a loop in your predecessor chain. Any optimal simple path is accepted; there is no required tie-breaking rule for equal-cost paths.

### DSU: `DisjointSet`

DSU maintains a partition of element IDs `0` through `n - 1`. It is independent of the `Graph` class: the supplied `components` and `restore` commands connect them by calling `unite` for each available edge.

| Function | Expected behavior |
|---|---|
| `DisjointSet(n)` | Creates `n` singleton sets. Zero elements are allowed. |
| `find(x)` | Returns the representative of `x`'s current set. The representative is a valid element ID, and `find(find(x)) == find(x)`. |
| `unite(a, b)` | Merges two distinct sets and returns `true`. If already in the same set, including `a == b`, returns `false` without changing membership, sizes, or count. |
| `connected(a, b)` | Returns whether `a` and `b` belong to the same set. |
| `componentSize(x)` | Returns the number of elements in `x`'s set. |
| `componentCount() const` | Returns the number of sets, including singleton sets. |

Every method with element arguments must validate **all** those arguments and throw `std::out_of_range` for an invalid ID. Failed calls must preserve membership, sizes, and count. Path compression of valid elements is harmless. For an empty DSU, `componentCount()` is zero and every element-based operation throws.

Implement **union by size or union by rank, together with path compression**. Representative IDs are not prescribed: the grader checks equivalence, sizes, and counts, not whether a particular root wins a tie. Do not implement DSU by running a graph traversal for each operation.

Example: start with `DisjointSet(4)`. `unite(0, 1)` returns `true`, `unite(1, 0)` returns `false`, `componentSize(1)` returns `2`, `connected(0, 2)` returns `false`, and `componentCount()` returns `3`.

## Get the starter

Download and extract the ZIP from [the starter repository](https://github.com/CaoAssignments/cse100-fa26-pa4-starter/archive/refs/heads/main.zip), or clone it:

```bash
git clone https://github.com/CaoAssignments/cse100-fa26-pa4-starter.git
cd cse100-fa26-pa4-starter
```

Do not create a public fork containing your solution. Keep your implementation private.

## Build and run

### Ubuntu Linux (including Ubuntu in WSL)

Install tools once:

```bash
sudo apt-get update
sudo apt-get install -y build-essential meson ninja-build libgtest-dev python3 gdb valgrind
```

From the starter repository directory:

```bash
meson setup build
meson compile -C build -j 1
meson test -C build --print-errorlogs
```

Run `meson setup build` once for a new build directory. After edits, run `meson compile` and `meson test` again. Use `meson setup build-debug --buildtype=debug` for an unoptimized debugging build. Meson configures compilation; Ninja invokes your C++ compiler. See the [Meson quickstart](https://mesonbuild.com/Quick-guide.html).

### Docker (macOS, Windows, or Linux)

Install and start [Docker Desktop](https://docs.docker.com/get-started/get-docker/), or Docker Engine on Linux. Confirm `docker version` shows a working server. Run these commands from the repository in a Bash-compatible terminal (macOS Terminal, Linux, or WSL):

```bash
docker build -t cse100-pa4 .
docker run --rm -it -v "$PWD:/work" cse100-pa4 bash
```

Inside the container:

```bash
meson setup build-docker
meson compile -C build-docker -j 1
meson test -C build-docker --print-errorlogs
./build-docker/graph_tool data/example.txt dfs 0
```

Use separate build directories for the container and host. Run `exit` to leave the container; edits in `/work` remain on your computer. Use the Linux container for Valgrind if your host does not support it.

### File and command formats

The input file starts with `n m`, followed by exactly `m` rows of `u v weight`, separated by whitespace. There is no textual header. Each row describes one undirected edge. `data/example.txt` contains:

```text
6 6
0 1 4
0 2 1
2 1 1
1 3 1
2 3 5
3 4 0
```

Vertex 5 is isolated. Once the corresponding algorithms are implemented:

```bash
./build/graph_tool data/example.txt dfs 0
# 0 1 2 3 4

./build/graph_tool data/example.txt bfs 0 4
# 3
# 0 1 3 4

./build/graph_tool data/example.txt dijkstra 0 4
# 3
# 0 2 1 3 4

./build/graph_tool data/example.txt bfs 0 5
# UNREACHABLE

./build/graph_tool data/example.txt components
# 2
# 5 5 5 5 5 1
```

The `#` prefixes above explain the output; the program does not print them. DFS prints a single line of IDs. BFS and Dijkstra print distance followed by a line of IDs, or a single `UNREACHABLE` line. `components` prints the set count followed by each vertex's component size in vertex-ID order. Invalid files, commands, or IDs cause a diagnostic on standard error and a nonzero exit code.

### Debugging tips

Start with tiny graphs you can draw, and compare one step of your algorithm with your drawing. Use `gdb --args ./build-debug/graph_tool data/example.txt bfs 0 4`; `break Graph::bfs`, `run`, `next`, `print`, and `bt` help inspect state and locate crashes. In Linux, run `valgrind --leak-check=full --track-origins=yes ./build/graph_tool data/example.txt dijkstra 0 4` to find invalid memory access and leaks. Fix the first reported error before chasing later ones. Check both directions of an undirected edge, reset per-query state, and test unreachable vertices and zero-weight cycles.

## Explore UCSD and La Jolla

The driver already loads `data/walk/graph.txt`. It reads **graph vertex IDs**, not OpenStreetMap IDs. The full landmark table is `data/walk/landmarks.tsv`:

| Landmark | Vertex ID |
|---|---:|
| Geisel Library | 8725 |
| Price Center | 8018 |
| Birch Aquarium | 6585 |
| La Jolla Cove | 8334 |
| Kellogg Park | 7167 |
| UC San Diego Central Campus | 7242 |

These commands use `build`; substitute `build-docker` inside Docker. The outputs below require working implementations; the untouched starter has TODO stubs.

```bash
./build/graph_tool data/walk/graph.txt bfs 8018 7242
./build/graph_tool data/walk/graph.txt dijkstra 8018 7242
```

BFS returns distance **23** (corridors). Dijkstra returns **76086** (centimeters = **760.86 meters**). Even the shortest-distance 23-corridor route is 984.10 meters: minimizing hops is genuinely different from minimizing walking distance. A valid optimal path need not be identical to the path shown in the video. Grade checks accept ties according to the API contracts above.

### Reachability with closures

Append a closure file to `dfs`, `bfs`, `dijkstra`, or `components` to omit those edge rows before building the graph:

```bash
./build/graph_tool data/walk/graph.txt dfs 8725
./build/graph_tool data/walk/graph.txt dfs 8725 data/walk/closures.tsv
./build/graph_tool data/walk/graph.txt bfs 8725 6585 data/walk/closures.tsv
./build/graph_tool data/walk/graph.txt components data/walk/closures.tsv
```

Geisel's DFS result has **9,523 vertices** before the simulated closures and **5,609** afterward. The closed graph has **209 components**. The Geisel-to-Birch query prints `UNREACHABLE`. A DFS result is a **visit order**, not a route: consecutive output vertices need not share an edge after backtracking.

### Restore corridors with DSU

```bash
./build/graph_tool data/walk/graph.txt restore data/walk/closures.tsv data/walk/restoration.tsv 8725 6585
```

The provided driver creates a DSU, unites every initially open edge, then calls `unite` once for each restoration event. You implement the DSU operations; you do not implement file parsing or a separate dynamic graph algorithm. Standard DSU cannot delete an edge: closures are applied **before** the DSU is initialized, and the event stream only adds edges.

The output is a tab-separated table:

```text
step  edge_id  merged  components  source_component_size  connected
```

- `step`: 0 for the initial closed state, then the numbered restoration events.
- `edge_id`: restored edge's zero-based row number after the `graph.txt` header; `-` at step 0.
- `merged`: 1 if `unite` merged distinct components, 0 if redundant; `-` at step 0.
- `components`: current number of components, including isolated vertices.
- `source_component_size`: size of the component containing the requested source.
- `connected`: 1 if the requested source and target are connected, otherwise 0.

For Geisel → Birch, step 0 has `components=209`, `source_component_size=5609`, and `connected=0`. They first connect at **step 78**. After all 84 events, there are **185 components**, and Geisel's component has **9,523 vertices**. Several restorations are redundant; do not decrement the count for every event.

`closures.tsv` starts with the header `edge_id` followed by unique valid edge IDs. `restoration.tsv` starts with `step<TAB>edge_id`; steps start at 1 and are consecutive, and edge IDs must be unique members of the closure list. A restoration file may contain only a prefix or subset of closures. The driver rejects malformed scenario files. Edge IDs refer to input rows; in custom graphs with duplicate endpoints, closing one row does not close other rows for that pair.

Metadata files and geometry are optional for visualization. Your submission must work on other graphs as well: **do not hard-code landmark IDs, dataset counts, paths, or event answers.** No visualization or video production is required from students.

## Testing and grading

| Area | Checkpoint | Final |
|---|---:|---:|
| DFS: basic traversal, cross edges, boundary cases, deep graph | 20 | 20 |
| BFS: shortest hops, ties, boundary cases, repeated queries | 20 | 20 |
| Dijkstra: weighted routes, zero edges/ties, large distances, boundaries, repeated queries, sparse graphs | — | 30 |
| DSU: initialization, merging, sizes/counts, boundaries, larger operation sequences | — | 25 |
| Memory safety across working implementations | — | 5 |
| Compile Check | 0 | 0 |
| **Total** | **40** | **100** |

Memory safety requires functional operations as well as no Valgrind errors or leaks; empty stubs do not earn memory points. Your testers must compile but are not independently scored. Use GoogleTest without defining your own `main`.

Expected query complexity, with `V` vertices and `E` stored undirected edges:

- DFS and BFS: `O(V + E)` time. BFS uses `O(V)` extra space; DFS may use `O(V + E)` stack space.
- Dijkstra: `O((V + E) log(V + 1))` time and `O(V + E)` extra space.
- DSU: `O(n)` storage and near-constant amortized operations using balancing and path compression.

These bounds exclude the provided graph-construction code. There is no hardware-specific CPU-speed score or comparison to a staff laptop. Staff may review whether the required algorithms were implemented; passing black-box tests alone does not prove algorithm choice.

Test singleton and empty structures, self-loops, duplicate edges, disconnected components, cycles, tied paths, invalid IDs, repeated queries, distances larger than `int`, and repeated DSU unions. At the checkpoint, you can run only your relevant tests with `./build/student_tests --gtest_filter='GraphExample.*'` (adjust the filter to include the test suites you wrote). Dijkstra and DSU stubs still have to compile.

## Package and submit

Run from the repository root:

```bash
python3 make_submission.py
```

Upload the resulting `submission.zip`. It contains all C++ source/header files under `src/` and `test/`, including these required files:

```text
src/Graph.hpp
src/GraphStorage.cpp
src/Graph.cpp
src/DisjointSet.hpp
src/DisjointSet.cpp
src/graph_tool.cpp
test/test_graph.cpp
test/test_dsu.cpp
```

Do not submit build directories, executable binaries, datasets, or Docker images. Keep the directory structure. Verify that Compile Check passes and that you uploaded the intended version. Re-download your ZIP if needed and inspect its contents before the deadline.

You may use AI tools, but you are responsible for understanding every part of the code you submit. Keep your solution private. Do not publish your implementation or fork the starter into a public solution repository. Ask implementation-specific questions privately on Piazza or during help hours.
