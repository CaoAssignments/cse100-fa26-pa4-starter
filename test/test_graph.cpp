#include <gtest/gtest.h>
#include "Graph.hpp"
TEST(GraphExample, OneVertexDFS) {
    Graph g(1);
    EXPECT_EQ(g.dfs(0), (std::vector<std::size_t>{0}));
}
// TODO: add your own DFS, BFS, and Dijkstra tests.
