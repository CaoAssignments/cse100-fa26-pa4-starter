#include <gtest/gtest.h>
#include "Graph.hpp"
TEST(GraphExample, OneVertexDFS) {
    Graph g(1);
    EXPECT_EQ(g.dfs(0), (std::vector<std::size_t>{0}));
}
// TODO: add your own DFS, BFS, and Dijkstra tests.

TEST(GraphExample, BFSIgnoresWeights) {
    Graph g(3);
    g.addEdge(0, 1, 100);
    g.addEdge(0, 2, 1);
    g.addEdge(2, 1, 1);
    auto result = g.bfs(0, 1);
    EXPECT_EQ(result.distance, 1);
    EXPECT_EQ(result.path, (std::vector<std::size_t>{0, 1}));
}

TEST(GraphFinalExample, DijkstraUsesWeights) {
    Graph g(3);
    g.addEdge(0, 1, 100);
    g.addEdge(0, 2, 1);
    g.addEdge(2, 1, 1);
    auto result = g.dijkstra(0, 1);
    EXPECT_EQ(result.distance, 2);
    EXPECT_EQ(result.path, (std::vector<std::size_t>{0, 2, 1}));
}
// These examples have unique optimal paths. For a graph with tied optimal paths,
// test that the returned path is legal and optimal, not that it matches one route.
