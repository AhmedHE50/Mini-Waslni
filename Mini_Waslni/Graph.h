#pragma once
#include <unordered_map>
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
	void BFS(string& cityName);

	CityGraph& getAllCities() {
		return cities;
	}
};

