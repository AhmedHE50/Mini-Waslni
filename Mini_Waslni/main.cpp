#include "Graph.h"
#include <iostream>
using namespace std;

int main() {
    Graph graph;

    string c = "Cairo";
    string a = "Alexandria";
    string g = "Giza";

    graph.addCity(c);
    graph.addCity(a);
    graph.addCity(g);

    graph.addRoad(c, a, 200.5);
    graph.addRoad(c, g, 10.2);

	return 0;
}