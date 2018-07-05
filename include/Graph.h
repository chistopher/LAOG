
#pragma once

#include <vector>
#include <iostream>
#include <map>

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
    void insertVertex();
    bool isConnected(int u, int v) const;

    // returns distances of each node to v or -1 if not reachable
    std::vector<int> distances(int v, int maxLayer = -1) const;

    // compute distance improvement if the edge (v,additionalEdge) is inserted; maxLayer should be max of oldDists
    int distImprovementOfEdge(int v, int additionalEdge, int maxLayer, const std::vector<int> & oldDists) const;

    // returns vector with distImprovementOfEdge for all edges from v to every node in distance 2
    // nodes not in distance 2 have 0 improvement
    std::vector<int> distImprovementOfTwoNeighs(int v) const;

    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);

    // special constructors
    static Graph createCircle(int size);
    static Graph createPath(int size);
    static Graph createRandomTree(int size, int seed = 1337);

protected:
    std::vector<std::vector<int>> m_adj; // adjacency list
    unsigned int m_n;
    unsigned int m_m = 0;
};