# UCSD / La Jolla walking graph — PA4 dataset v1

A frozen educational graph derived from OpenStreetMap, with snapshot requested at **2026-11-10 00:00:00 UTC**. Bounds: west -117.285, south 32.825, east -117.210, north 32.895 (longitude/latitude degrees). There are **10,350 vertices and 13,239 undirected edges**. Keep this exact version for reproducible outputs; students and graders should not download live data.

## Files and units

- `graph.txt`: first line `vertex_count edge_count`; then `u v length_cm` per edge. Vertex IDs are 0-based and contiguous. Edge ID is the zero-based index of its row after the header. All weights are strictly positive integers in **centimeters**. Use 64-bit accumulated distances. This simple format is sufficient for all four algorithms.
- `nodes.tsv`: `node_id`, original `osm_node_id`, `latitude`, `longitude`. Coordinates are for display, not the edge weights.
- `edges.tsv`: `edge_id`, `u`, `v`, `length_cm`, `name`, `highway`, and source `osm_way_ids`. One row per undirected edge; add both adjacency directions. Names/tags may be blank, and merged metadata values may be joined with `|`.
- `landmarks.tsv`: six named points, original OSM feature type/ID and coordinates, assigned graph node, and snapping distance. These are nearby network anchors, not verified building entrances. The `node_id` is the query input.
- `geometry.geojson`: the retained edge polylines for optional visualization. Match features by `edge_id`. Do not compute distances as a straight line between the simplified edge's endpoints.
- `closures.tsv`: a list of initially unavailable edge IDs for the **simulated** restoration exercise.
- `restoration.tsv`: `step edge_id`, defining the order in which closed edges become available again. Begin at step 0 with all edges except those in `closures.tsv`.
- `manifest.json`: file sizes and SHA-256 checksums for this release.

TSV files have headers and use standard CSV quoting with a tab delimiter. Prefer the provided integer graph format for student parsing; staff can supply a parser for optional labels.

## Graph interpretation

The graph is undirected and models pedestrian connectivity. Edges are simplified corridors: intermediate geometry points were contracted, while intersections, dead ends, and selected landmark anchors were retained. Thus BFS minimizes the number of these corridors, not the number of turns, meters, or traffic signals. Dijkstra minimizes the sum of `length_cm`.

Where multiple corridors had the same endpoints, the shortest one was retained. This preserves minimum-hop and minimum-distance answers in the static graph but omits longer parallel alternatives. Self-loops were removed. For the closure scenario, all parallel alternatives represented by a closed edge are considered unavailable; restoration restores the retained corridor.

Disconnected components remain in the data. Some separations can reflect clipped boundaries, incomplete OSM connections, and access filtering. They are connectivity of this dataset, not evidence that physical neighborhoods are inaccessible. Isolated geometry points with no remaining eligible edges were removed; explicitly isolated vertices can be tested separately in small teaching graphs.

Road lengths are sums of great-circle distances along the original geometry, rounded to the nearest centimeter after simplification, with a minimum weight of 1 cm. This numerical unit supports deterministic arithmetic; it does not imply centimeter survey accuracy.

## Simulated closures and DSU

This is a teaching scenario, not a report of actual closures. Assign a region to each vertex:

1. Latitude below 32.863: `south`.
2. At or above that latitude, longitude below -117.244: `northwest`.
3. Otherwise: `northeast`.

Close every edge whose endpoints belong to different regions (84 edges). Restore them by increasing `(length_cm, edge_id)`. The order is a fixed teaching fixture, not a real repair-time estimate.

DFS can compare reachability before and after these closures. For DSU, first unite endpoints of all initially open edges, then process the restorations with `unite`. Query component counts, component sizes, and whether landmark pairs are connected. Standard DSU is used only for edge additions; no online deletion operation is required. Restoring an edge inside an already connected component must return false and leave the component count unchanged.

## Source and limitations

Source: [OpenStreetMap](https://www.openstreetmap.org/), retrieved via Overpass using OSMnx 2.1.1's walking-network filter. Additional preprocessing excludes explicitly prohibited/restricted foot access, indoor ways, pedestrian one-way restrictions, and unresolved conditional access; it removes disconnected geometry-only points and simplifies the remaining topology. Private ways excluded by the initial walking filter remain excluded even if another tag might grant foot access.

The model does not account for opening hours, current construction, live closures, wheelchair accessibility, grades, or every unmapped gate/restriction. Labels are snapped 7.62–42.03 meters to the retained connected network. It is an algorithm dataset, not a navigation service. Scripps Pier was not included as a query landmark because the nearest selected-network anchor was more than 150 meters from its mapped center.

Data © OpenStreetMap contributors, available under the [Open Database License 1.0](https://opendatacommons.org/licenses/odbl/1-0/). See `ATTRIBUTION.md`. The processed graph is distributed under the same data license. No staff implementation, grading scripts, component-answer labels, or complete expected outputs are included in this dataset package.
