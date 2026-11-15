#include <gtest/gtest.h>
#include "DisjointSet.hpp"
TEST(DisjointSetExample, InitialComponents) {
    DisjointSet s(3);
    EXPECT_EQ(s.componentCount(), 3u);
}
// TODO: add your own tests; DSU tests may fail until the final stage.

TEST(DisjointSetExample, DuplicateUnionDoesNotChangeCount) {
    DisjointSet s(3);
    EXPECT_TRUE(s.unite(0, 1));
    EXPECT_FALSE(s.unite(1, 0));
    EXPECT_EQ(s.componentCount(), 2u);
    EXPECT_EQ(s.componentSize(1), 2u);
    EXPECT_FALSE(s.connected(0, 2));
}
