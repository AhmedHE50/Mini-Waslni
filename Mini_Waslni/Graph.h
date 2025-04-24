#pragma once
#include <unordered_map>
#include <map>
#include <list>
#include <string>
using namespace std;

// Define the graph type to easily written
typedef unordered_map<string, list<pair<string, double>>> CityGraph;
/*
* First string -> cityName
* list -> contains all the city neighbors
* each neighbor -> cityName and distance bet. them
*/


class Graph
{
private:
	CityGraph cities;

public:
	void addCity(const string& cityName);
	void addRoad(const string& fromCity, const string& toCity, double dist);
	void deleteCity(const string& cityName);
	void deleteRoad(const string& fromCity, const string& toCity);
	map<string,int> BFS(const string& cityName);
	map<string, pair<double, string>> Dijkstra(const string& cityName);
	void DFS(const string& start);
	map<double, vector<pair<string, string>>> roadExtract(CityGraph& cities);
	bool hasCycle(unordered_map<string, vector<string>>& graph, const string& current, unordered_map<string, bool>& visited, const string& parent);
	void kruskalMST(CityGraph& cities);


	CityGraph& getAllCities() {
		return cities;
	}
};

