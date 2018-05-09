
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

#include <Graph.h>
#include <Network.h>


using namespace std;


int main(int argc, char* argv[]){

    auto network = Network(Graph::createRandomTree(1000), &Network::bestResponseTwoNeigh, &Network::linearCost);
    network.m_alpha = 0.49;
    network.m_c = 0.0;
    network.m_greedy = true;

    do {
        // network.save_gexf();
        cout << "starting round " << network.m_round + 1 << endl;
    } while (!network.performRound(true));
    cout << "network converged" << endl;

    return 0;
}