// Provided graph representation. Do not modify.
#include "Graph.hpp"
#include <algorithm>
#include <stdexcept>
constexpr long long Graph::INF;
Graph::Graph(std::size_t n) : adjacency_(n) {}
std::size_t Graph::vertexCount() const { return adjacency_.size(); }
void Graph::checkVertex(std::size_t v) const {
    if (v >= vertexCount()) throw std::out_of_range("vertex ID out of range");
}
const std::vector<Edge>& Graph::neighbors(std::size_t v) const {
    checkVertex(v); return adjacency_[v];
}
void Graph::addEdge(std::size_t u, std::size_t v, long long weight) {
    checkVertex(u); checkVertex(v);
    if (weight < 0 || weight > 1000000000LL) throw std::invalid_argument("weight out of range");
    auto insert = [&](std::size_t a, std::size_t b) {
        auto& list = adjacency_[a];
        auto pos = std::lower_bound(list.begin(), list.end(), b,
            [](const Edge& e, std::size_t id) { return e.to < id; });
        if (pos != list.end() && pos->to == b) pos->weight = std::min(pos->weight, weight);
        else list.insert(pos, Edge{b, weight});
    };
    insert(u, v); if (u != v) insert(v, u);
}
