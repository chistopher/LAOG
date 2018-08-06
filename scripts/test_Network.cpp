
#include <iostream>

#include <Graph.h>
#include <Network.h>


using namespace std;
using BestResponseFunction = int(Network::*)(int) const;

// change to test other starting graphs
const auto seed = 1337;
const auto n = 100;


void assert(bool cond){
    if(!cond){
        cout << "assertion failed" << endl;
        exit(1);
    }
}



void testTwoNeighBRs(){
    auto reference = Network(Graph::createRandomTree(n, seed), &Network::bestResponseTwoNeigh, &Network::linearCost);
    reference.seed(seed);
    reference.m_alpha = 0.4;
    while(true)
        if(reference.performRound(true))
            break;

    BestResponseFunction neighBRs[] = {
            &Network::neighNaive,
            &Network::neighCurrent,
            &Network::neighUnrolled,
    };
    for(auto BR_func : neighBRs){
        auto network = Network(Graph::createRandomTree(n, seed), BR_func, &Network::linearCost);
        network.seed(seed);
        network.m_alpha = 0.4;
        while(true)
            if(network.performRound(true))
                break;

        assert(reference.graph().m() == network.graph().m());
        assert(reference.m_round == network.m_round);
        assert(reference.graph().sizeOfTwoNeighborhood(0) == network.graph().sizeOfTwoNeighborhood(0));
    }
}

void testDistBRs(){
    auto reference = Network(Graph::createRandomTree(n, seed), &Network::bestResponseSumDist, &Network::linearCost);
    reference.seed(seed);
    reference.m_alpha = 1;
    while(true)
        if(reference.performRound(true))
            break;

    BestResponseFunction distBRs[] = {
            &Network::distNaive,
            &Network::distImproOfEdge,
            &Network::distImproOfEdgeMaxLayer,
            &Network::distWithSets,
            &Network::distDAGandBFS,
            &Network::distDAGandBFSStatic,
    };
    for(auto BR_func : distBRs){
        auto network = Network(Graph::createRandomTree(n, seed), BR_func, &Network::linearCost);
        network.seed(seed);
        network.m_alpha = 1;
        while(true)
            if(network.performRound(true))
                break;

        assert(reference.graph().m() == network.graph().m());
        assert(reference.m_round == network.m_round);
        assert(reference.graph().sizeOfTwoNeighborhood(0) == network.graph().sizeOfTwoNeighborhood(0));
    }
}


int main(int argc, char* argv[]){

    testTwoNeighBRs();
    testDistBRs();


    cout << "all tests passed" << endl;
    return 0;
}
