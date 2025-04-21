#include "FileManager.h"
#include "Graph.h"
#include <iostream>
typedef map<string, pair<double, string>> dijk;
using namespace std;

int main() {
    Graph graph;

    /*string c = "Cairo";
    string a = "Alexandria";
    string g = "Giza";
    string s = "sharm";
    string i = "Idlib";
    graph.addCity(c);
    graph.addCity(a);
    graph.addCity(g);
    graph.addCity(s);
    graph.addCity(i);

    graph.addRoad(c, a, 200.5);
    graph.addRoad(c, g, 3.69);
    graph.addRoad(c, s, 10.2);
    graph.addRoad(a, s, 10.2);
    graph.addRoad(s, g, 3.4);
    graph.addRoad(i,s,1.67);*/

    FileManager::loadGraph(graph, "test.json");

    auto res = graph.BFS("Alexandria");
    //graph.DFS(a);
    dijk result = graph.Dijkstra("Cairo");
    cout << endl;
    for (auto it : res)
    {
        cout << "Name of the city: " << it.first
            << " , Level to the city from Source (Alexandria): " << it.second << endl;
    }
    cout << endl << endl;
    for (auto it : result)
    {
        cout << "Name of the city: " << it.first
            << " , Distance to the city from Source (Cairo): " 
            << it.second.first << " , City where I came from: "
            << it.second.second << endl;
    }

    FileManager::saveGraph(graph, "test.json");

	return 0;
};