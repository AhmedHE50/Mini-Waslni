#include "Graph.h"
#include <iostream>
#include <queue>
#include <list>
#include <stack>
#include <map>
#include <set>
#include <vector>
#include <limits>

CityGraph& Graph::getAllCities() {
    return cities; // Return a modifiable reference
}

const CityGraph& Graph::getAllCities() const {
    return cities; // Return a const (read-only) reference
}

// Add a city to the graph
void Graph::addCity(const string& cityName) {
    if (cities.find(cityName) == cities.end()) {
        cities[cityName] = {}; // Add the city with an empty list of neighbors
        cout << "City '" << cityName << "' added successfully." << endl;
    }
    else {
        cout << "City '" << cityName << "' already exists." << endl;
    }
}

// Add a road (edge) between two cities
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

// Delete a city and all connected roads
void Graph::deleteCity(const string& cityName) {
    if (cities.find(cityName) != cities.end()) {
        cities.erase(cityName); // Remove the city from the graph
        cout << "City '" << cityName << "' deleted successfully." << endl;

        // Remove roads connected to the deleted city
        for (auto it = cities.begin(); it != cities.end(); ++it) {
            auto& roads = it->second;

            auto roadIt = roads.begin();
            while (roadIt != roads.end()) {
                if (roadIt->first == cityName) {
                    roadIt = roads.erase(roadIt);
                }
                else {
                    ++roadIt;
                }
            }
        }

        cout << "All roads connected to '" << cityName << "' deleted." << endl;
    }
    else {
        cout << "City '" << cityName << "' does not exist." << endl;
    }
}

// Delete a specific road between two cities
void Graph::deleteRoad(const string& fromCity, const string& toCity) {
    if (cities.find(fromCity) == cities.end() || cities.find(toCity) == cities.end()) {
        cout << "One or both cities do not exist." << endl;
        return;
    }

    // Remove road from fromCity to toCity
    auto& roads1 = cities.at(fromCity);
    auto roadIt1 = roads1.begin();
    bool found1 = false;
    while (roadIt1 != roads1.end()) {
        if (roadIt1->first == toCity) {
            roadIt1 = roads1.erase(roadIt1);
            found1 = true;
            break;
        }
        else {
            ++roadIt1;
        }
    }

    // Remove the reverse road from toCity to fromCity
    auto& roads2 = cities.at(toCity);
    auto roadIt2 = roads2.begin();
    bool found2 = false;
    while (roadIt2 != roads2.end()) {
        if (roadIt2->first == fromCity) {
            roadIt2 = roads2.erase(roadIt2);
            found2 = true;
            break;
        }
        else {
            ++roadIt2;
        }
    }

    if (found1 && found2) {
        cout << "Road deleted between '" << fromCity << "' and '" << toCity << "'." << endl;
    }
    else {
        cout << "Road between '" << fromCity << "' and '" << toCity << "' does not exist." << endl;
    }
}

vector<string> Graph::BFS(const string& cityName) {
    unordered_map<string, bool> visited;
    vector<string> bfsOrder;
    queue<string> q;

    q.push(cityName);
    visited[cityName] = true;

    while (!q.empty()) {
        string current = q.front();
        q.pop();
        bfsOrder.push_back(current);

        for (auto& neighbor : cities[current]) {
            if (!visited[neighbor.first]) {
                visited[neighbor.first] = true;
                q.push(neighbor.first);
            }
        }
    }

    return bfsOrder;
}

vector<string> Graph::DFS(const string& start) {
    unordered_map<string, bool> visited;
    vector<string> dfsOrder;
    stack<string> s;

    s.push(start);

    while (!s.empty()) {
        string current = s.top();
        s.pop();

        if (!visited[current]) {
            visited[current] = true;
            dfsOrder.push_back(current);
        }

        for (auto& neighbor : cities.at(current)) {
            if (!visited[neighbor.first]) {
                s.push(neighbor.first);
            }
        }
    }

    return dfsOrder;
}

map<string, pair<double, string>> Graph::Dijkstra(const string& source)
{
    map<string, pair<double, string>> shortestPathsFromTheSource;
    if (cities.find(source) != cities.end())
    {
        priority_queue<pair<double, string>, vector<pair<double, string>>, greater<pair<double, string>>> pq;
        unordered_map<string, double> distToCities;
        unordered_map<string, bool> Visited;

        for (auto it : cities)
        {
            distToCities.insert({ it.first, numeric_limits<double>::max() });
        }
        distToCities[source] = 0;
        shortestPathsFromTheSource.insert({ source, {0,"-"} });
        pq.push({ 0,source });
        while (!pq.empty())
        {
            double dist = pq.top().first;
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

map<double, vector<pair<string, string>>> Graph::roadExtract(CityGraph& cities)
{
    map<double, vector< pair<string, string>>> roads;
    set<pair<string, string>> uniqueRoads;
    for (auto it = cities.begin(); it != cities.end(); ++it) {
        string city = it->first;
        list<pair<string, double>>& neighbors = it->second;
        for (auto neighborIt = neighbors.begin(); neighborIt != neighbors.end(); ++neighborIt) {
            string neighbor = neighborIt->first;
            double weight = neighborIt->second;
            string a = min(city, neighbor);
            string b = max(city, neighbor);
            if (!uniqueRoads.count({ a, b })) {
                roads[weight].push_back({ a, b });
                uniqueRoads.insert({ a, b });
            }
        }
    }
    return roads;
}

bool Graph::hasCycle(unordered_map<string, vector<string>>& graph, const string& current, unordered_map<string, bool>& visited, const string& parent) {

    visited[current] = true;
    for (const string& neighbor : graph.at(current)) {
        if (!visited[neighbor]) {
            if (hasCycle(graph, neighbor, visited, current))
                return true;
        }
        //cycle
        else if (neighbor != parent) {
            return true;
        }
    }
    return false;
}

void Graph::kruskalMST(CityGraph& cities) {
    map<double, vector<pair<string, string>>> sortedEdges = roadExtract(cities);
    set<string> uniqueCities;

    for (auto it = cities.begin(); it != cities.end(); ++it) {
        string city = it->first;
        uniqueCities.insert(city);

        list<pair<string, double>>& neighbors = it->second;
        for (auto neighborIt = neighbors.begin(); neighborIt != neighbors.end(); ++neighborIt) {
            string neighbor = neighborIt->first;
            uniqueCities.insert(neighbor);
        }
    }
    int cityCount = uniqueCities.size();

    vector<pair<pair<string, string>, double>> mst;
    unordered_map<string, vector<string>> mstGraph;

    //initial graph with only nodes
    for (set<string>::iterator cityIt = uniqueCities.begin(); cityIt != uniqueCities.end(); ++cityIt) {
        mstGraph[*cityIt] = vector<string>();
    }

    //iterate ascendingly by cost
    for (map<double, vector<pair<string, string>>>::iterator edgeIt = sortedEdges.begin();
         edgeIt != sortedEdges.end(); ++edgeIt) {

        double weight = edgeIt->first;
        vector<pair<string, string>>& edgeList = edgeIt->second;

        for (unsigned int i = 0; i < edgeList.size(); ++i) {
            string u = edgeList[i].first;
            string v = edgeList[i].second;

            mstGraph[u].push_back(v);
            mstGraph[v].push_back(u);

            unordered_map<string, bool> visited;
            bool cycleFound = false;

            for (set<string>::iterator cityIt = uniqueCities.begin(); cityIt != uniqueCities.end(); ++cityIt) {
                visited[*cityIt] = false;
            }

            if (hasCycle(mstGraph, u, visited, "")) {
                cycleFound = true;
            }

            if (cycleFound) {

                vector<string>& uNeighbors = mstGraph[u];
                auto newEndU = std::remove(uNeighbors.begin(), uNeighbors.end(), v);
                uNeighbors.erase(newEndU, uNeighbors.end());

                vector<string>& vNeighbors = mstGraph[v];
                auto newEndV = std::remove(vNeighbors.begin(), vNeighbors.end(), u);
                vNeighbors.erase(newEndV, vNeighbors.end());
            }
            else {
                mst.push_back(make_pair(make_pair(u, v), weight));

                if (static_cast<long long>(mst.size()) == cityCount - 1) {

                    cout << "Minimum Spanning Tree:" << endl;
                    double totalWeight = 0.0;
                    for (int j = 0; j < mst.size(); ++j) {
                        cout << mst[j].first.first << " -- " << mst[j].first.second
                             << " : " << mst[j].second << endl;
                        totalWeight += mst[j].second;
                    }
                    cout << "Total MST weight: " << totalWeight << endl;

                    return;
                }
            }
        }
    }
}
