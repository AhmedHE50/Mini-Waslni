#include "FileManager.h"
#include "Graph.h"
#include <iostream>
typedef map<string, pair<double, string>> dijk;
using namespace std;

//// Test function for Kruskal's algorithm
//void testKruskal() {
//    // Create a sample graph
//    CityGraph cities;
//    Graph g;
//
//    // Add cities and connections
//    // City A connections
//    cities["A"].push_back(make_pair("B", 2.0));
//    cities["A"].push_back(make_pair("C", 3.0));
//
//    // City B connections
//    cities["B"].push_back(make_pair("A", 2.0));
//    cities["B"].push_back(make_pair("C", 1.0));
//    cities["B"].push_back(make_pair("D", 4.0));
//
//    // City C connections
//    cities["C"].push_back(make_pair("A", 3.0));
//    cities["C"].push_back(make_pair("B", 1.0));
//    cities["C"].push_back(make_pair("D", 2.0));
//
//    // City D connections
//    cities["D"].push_back(make_pair("B", 4.0));
//    cities["D"].push_back(make_pair("C", 2.0));
//    cities["D"].push_back(make_pair("E", 5.0));
//
//    // City E connections
//    cities["E"].push_back(make_pair("D", 5.0));
//
//    // Run Kruskal's algorithm
//    cout << "Running Kruskal's algorithm on test graph:" << endl;
//    g.kruskalMST(cities);
//}

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

    //testKruskal();

    FileManager::saveGraph(graph, "test.json");

	return 0;
};