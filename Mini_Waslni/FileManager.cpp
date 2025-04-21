#include "FileManager.h"
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

void FileManager::loadGraph(Graph& graph, const string& filename) {
	ifstream in(filename);

	if (!in.is_open())
		throw runtime_error("Cannot open file: " + filename);

	json j;
	in >> j;

	CityGraph& cities = graph.getAllCities();
	cities.clear();

	for (auto& city : j.items()) {
		for (auto& neighbor : city.value()) {  // city.value() -> neighbors list where each neighbor constains neighborCityName and distance between source and neighbor
			cities[city.key()].emplace_back(neighbor["neighbor"], neighbor["distance"]);  // city.key() -> Source cityName
		}
	}
}


void FileManager::saveGraph(Graph& graph, const string& filename) {
	ofstream out(filename);

	if (!out.is_open())
		throw runtime_error("Cannot open file: " + filename);

	json j;

	const CityGraph& cities = graph.getAllCities();

	for (auto& city : cities) {
		for (auto& neighbor : city.second) {
			j[city.first].push_back({ {"neighbor", neighbor.first}, {"distance", neighbor.second} });
		}
	}

	out << j.dump(4);
}