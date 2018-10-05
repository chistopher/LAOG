
#include <string>
#include <random>

#include <Graph.h>


// representation of our add only game
class Network{
public:
    using BestResponseFunction = int(Network::*)(int) const;
    using EdgeCostFunction = double(Network::*)(int) const;

    Network(Graph&& startingGraph,
            BestResponseFunction bestResponse = &Network::bestResponseTwoNeigh,
            EdgeCostFunction edgeCost = &Network::linearCost,
            std::string startingName = "unknown");

    const Graph& graph() const;

    void activateAgent(int agent);

    // returns if converged
    bool performRound(bool random);

    // pass a pointer to this member to configure the network in constructor
    // returns -1 if agent has no improving move
    int bestResponseTwoNeigh(int agent) const; // dist cost is n - 2-neighborhood
    int bestResponseSumDist(int agent) const; // dist cost is sum of distances

    // pass a pointer to this member to configure the network in constructor
    double linearCost(int deg) const;
    double polyCost(int deg) const;

    // seeds the mt19937 used to perform round in random order
    void seed(unsigned int seed);

    // filename with most meta info; used if no name given in save_*
    std::string filename() const;
    void save_dot(std::string name = "") const;
    void save_gexf(std::string name = "") const;

    struct Stats{
        double approx_ratio;
        double avg_approx;
        double stable_ratio;
    };
    Stats stats() const;

    // alternative bestResponse implementations
protected:
    template<typename T> // takes lambda as move evaluator
    int find_BR(const std::vector<int> &old_dists, T getDistGain) const;
public:
    int neighNaive(int agent) const;
    int neighCurrent(int agent) const;
    int neighUnrolled(int agent) const;
    int distNaive(int agent) const;
    int distImproOfEdge(int agent) const;
    int distImproOfEdgeMaxLayer(int agent) const;
    int distWithSets(int agent) const;
    int distDAGandBFS(int agent) const;
    int distDAGandBFSStatic(int agent) const;

protected:
    BestResponseFunction m_bestResponse;
    EdgeCostFunction m_edgeCost;

    Graph m_graph;
    std::string m_startingName;
    std::mt19937 m_rand;

    // things to remember
    int m_seed;
    bool m_random;

public:
    unsigned int m_round = 0;
    bool m_converged = false;

    bool m_greedy = false;

    double m_alpha = 0.4; // factor for linear or exponent for poly
    double m_c = 0.0;
};
