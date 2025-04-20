#include "Graph.h"
#include <iostream>
#include <queue>
#include <list>
#include <stack>
#include <map>


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

map<string,int> Graph::BFS(const string& cityName) {
	queue<pair<string,int>> Names; // int: distance from the source city
    Names.push({ cityName,0 });
	//int n = cities[cityName].size();
    unordered_map<string, bool>Visited;
	Visited[cityName] = 1;
    // to record distances of cities from the source to all other cities
    map<string, int> distances;
    distances.insert({ cityName,0 });
	while (!Names.empty()) {
		string currentCity = Names.front().first;
        int dist = Names.front().second;
		Names.pop();
		cout << currentCity << ' ';
		for (auto& it : cities[currentCity]) {
			if (Visited[it.first] != 1) {
                Names.push({ it.first, dist + 1 });
				Visited[it.first] = 1;
                distances.insert({ it.first, dist + 1 });
			}
		}
	}
    cout << '\n';
    return distances;
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
map<string, pair<int, string>> Graph::Dijkstra(const string& cityName)
{
    // key: the city || pair<int, string> int: distance from source, string: city where i came from
    map<string, pair<int, string>> shortestPathsFromTheSource;
    if(cities.find(cityName) != cities.end())
    { 
        // int : distance from the source, string: the city of this distance
        priority_queue<pair<int,string>, vector<pair<int,string>>, greater<pair<int, string>>> pq;
        unordered_map<string, int> distToCities;
        unordered_map<string, bool> Visited;
        // collecting keys and initialising it
        for (auto it : cities)
        {
            distToCities.insert({ it.first, INT_MAX });
        }
        distToCities[cityName] = 0;
        shortestPathsFromTheSource.insert({ cityName, {0,"-"} });
        pq.push({ 0,cityName });
        while (!pq.empty())
        {
            int dist = pq.top().first;
            string city = pq.top().second;
            pq.pop();
            Visited[city] = 1;
            for (auto it : cities[city])
            {
                if (it.second + dist < distToCities[it.first] && !Visited[it.first])
                {
                    distToCities[it.first] = it.second + dist;
                    shortestPathsFromTheSource[it.first] = { distToCities[it.first],city };
                    pq.push({ distToCities[it.first] , it.first });
                }
            }
        }
        
    }
    return shortestPathsFromTheSource;
}
