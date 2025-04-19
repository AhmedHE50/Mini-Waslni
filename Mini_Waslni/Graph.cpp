#include "Graph.h"
#include <iostream>
#include <algorithm>
#include<queue>
#include<list>
#include<map>

// Define the graph type to easily written
typedef unordered_map<string, list<pair<string, double>>> CityGraph;
/*
* First string -> cityName
* list -> contains all the city neighbors
* each neighbor -> cityName and distance bet. them
*/

void Graph::addCity(const string& cityName) {
    if (cities.find(cityName) == cities.end()) {
        cities[cityName] = {}; // Add the city with empty list
        cout << "City '" << cityName << "' added successfully." << endl;
    }
    else {
        cout << "City '" << cityName << "' already exists." << endl;
    }
}

void Graph::addRoad(const string& fromCity, const string& toCity, double dist) {
    if (cities.find(fromCity) == cities.end() || cities.find(toCity) == cities.end()) {
        cout << "One or both cities do not exist." << endl;
        return;
    }

    // For undirected graph
    cities[fromCity].emplace_back(toCity, dist);
    cities[toCity].emplace_back(fromCity, dist);

    cout << "Road added between '" << fromCity << "' and '" << toCity << "' with distance " << dist << "." << endl;
}

void Graph::deleteCity(const string& cityName) {
    if (cities.find(cityName) != cities.end()) {
        // Remove the city from the graph
        cities.erase(cityName);
        cout << "City '" << cityName << "' deleted successfully." << endl;

        // Remove all roads pointing to the deleted city
        for (auto it = cities.begin(); it != cities.end(); ++it) {
            auto& roads = it->second;

            auto roadIt = roads.begin();
            while (roadIt != roads.end()) {
                if (roadIt->first == cityName) {
                    roadIt = roads.erase(roadIt); // Erase the road and move to the next
                }
                else {
                    ++roadIt; // Move to the next road
                }
            }
        }

        cout << "All roads connected to '" << cityName << "' deleted." << endl;
    }
    else {
        cout << "City '" << cityName << "' does not exist." << endl;
    }
}

void Graph::BFS(string& cityName) {
	queue<string> Names;
	Names.push(cityName);
	//int n = cities[cityName].size();
	map<string, bool>Visited;
	for (const auto& pair : cities) {
		Visited[pair.first] = 0;
	}
	//fill(Visited, Visited + n, 0);
	Visited[cityName] = 1;
	while (!Names.empty()) {
		string currentCity = Names.front();
		Names.pop();
		cout << currentCity << ' ';
		list<pair<string, double>> Neigbours = cities[currentCity];
		for (auto it : Neigbours) {
			if (Visited[it.first] != 1) {
				Names.push(it.first);
				Visited[it.first] = 1;
			}
		}
		
	}
}
