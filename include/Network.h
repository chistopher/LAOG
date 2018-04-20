
#include <Graph.h>


// representation of our add only game
class Network{
public:
    using BestResponseFunction = int(Network::*)(int);
    using EdgeCostFunction = double(Network::*)(int);

    Network(Graph&& startingGraph,
            BestResponseFunction bestResponse = &Network::bestResponseTwoNeigh,
            EdgeCostFunction edgeCost = &Network::linearCost);

    const Graph& graph() const;

    void activateAgent(int agent);

    // returns if converged
    bool performRound(bool random);

    // different ways to compute best response
    int bestResponseTwoNeigh(int agent); // dist cost is n - 2-neighborhood
    int bestResponseSumDist(int agent); // dist cost is sum of distances

    double linearCost(int deg);
    double polyCost(int deg);

protected:
    BestResponseFunction m_bestResponse;
    EdgeCostFunction m_edgeCost;

    Graph m_graph;

public:
    unsigned int m_round = 0;
    bool m_converged = false;

    double m_alpha = 0.4;
    double m_c = -1.0;
    double m_beta = 0.8;
};
