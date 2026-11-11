#ifndef DISJOINT_SET_HPP
#define DISJOINT_SET_HPP
#include <cstddef>
#include <vector>
class DisjointSet {
public:
    explicit DisjointSet(std::size_t n);
    std::size_t find(std::size_t x);
    bool unite(std::size_t a, std::size_t b);
    bool connected(std::size_t a, std::size_t b);
    std::size_t componentSize(std::size_t x);
    std::size_t componentCount() const;
private:
    // TODO: add parent, size/rank, and component-count state.
};
#endif
