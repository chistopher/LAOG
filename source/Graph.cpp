
#include <Graph.h>

#include <cassert>
#include <random>
#include <list>


Graph::Graph(int size) {
    m_adj.resize(size);
}

Graph::Graph(Graph&& other)
: m_adj(std::move(other.m_adj)),
  m_m(other.m_m)
{
}

unsigned int Graph::n() const {
    return m_adj.size();
}

unsigned int Graph::m() const {
    return m_m;
}

unsigned int Graph::deg(const int v) const {
    assert(v < (int)n());
    return m_adj[v].size();
}

void Graph::connect(int u, int v) {
    m_adj[u].push_back(v);
    m_adj[v].push_back(u);
    ++m_m;
}

std::vector<int> Graph::distances(int v, int additionalEdge = -1) const {
    // act as if an additional edge from v is present in the graph
    if(additionalEdge >= 0 && additionalEdge < n() && additionalEdge != v)
        m_adj[v].push_back(additionalEdge);

    const auto n = n();
    auto visited = std::vector<bool>(n, false);
    auto dist = std::vector<int>(n, -1);

    std::list<int> queue;
    visited[v] = true;
    dist[v] = 0;
    queue.push_back(v);

    // do normal bfs
    while(!queue.empty())
    {
        auto current = queue.front();
        queue.pop_front();

        // queue all neighbors
        for (auto neighbor : m_adj[current])
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                dist[neighbor] = 1 + dist[current];
                queue.push_back(neighbor);
            }
    }

    // remove the temporary edge again
    if(additionalEdge >= 0 && additionalEdge < n() && additionalEdge != v)
        m_adj[v].pop_back();

    return dist;
}

Graph Graph::createCircle(int size) {
    auto circle = Graph(size);
    for(int i=0; i<size-1; ++i)
        circle.connect(i, i+1);
    circle.connect(0, size-1);
    return circle;
}

// Wilson's Algorithm for uniform random spanning trees
Graph Graph::createRandomTree(int size) {

    // Create two partitions, S and T. Initially store all nodes in S.
    auto tree = Graph(size);
    auto T = std::vector<bool>(size, false);
    auto S = size;

    auto gen = std::mt19937(1337);
    auto dist = std::uniform_int_distribution<>(0, size-1);

    // Pick a random node, and mark it as visited and the current node.
    auto current_node = dist(gen);
    --S;
    T[current_node] = true;

    // Create a random connected graph
    while(S){
        // Randomly pick the next node from the neighbors of the current node.
        // As we are generating a connected graph, we assume a complete graph.
        auto neighbor_node = dist(gen);
        // If the new node hasn't been visited, add the edge from current to new.
        if(!T[neighbor_node]){
            tree.connect(current_node, neighbor_node);
            --S;
            T[neighbor_node] = true;
        }
        // Set the new node as the current node.
        current_node = neighbor_node;
    }
    return tree;
}

std::ostream &operator<<(std::ostream &os, const Graph &graph) {
    os << "graph graphname {" << '\n';
    for(int i = 0; i < (int)graph.n(); ++i)
        for(auto neighbor : graph.m_adj[i])
            if(i < neighbor)
                os << i << " -- " << neighbor << ";\n";
    os << '}' << std::endl;
    return os;
}









