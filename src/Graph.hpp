#ifndef GRAPH_HPP
#define GRAPH_HPP
#include <cstddef>
#include <limits>
#include <vector>
struct Edge { std::size_t to; long long weight; };
struct PathResult { long long distance; std::vector<std::size_t> path; };
class Graph {
public:
    static constexpr long long INF = std::numeric_limits<long long>::max() / 4;
    explicit Graph(std::size_t vertices);
    std::size_t vertexCount() const;
    void addEdge(std::size_t u, std::size_t v, long long weight);
    const std::vector<Edge>& neighbors(std::size_t v) const;
    // TODO: recursive-preorder-equivalent DFS; visit neighbors by ascending ID.
    std::vector<std::size_t> dfs(std::size_t start) const;
    // TODO: any shortest path in number of edges (ignore stored weights).
    PathResult bfs(std::size_t source, std::size_t target) const;
    // TODO: any minimum-total-weight path; all edge weights are nonnegative.
    PathResult dijkstra(std::size_t source, std::size_t target) const;
private:
    std::vector<std::vector<Edge>> adjacency_;
    void checkVertex(std::size_t v) const;
};
#endif
