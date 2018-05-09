
#include <string>

#include <Graph.h>


// representation of our add only game
class Network{
public:
    using BestResponseFunction = int(Network::*)(int);
    using EdgeCostFunction = double(Network::*)(int);

    Network(Graph&& startingGraph,
            BestResponseFunction bestResponse = &Network::bestResponseTwoNeigh,
            EdgeCostFunction edgeCost = &Network::linearCost,
            std::string startingName = "unknown");

    const Graph& graph() const;

    void activateAgent(int agent);

    // returns if converged
    bool performRound(bool random);

    // different ways to compute best response
    int bestResponseTwoNeigh(int agent); // dist cost is n - 2-neighborhood
    int bestResponseSumDist(int agent); // dist cost is sum of distances

    double linearCost(int deg);
    double polyCost(int deg);

    std::string filename() const;
    void save_dot(std::string path = ".") const;
    void save_gexf(std::string path = ".") const;

protected:
    BestResponseFunction m_bestResponse;
    EdgeCostFunction m_edgeCost;

    Graph m_graph;
    std::string m_startingName;

public:
    unsigned int m_round = 0;
    bool m_converged = false;

    bool m_greedy = false;

    double m_alpha = 0.4; // factor for linear or exponent for poly
    double m_c = 0.0;

};
