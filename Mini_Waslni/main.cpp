#include "Graph.h"
#include <iostream>
using namespace std;

int main() {
    Graph graph;

    string c = "Cairo";
    string a = "Alexandria";
    string g = "Giza";
    string s = "sharm";

    graph.addCity(c);
    graph.addCity(a);
    graph.addCity(g);
    graph.addCity(s);

    graph.addRoad(c, a, 200.5);
    graph.addRoad(c, g, 10.2);
    graph.addRoad(c, s, 10.2);
    graph.addRoad(a, s, 10.2);

    graph.BFS(a);
    graph.DFS(a);

	return 0;
};