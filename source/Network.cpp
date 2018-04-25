
#include <Network.h>

#include <numeric>
#include <algorithm>
#include <random>
#include <list>
#include <fstream>
#include <string>
#include <sstream>


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
    const int n = m_graph.n();

    // bfs up to layer 3
    auto dist = m_graph.distances(agent, 3);

    int bestResponse = -1;
    double bestGain = 0;
    for(int i = 0; i<n; ++i)
    {
        if(dist[i] != 2) continue;
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

    auto dist = m_graph.distances(agent); // should not be -1 coz connected
    auto maxDist = 0;
    // dist cost
    auto currentDistCost = 0; 
    for (auto each : dist) {
        currentDistCost += each;
        maxDist = std::max(maxDist, each);
    }
    // edge cost 
    auto currentEdgeCost = 0.0;
    for(auto neigh : m_graph.neighbors(agent))
        currentEdgeCost += (this->*m_edgeCost)(m_graph.deg(neigh)); 

    auto bestResponse = -1;
    auto bestCost = currentDistCost + currentEdgeCost;

    for(int i = 0; i < (int)m_graph.n(); ++i)
    {
        // only dist 2 vertices are relevant
        if(dist[i] != 2) continue; 
        // dist cost if edge (agent, i) is present
        auto newDists = m_graph.distancesWithEdge(agent, i, maxDist, dist);
        auto newDistCost = std::accumulate(newDists.begin(), newDists.end(), 0);
        // edge cost (only add cost of new edge)
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
    return std::max(0.0, m_alpha * (deg-1) + m_c);
}

double Network::polyCost(int deg) {
    return std::pow(deg, m_beta);
}

std::string Network::filename(const char* startingGraph, const char* ext) const{
    std::ostringstream ss;
    ss.precision(2);
    ss << std::fixed;
    ss << ((m_bestResponse == &Network::bestResponseSumDist) ? "sumDist" : "twoNeigh");
    ss << '_' << m_graph.n();
    if (m_edgeCost == &Network::linearCost)
        ss << '_' << "linear" << m_alpha << '_' << m_c;
    else
        ss << '_' << "poly" << m_beta << '_' << m_beta;
    ss << '_' << startingGraph;
    ss << '_' << "round" << m_round;
    ss << ext;
    return ss.str();
}

void Network::save_dot(const char* startingGraph) const{
    std::ofstream file(filename(startingGraph, ".dot"));
    file << graph();
}

void Network::save_gexf(const char* startingGraph) const{
    std::ofstream file(filename(startingGraph, ".gexf"));
    file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    file << "<gexf version=\"1.3\">\n";
    file << "<graph mode=\"slice\" defaultedgetype=\"undirected\" timerepresentation=\"timestamp\" timestamp=\"" << m_round << "\">\n";
    file << "<nodes>\n";
    for(int i = 0; i < (int)m_graph.n(); ++i)
        file << "\t<node id=\"" << i << "\"></node>\n";
    file << "</nodes>\n";
    file << "<edges>\n";
    for(int i = 0; i < (int)m_graph.n(); ++i)
        for(auto neighbor : m_graph.neighbors(i))
            if(i < neighbor)
                file << "\t<edge source=\"" << i << "\" target=\"" << neighbor << "\"></edge>\n";
    file << "</edges>\n";
    file << "</graph>\n";
    file << "</gexf>\n";

}
