# Greater San Diego walking graph

A larger graph for algorithm testing, separate from the campus demonstration. **No visualization is required for PA4.**

- Vertices: **266,647**; undirected edges: **341,458**.
- Connected components: **4,583**; largest component: **239,321 vertices**.
- Frozen OpenStreetMap snapshot requested: **2026-11-10 00:00:00 UTC**.
- Bounding box: west -117.30, south 32.55, east -116.90, north 33.10. This rectangular extract is not an administrative city boundary.

## Files

`graph.txt` has the same format as the campus graph: `N M`, then `M` rows of `u v length_cm`. Edges are undirected, and weights are physical corridor lengths in integer centimeters, not travel time. Accumulate distances with `long long`.

`nodes.tsv` maps graph IDs to OSM IDs and coordinates. `landmarks.tsv` provides nearby network anchors for the campus landmarks. **IDs belong to this dataset: do not reuse campus-graph vertex IDs or closure edge IDs.**

`queries.txt` contains 20 reproducible `source target` pairs, with no header. All pairs are in the largest component and were sampled with a fixed seed. They are practice queries, not a complete set of grading tests. Pass one pair at a time to `graph_tool`; the tool does not take a query-file argument. Hidden tests may use other graphs, disconnected pairs, and boundary cases.

`manifest.json` records SHA-256 checksums. No download or API key is needed to run the graph.

## Processing and interpretation

The OSMnx walking filter is followed by exclusion of explicit restricted access, indoor ways, pedestrian one-way restrictions, and unresolved conditional access. Intermediate geometry points are contracted while keeping intersections and landmark anchors. Self-loops are removed and parallel corridors are collapsed to the shortest corridor. Components are retained; there are no fabricated connections at geometric crossings. BFS minimizes corridor count; Dijkstra minimizes total corridor length. It is not a driving network.

Disconnected regions may reflect access restrictions, missing map connections, or the extract boundary. This is a fixed educational dataset, not a live routing or accessibility service. Distances are rounded sums along corridor geometry, not straight lines between endpoints and not centimeter-accurate surveys.

The campus closure/restoration fixtures apply **only** to `data/walk/graph.txt`. Do not apply them to this graph. DSU can process every edge of this graph to compute connected components.

No two-second grading limit has been established for this dataset. Any scored runtime limit must specify the workload and be calibrated on Gradescope; local timing is for development only.

Data © OpenStreetMap contributors, ODbL 1.0. See [attribution](ATTRIBUTION.md).
