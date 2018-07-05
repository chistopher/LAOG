
#include <Network.h>

#include <numeric>
#include <algorithm>
#include <random>
#include <list>
#include <fstream>
#include <string>
#include <sstream>


Network::Network(Graph&& startingGraph, BestResponseFunction bestResponse, EdgeCostFunction edgeCost, std::string startingName)
: m_bestResponse(bestResponse),
  m_edgeCost(edgeCost),
  m_graph(std::move(startingGraph)),
  m_startingName(std::move(startingName))
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
        auto order = std::vector<int>(n);
        std::iota(order.begin(), order.end(), 0);
        std::shuffle(order.begin(), order.end(), m_rand);
        for(auto each : order)
            activateAgent(each);
    }
    m_round += !m_converged;
    m_random |= random;
    return m_converged;
}

int Network::bestResponseTwoNeigh(int agent) const {
    const int n = m_graph.n();

    // bfs up to layer 3
    auto dist = m_graph.distances(agent, 3);

    auto bestResponse = -1;
    auto bestGain = 0.0;

    for(int i = 0; i<n; ++i)
    {
        // only dist 2 vertices are relevant
        if(dist[i] != 2) continue;
        // dist improvement if edge (agent, i) is present
        auto distGain = 0; // TODO try std::count_if
        auto edgeCost = (this->*m_edgeCost)(m_graph.deg(i) + 1);
        for(auto neighbor : m_graph.neighbors(i))
            if(dist[neighbor] == 3) ++distGain;
        auto realGain = distGain - edgeCost;
        if(realGain > bestGain){
            if (m_greedy) return i;
            bestResponse = i;
            bestGain = realGain;
        }
    }

    return bestResponse;
}

int Network::bestResponseSumDist(int agent) const {

    const auto dist = m_graph.distances(agent); // should not be -1 coz connected
    auto maxDist = *std::max_element(dist.begin(), dist.end());

    auto bestResponse = -1;
    auto bestGain = 0.0;

    for(int i = 0; i < (int)m_graph.n(); ++i)
    {
        // only dist 2 vertices are relevant
        if(dist[i] != 2) continue;
        // dist improvement if edge (agent, i) is present
        auto distGain = m_graph.distImprovementOfEdge(agent, i, maxDist, dist);
        auto edgeCost = (this->*m_edgeCost)(m_graph.deg(i)+1);
        auto realGain = distGain - edgeCost;
        if(realGain > bestGain){
            if (m_greedy) return i;
            bestResponse = i;
            bestGain = realGain;
        }

    }
    return bestResponse;
}

double Network::linearCost(int deg) const {
    return std::max(0.0, m_alpha * (deg-1) + m_c);
}

double Network::polyCost(int deg) const {
    return std::max(0.0, std::pow(deg-1, m_alpha) + m_c);
}

void Network::seed(unsigned int seed) {
    m_seed = seed; // only to store value for later lookup
    m_rand.seed(seed);
}

std::string Network::filename() const{
    std::ostringstream ss;
    ss.precision(2);
    ss << std::fixed;
    ss << ((m_bestResponse == &Network::bestResponseSumDist) ? "sumDist" : "twoNeigh");
    ss << '_' << m_graph.n();
    ss << (m_edgeCost == &Network::linearCost ? "_linear" : "_poly");
    ss << '_' << m_alpha << '_' << m_c;
    ss << '_' << m_startingName;
    ss << '_' << std::to_string(m_round);
    return ss.str();
}

void Network::save_dot(std::string name) const{
    if(name.empty()) name = filename();
    std::ofstream file(name + ".dot");
    // store meta info as comments
    file << "/* META INFO:" << '\n';
    file << "round      " << m_round << '\n';
    file << "converged  " << m_converged << '\n';
    file << "seed       " << m_seed << '\n';
    file << "n          " << graph().n() << '\n';
    file << "m          " << graph().m() << '\n';
    file << "start      " << m_startingName << '\n';
    file << "dist       " << ((m_bestResponse == &Network::bestResponseSumDist) ? "dist" : "two") << '\n';
    file << "cost       " << (m_edgeCost == &Network::linearCost ? "linear" : "poly") << '\n';
    file << "a          " << m_alpha << '\n';
    file << "c          " << m_c << '\n';
    file << "greedy     " << m_greedy << '\n';
    file << "random     " << m_random << '\n';
    file << "*/" << "\n\n";

    // write the graph in dot format
    file << graph();
}

void Network::save_gexf(std::string name) const{
    if(name.empty()) name = filename();
    std::ofstream file(name + ".gexf");
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
