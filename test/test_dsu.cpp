#include <gtest/gtest.h>
#include "DisjointSet.hpp"
TEST(DisjointSetExample, InitialComponents) {
    DisjointSet s(3);
    EXPECT_EQ(s.componentCount(), 3u);
}
// TODO: add your own tests; DSU tests may fail until the final stage.
