
#include <iostream>
#include <algorithm>
#include <fstream>
#include <utility>
#include <map>
#include <cassert>

#include <Graph.h>
#include <Network.h>

using namespace std;



int rand(int max){
    static mt19937 gen;
    static uniform_real_distribution<> dist(0, 0.999999999);
    return (int)(dist(gen)*max);
}


int main(int argc, char* argv[]){

    if(argc < 3){
        cout << "too few params" << endl;
        return 0;
    }

    mt19937 gen;
    uniform_real_distribution<> dist(0, 0.999999999);

    auto max_n = stoi(argv[1]);
    auto u = stod(argv[2]);

    auto graph = Graph(1);

    // not sure if set or multiset -- multiset benefits potential edges with multiple common neighbors
    auto potentialEdges = vector<pair<int,int>>();
    auto isPotential = map<pair<int,int>, bool>();

    while(graph.n() < max_n){

        if(dist(gen) > u){
            /*
             * With probability 1-u introduce a new vertex in the
             * graph, create an edge from the new vertex to a vertex j selected
             * at random (implying the creation of a potential edge
             * between the new vertex and all the neighbors of j).
             */
            auto from = graph.n();
            auto to = rand(graph.n());
            graph.insertVertex();
            for(auto neigh : graph.neighbors(to)){
                pair<int,int> edge = {neigh, from};
                if(isPotential.find(edge) == isPotential.end()){
                    potentialEdges.emplace_back(edge);
                    isPotential[edge] = true;
                }
            }
            graph.connect(from, to);
        }
        else {
            /*
             * With probability u convert one potential edge selected
             * at random into an edge.
             */
            bool done = false;
            while(!done && !potentialEdges.empty()){
                auto edge_index = rand(potentialEdges.size());
                auto edge = potentialEdges[edge_index];
                auto from = edge.first;
                auto to = edge.second;

                // remove chosen edge
                potentialEdges[edge_index].first = potentialEdges.back().first;
                potentialEdges[edge_index].second = potentialEdges.back().second;
                potentialEdges.resize(potentialEdges.size()-1);

                //if(graph.isConnected(from, to))
                    //continue;

                // create new potential edges
                for(auto neigh : graph.neighbors(from)){
                    pair<int,int> edge = {to,neigh};
                    if(edge.first > edge.second) swap(edge.first, edge.second);
                    if(isPotential.find(edge) == isPotential.end() && !graph.isConnected(to, neigh)){
                        potentialEdges.push_back(edge);
                        isPotential[edge] = true;
                    }
                }
                for(auto neigh : graph.neighbors(to)){
                    pair<int,int> edge = {from,neigh};
                    if(edge.first > edge.second) swap(edge.first, edge.second);
                    if(isPotential.find(edge) == isPotential.end() && !graph.isConnected(from, neigh)){
                        potentialEdges.push_back(edge);
                        isPotential[edge] = true;
                  }
                }

                graph.connect(from, to);
                done = true;
            }
        }
    }


    ofstream f("randomlink.dot");
    f << graph;


    return 0;
}