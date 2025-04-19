#include "Graph.h"
#include <iostream>
#include <queue>
#include <list>
#include <stack>

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

void Graph::deleteRoad(const string& fromCity, const string& toCity) {
    // Ensure both cities exist
    if (cities.find(fromCity) == cities.end() || cities.find(toCity) == cities.end()) {
        cout << "One or both cities do not exist." << endl;
        return;
    }

    // Delete the road from fromCity -> toCity
    auto& roads1 = cities.at(fromCity);
    auto roadIt1 = roads1.begin();
    bool found1 = false;
    while (roadIt1 != roads1.end()) {
        if (roadIt1->first == toCity) {
            roadIt1 = roads1.erase(roadIt1); // Erase the road and move to the next
            found1 = true;
            break; // Exit the loop once the road is found and deleted
        }
        else {
            ++roadIt1; // Move to the next road
        }
    }

    // Delete the reverse road from toCity -> fromCity
    auto& roads2 = cities.at(toCity);
    auto roadIt2 = roads2.begin();
    bool found2 = false;
    while (roadIt2 != roads2.end()) {
        if (roadIt2->first == fromCity) {
            roadIt2 = roads2.erase(roadIt2); // Erase the road and move to the next
            found2 = true;
            break; // Exit the loop once the road is found and deleted
        }
        else {
            ++roadIt2; // Move to the next road
        }
    }

    if (found1 && found2) {
        cout << "Road deleted between '" << fromCity << "' and '" << toCity << "'." << endl;
    }
    else {
        cout << "Road between '" << fromCity << "' and '" << toCity << "' does not exist." << endl;
    }
}

void Graph::BFS(const string& cityName) {
	queue<string> Names;
	Names.push(cityName);
	//int n = cities[cityName].size();
    unordered_map<string, bool>Visited;
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
    cout << '\n';
}
               
void Graph::DFS(const string& start) {
    unordered_map<string, bool> visited;
    stack<string> s;

    visited[start] = true;
    s.push(start);

    while (!s.empty()) {
        string current = s.top();
        s.pop();


        cout << current << ' ';

        for (auto& neighbor : cities.at(current)) {
            if (!visited[neighbor.first]) {
                s.push(neighbor.first);
                visited[neighbor.first] = true;
            }
        }
    }

    cout << '\n';
}
