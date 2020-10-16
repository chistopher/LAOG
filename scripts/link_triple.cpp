
#include <iostream>
#include <algorithm>
#include <fstream>
#include <utility>
#include <set>
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

    mt19937 gen;
    uniform_real_distribution<> dist(0, 0.999999999);

    auto max_n = (int)1e5;
    auto u = 0.7;

    auto graph = Graph(1);

    // not sure if set or multiset -- multiset benefits potential edges with multiple common neighbors
    auto potentialEdges = vector<pair<int,int>>();
    auto connections = set<pair<int,int>>();

    while(graph.n() < max_n){

        if(dist(gen) > u){
            if((graph.n()+1)%1000==0) cerr << graph.n()+1 << endl;
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
                potentialEdges.emplace_back(edge);
                connections.insert(edge);
            }
            graph.connect(from, to);
            connections.emplace(to,from);
        }
        else {
            /*
             * With probability u convert one potential edge selected
             * at random into an edge.
             */
            if(potentialEdges.empty())
                continue;

            auto edge_index = rand(potentialEdges.size());
            auto edge = potentialEdges[edge_index];
            auto from = edge.first;
            auto to = edge.second;

            // remove chosen edge
            potentialEdges[edge_index] = potentialEdges.back();
            potentialEdges.pop_back();

            // create new potential edges
            for(auto neigh : graph.neighbors(from)){
                pair<int,int> edge = minmax(to,neigh);
                if(connections.find(edge) == connections.end()){
                    potentialEdges.push_back(edge);
                    connections.insert(edge);
                }
            }
            for(auto neigh : graph.neighbors(to)){
                pair<int,int> edge = minmax(from,neigh);
                if(connections.find(edge) == connections.end()){
                    potentialEdges.push_back(edge);
                    connections.insert(edge);
              }
            }

            graph.connect(from, to);
        }
    }


    ofstream f("randomlink.dot");
    f << graph;


    return 0;
}
