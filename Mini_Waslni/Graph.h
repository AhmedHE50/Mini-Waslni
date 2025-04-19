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
	void addCity(string& cityName);
	void addRoad(string& fromCity, string& toCity, double dist);
	// void deleteCity(string& cityName);
	// void deleteRoad(string& fromCity, string& toCity);

	CityGraph& getAllCities() {
		return cities;
	}
};

