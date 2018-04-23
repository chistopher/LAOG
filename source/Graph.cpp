
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

const std::vector<int> &Graph::neighbors(int v) const {
    return m_adj[v];
}

void Graph::connect(int u, int v) {
    m_adj[u].push_back(v);
    m_adj[v].push_back(u);
    ++m_m;
}

std::vector<int> Graph::distances(int v, int additionalEdge) const {
    // act as if an additional edge from v is present in the graph
    if(additionalEdge >= 0 && additionalEdge < (int)n() && additionalEdge != v)
        const_cast<Graph*>(this)->m_adj[v].push_back(additionalEdge);

    const auto _n = n();
    auto dist = std::vector<int>(_n, -1);

    auto queue = std::vector<int>(_n, -1);
    auto q_start = 0;
    auto q_end = 0;
    queue[q_end++] = v; // queue.push_back(v);
    dist[v] = 0;
    

    // do normal bfs
    while(q_start < q_end)
    {
        auto current = queue[q_start++];

        // queue all neighbors
        for (auto neighbor : m_adj[current])
            if (dist[neighbor] == -1)
            {
                dist[neighbor] = 1 + dist[current];
                queue[q_end++] = neighbor;
            }
    }

    // remove the temporary edge again
    if(additionalEdge >= 0 && additionalEdge < (int)n() && additionalEdge != v)
        const_cast<Graph*>(this)->m_adj[v].pop_back();

    return dist;
}

Graph Graph::createCircle(int size) {
    auto circle = createPath(size);
    circle.connect(0, size-1);
    return circle;
}

Graph Graph::createPath(int size) {
    auto path = Graph(size);
    for(int i=0; i<size-1; ++i)
        path.connect(i, i+1);
    return path;
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

