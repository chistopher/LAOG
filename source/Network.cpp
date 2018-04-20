
#include <Network.h>

#include <Graph.h>

#include <numeric>
#include <algorithm>
#include <random>
#include <list>


Network::Network(Graph&& startingGraph, BestResponseFunction bestResponse, EdgeCostFunction edgeCost)
: m_bestResponse(bestResponse),
  m_edgeCost(edgeCost),
  m_graph(std::move(startingGraph))
{
}

const Graph &Network::graph() const {
    return m_graph;
}

void Network::activateAgent(int agent) {
    auto best = (this->*m_bestResponse)(agent);
    if(best != -1) {
        m_graph.connect(agent, best);
        m_converged = false;
    }
}

bool Network::performRound(bool random) {
    m_converged = true;
    const auto n = m_graph.n();

    if(!random){ // roundRobin
        for(unsigned int i=0; i<n; ++i)
            activateAgent(i);
    } else { // random
        static auto gen = std::mt19937(1337);
        auto order = std::vector<int>(n);
        std::iota(order.begin(), order.end(), 0);
        std::shuffle(order.begin(), order.end(), gen);
        for(auto each : order)
            activateAgent(each);
    }
    ++m_round;
    return m_converged;
}

int Network::bestResponseTwoNeigh(int agent) {
    const auto n = m_graph.n();

    // bfs up to layer 3
    auto dist = std::vector<int>(n, -1);
    auto visited = std::vector<bool>(n,false);
    auto dist2Nodes = std::vector<int>();

    std::list<int> queue;
    visited[agent] = true;
    dist[agent] = 0;
    queue.push_back(agent);

    while(!queue.empty())
    {
        auto current = queue.front();
        queue.pop_front();
        if(dist[current] > 2) break;

        // queue all neighbors
        for (auto neighbor : m_graph.neighbors(current))
            if (!visited[neighbor])
            {
                visited[neighbor] = true;
                queue.push_back(neighbor);
                dist[neighbor] = dist[current] + 1;
                if(dist[neighbor] == 2) dist2Nodes.push_back(neighbor);
            }
    }

    int bestResponse = -1;
    double bestGain = 0;
    for(auto i : dist2Nodes)
    {
        int distGain = 0;
        double edgeCost = (this->*m_edgeCost)(m_graph.deg(i) + 1);
        for(auto neighbor : m_graph.neighbors(i))
            if(dist[neighbor] == 3) distGain++;
        auto realGain = distGain - edgeCost;
        if(realGain > bestGain){
            bestResponse = i;
            bestGain = realGain;
        }
    }

    return bestResponse;
}

int Network::bestResponseSumDist(int agent) {

    auto dist = m_graph.distances(agent); // should be no -1 coz connected
    auto currentDistCost = std::accumulate(dist.begin(), dist.end(), 0); // dist cost
    auto currentEdgeCost = 0.0;
    for(auto neigh : m_graph.neighbors(agent))
        currentEdgeCost += (this->*m_edgeCost)(m_graph.deg(neigh)); // edge cost

    auto bestResponse = -1;
    auto bestCost = currentDistCost + currentEdgeCost;

    for(int i = 0; i < (int)m_graph.n(); ++i)
    {
        if(dist[i] != 2)
            continue;
        auto newDists = m_graph.distances(agent, i);
        auto newDistCost = std::accumulate(newDists.begin(), newDists.end(), 0);
        auto newEdgeCost = currentEdgeCost + (this->*m_edgeCost)(m_graph.deg(i)+1);
        auto newCost = newDistCost + newEdgeCost;
        if(newDistCost + newEdgeCost < bestCost)
        {
            bestResponse = i;
            bestCost = newCost;
        }

    }
    return bestResponse;
}

double Network::linearCost(int deg) {
    return std::max(0.0, m_alpha * deg + m_c);
}

double Network::polyCost(int deg) {
    return std::pow(deg, m_beta);
}
