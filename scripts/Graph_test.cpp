
#include <iostream>
#include <fstream>
#include <cassert>

#include <Graph.h>
#include <Network.h>

using namespace std;


int main(int argc, char* argv[]){

    auto g = Graph::createRandomTree(100);
    const int v = 13;

    auto realDist = g.distances(v);

    // test maxLayer
    auto distTo5 = g.distances(v, 5);
    for(unsigned int i=0; i<g.n(); ++i)
    {
        if(realDist[i] > 5)
            assert(distTo5[i] == -1);
        else
            assert(distTo5[i] == realDist[i]);
    }

    // test distWidthEdge
    auto distAddEdge = g.distancesWithEdge(v, 67);
    auto dists2 = g.distances(v);
    for(unsigned int i=0; i<g.n(); ++i) // check that nothing has changed
        assert(dists2[i] == realDist[i]);

    g.connect(v, 67);
    auto newDists = g.distances(v);
    for(unsigned int i=0; i<g.n(); ++i)
        assert(distAddEdge[i] == newDists[i]);



    return 0;
}