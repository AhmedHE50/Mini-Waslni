#pragma once
#include <iostream>
#include <list>
#include <unordered_map>
using namespace std;


class Graph
{
private:
	unordered_map<string, list<pair<string, double>>> cities; 
	//First string = cityName, list contains all the city neighbors -> each neighbor = cityName, distance between them in double

public:
	void addCity(string cityName);
	void addRoad(string fromCity, string toCity, double dist);
	void deleteCity(string cityName);
	void deleteRoad(string fromCity, string toCity);

	unordered_map<string, list<pair<string, double>>>& getAllCities() {
		return cities;
	}
};

