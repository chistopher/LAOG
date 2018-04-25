
#include <iostream>
#include <fstream>
#include <cassert>
#include <string>

#include <Graph.h>
#include <Network.h>

using namespace std;


int main(int argc, char* argv[]){

    auto network = Network(Graph::createCircle(1000), &Network::bestResponseTwoNeigh, &Network::linearCost);
    network.m_alpha = 0.49;
    network.m_c = 0.0;
    auto starting = "circle";

    network.save_gexf(starting);
    while(!network.performRound(true)){
        cout << "finished round " << network.m_round << endl;
        network.save_gexf(starting);
    }
    cout << "network converged" << endl;

    return 0;
}