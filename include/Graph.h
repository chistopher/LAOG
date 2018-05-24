
#pragma once

#include <vector>
#include <iostream>

class Graph
{
public:
    explicit Graph(int size);
    Graph(Graph&& other);

    unsigned int n() const;
    unsigned int m() const;
    unsigned int deg(const int v) const;
    const std::vector<int>& neighbors(int v) const;

    void connect(int u, int v);

    // returns distances of each node to v or -1 if not reachable
    std::vector<int> distances(int v, int maxLayer = -1) const;

    // compute distance improvement if the edge (v,additionalEdge) is inserted; maxLayer should be max of oldDists
    int distImprovementOfEdge(int v, int additionalEdge, int maxLayer, const std::vector<int> & oldDists) const;

    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);

    // special constructors
    static Graph createCircle(int size);
    static Graph createPath(int size);
    static Graph createRandomTree(int size);

protected:
    std::vector<std::vector<int>> m_adj; // adjacency list
    unsigned int m_n;
    unsigned int m_m = 0;
};