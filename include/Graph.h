
#pragma once

#include <vector>
#include <iostream>
#include <map>
#include <string>

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


    friend std::ostream& operator<<(std::ostream& os, const Graph& graph);

    // special constructors
    static Graph createCircle(int size);
    static Graph createPath(int size);
    static Graph createRandomTree(int size, int seed = 1337);
    // expects the nodes to be numbered from 0 to n-1
    // lines with # are comments
    // edges are given as "fromID   toID"
    static Graph fromTxt(std::string file, char sep = '\t');

    // functions for alternative dist BR versions

    // returns vector with distImprovementOfEdge for all edges from v to every node in distance 2
    // nodes not in distance 2 have 0 improvement
    std::vector<int> distImprovementOfTwoNeighs(int v) const;
    Graph shortestPathDAG(int v) const;
    Graph& shortestPathDAGWithStaticMemory(int v) const;
    void fillShortestPathDAG(Graph& DAG, int v) const; // helper
    int reachable(int v) const;

    // functions for alternative neigh BR versions
    int sizeOfTwoNeighborhood(int v) const;
    std::vector<std::pair<int, int>> neighImprovementOfTwoNeighs(int v) const;

protected:
    std::vector<std::vector<int>> m_adj; // adjacency list
    unsigned int m_n;
    unsigned int m_m = 0;
};
