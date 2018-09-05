
#include <Graph.h>

#include <algorithm>
#include <cassert>
#include <random>
#include <list>
#include <set>


Graph::Graph(int size) {
    m_adj.resize(size);
    m_n = size;
}

Graph::Graph(Graph&& other)
: m_adj(std::move(other.m_adj)),
  m_n(other.m_n),
  m_m(other.m_m)
{
}

unsigned int Graph::n() const {
    assert(m_adj.size() == m_n);
    return m_n;
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

void Graph::insertVertex() {
    m_n++;
    m_adj.emplace_back();
}

bool Graph::isConnected(int u, int v) const {
    return std::find(m_adj[u].begin(), m_adj[u].end(), v) != m_adj[u].end();
}

std::vector<int> Graph::distances(int v, int maxLayer) const {
    // if no maxLayer given do full bfs
    if(maxLayer < 0)
        maxLayer = m_n -1;

    auto dist = std::vector<int>(m_n, -1);

    std::vector<int> queue(m_n);
    auto q_start = 0;
    auto q_end = 0;

    queue[q_end++] = v; // queue.push_back(v);
    dist[v] = 0;

    // do normal bfs
    while(q_start < q_end) // !queue.empty()
    {
        auto current = queue[q_start++]; // queue.pop_front()

        // skip last layer since it does not change any distances
        if (dist[current] >= maxLayer)
            break;

        // queue all neighbors
        for (auto neighbor : m_adj[current])
            if (dist[neighbor] == -1)
            {
                dist[neighbor] = 1 + dist[current];
                queue[q_end++] = neighbor; // queue.push_back(v);
            }
    }

    return dist;
}

int Graph::distImprovementOfEdge(int v, int additionalEdge, int maxLayer, const std::vector<int> & oldDists) const {

    // remove constness from oldDists but we revert all changes back later
    auto & dist = const_cast<std::vector<int>&>(oldDists);
    static std::vector<std::pair<int,int>> oldEntries;
    oldEntries.reserve(m_n);

    static std::vector<int> queue(m_n);
    queue.reserve(m_n);
    auto q_start = 0;
    auto q_end = 0;

    queue[q_end++] = additionalEdge; // queue.push_back();
    oldEntries.emplace_back(additionalEdge, dist[additionalEdge]); // save old value
    dist[additionalEdge] = 1; // act as if there is a new edge

    // bfs part
    while(q_start < q_end) // !queue.empty()
    {
        auto current = queue[q_start++]; // auto current = queue.pop_front()

        // break on maxlayer-1 coz all neighbors later nodes can at best be maxlayer which is the max in oldDists
        if (dist[current] >= maxLayer - 1)
            break;

        // only update distances that get smaller because of the new edge
        for (auto neighbor : m_adj[current])
            if (1 + dist[current] < dist[neighbor])
            {
                oldEntries.emplace_back(neighbor, dist[neighbor]); // save old values
                dist[neighbor] = 1 + dist[current];
                queue[q_end++] = neighbor; // queue.push_back(neighbor);
            }
    }

    // revert back to old dists
    auto sumImprovement = 0;
    for(auto & entry : oldEntries)
    {
        assert(dist[entry.first] < entry.second);
        sumImprovement += entry.second - dist[ entry.first];
        dist[entry.first] = entry.second;
    }
    oldEntries.clear();

    return sumImprovement;
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

// random walk algorithm for uniform random spanning trees
Graph Graph::createRandomTree(int size, int seed) {

    // Create two partitions, S and T. Initially store all nodes in S.
    auto tree = Graph(size);
    auto T = std::vector<bool>(size, false);
    auto S = size;

    auto gen = std::mt19937(seed);
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



// alternative stuff from here on

std::vector<int> Graph::distImprovementOfTwoNeighs(int v) const {

    auto dist = std::vector<int>(m_n, -1);
    std::vector<int> queue(m_n);
    auto q_start = 0;
    auto q_end = 0;

    queue[q_end++] = v; // queue.push_back(v);
    dist[v] = 0;

    std::vector<std::set<int>> twoNeighsOnShortestPath(m_n);
    while(q_start < q_end) // !queue.empty()
    {
        auto current = queue[q_start++]; // queue.pop_front()

        if(dist[current] == 2)
            twoNeighsOnShortestPath[current].insert(current);

        // queue all neighbors
        for (auto neighbor : m_adj[current]) {
            if (dist[neighbor] == -1) {
                dist[neighbor] = 1 + dist[current];
                queue[q_end++] = neighbor; // queue.push_back(v);
            }
            if(dist[neighbor] == dist[current] + 1 && dist[current] >= 2)
                for(auto each : twoNeighsOnShortestPath[current])
                    twoNeighsOnShortestPath[neighbor].insert(each);
        }
    }

    std::vector<int> improvement(m_n, 0);
    for(auto& each : twoNeighsOnShortestPath)
        for(auto twoNeigh : each)
            improvement[twoNeigh]++;

    return improvement;
}

Graph Graph::shortestPathDAG(int v) const {
    auto DAG = Graph(m_n);
    fillShortestPathDAG(DAG, v);
    return DAG;
}

Graph& Graph::shortestPathDAGWithStaticMemory(int v) const {
    static auto DAG = Graph(m_n);
    DAG.m_adj.resize(m_n);
    for(auto& each : DAG.m_adj)
        each.clear();
    DAG.m_n = m_n;
    fillShortestPathDAG(DAG, v);
    return DAG;
}

void Graph::fillShortestPathDAG(Graph &DAG, int v) const {
    // BFS to compute DAG
    const auto infinity = 2*m_n; // use 2*m_n as infinity
    auto dist = std::vector<int>(m_n, infinity);
    std::vector<int> queue(m_n);
    auto q_start = 0;
    auto q_end = 0;
    queue[q_end++] = v; // queue.push_back(v);
    dist[v] = 0;
    while(q_start < q_end) { // !queue.empty()
        auto current = queue[q_start++]; // queue.pop_front()
        for (auto neighbor : m_adj[current]) {
            if (dist[neighbor] == infinity) {
                // found first shortest path to neighbor
                dist[neighbor] = 1 + dist[current];
                queue[q_end++] = neighbor; // queue.push_back(v);
                DAG.m_adj[current].push_back(neighbor);
            } else if(dist[neighbor] == 1 + dist[current]){
                // found additional shortest path to neighbor
                DAG.m_adj[current].push_back(neighbor);
            }
        }
    }
}

int Graph::reachable(int v) const {
    std::vector<int> queue(m_n);
    std::vector<char> visited(m_n, false);
    auto q_start = 0;
    auto q_end = 0;
    queue[q_end++] = v; // queue.push_back(v);
    visited[v] = true;
    while(q_start < q_end) { // !queue.empty()
        auto current = queue[q_start++]; // queue.pop_front()
        for (auto neighbor : m_adj[current]) {
            if (!visited[neighbor]) {
                visited[neighbor] = true;
                queue[q_end++] = neighbor; // queue.push_back(v);
            }
        }
    }
    return q_end; // number of nodes that were in queue
}

int Graph::sizeOfTwoNeighborhood(int v) const {
    auto dists = distances(v, 2);
    auto result = 0;
    for(auto dist : dists)
        if(dist >= 0 && dist <= 2)
            ++result;
    return result;
}

std::vector<std::pair<int, int>> Graph::neighImprovementOfTwoNeighs(int v) const {

    auto& neighbors = m_adj[v];
    auto done = std::vector<char>(m_n, false);
    auto twoNeighs = std::vector<std::pair<int, int>>();
    twoNeighs.reserve(2000);
    done[v] = true;
    for(auto each : neighbors)
        done[each] = true;
    for(auto each : neighbors){
        for(auto twoNeigh : m_adj[each]){
            if(!done[twoNeigh]){
                done[twoNeigh] = true;
                twoNeighs.emplace_back(twoNeigh, 0);
            }
        }
    }
    for(auto& neigh : twoNeighs){
        int& imp = neigh.second;
        for(auto each : m_adj[neigh.first])
            if(!done[each])
                ++imp;
    }
    return twoNeighs;
}
