#include "Graph.h"
#include <iostream>
#include <algorithm>

// Define the graph type to easily written
typedef unordered_map<string, list<pair<string, double>>> CityGraph;
/*
* First string -> cityName
* list -> contains all the city neighbors
* each neighbor -> cityName and distance bet. them
*/

void Graph::addCity(string& cityName) {
    if (cities.find(cityName) == cities.end()) {
        cities[cityName] = {}; // Add the city with empty list
        cout << "City '" << cityName << "' added successfully." << endl;
    }
    else {
        cout << "City '" << cityName << "' already exists." << endl;
    }
}

void Graph::addRoad(string& fromCity, string& toCity, double dist) {
    if (cities.find(fromCity) == cities.end() || cities.find(toCity) == cities.end()) {
        cout << "One or both cities do not exist." << endl;
        return;
    }

    // For undirected graph
    cities[fromCity].emplace_back(toCity, dist);
    cities[toCity].emplace_back(fromCity, dist);

    cout << "Road added between '" << fromCity << "' and '" << toCity << "' with distance " << dist << "." << endl;
}
