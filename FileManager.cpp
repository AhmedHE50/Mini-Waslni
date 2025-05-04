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
		cities[city.key()] = {}; // Ensure we add the city even if it has no neighbors

		if (city.value().is_array()) {
			for (auto& neighbor : city.value()) {
				if (neighbor.contains("neighbor") && neighbor.contains("distance")) {
					cities[city.key()].emplace_back(neighbor["neighbor"], neighbor["distance"]);
				}
			}
		}
	}
}

void FileManager::saveGraph(const Graph& graph, const string& filename) {
	ofstream out(filename);

	if (!out.is_open())
		throw runtime_error("Cannot open file: " + filename);

	json j;

	const CityGraph& cities = graph.getAllCities();

	for (auto& city : cities) {
		if (city.second.empty()) {
			j[city.first] = json::array();
		}
		else {
			for (auto& neighbor : city.second) {
				j[city.first].push_back({
					{"neighbor", neighbor.first},
					{"distance", neighbor.second}
					});
			}
		}
	}

	out << j.dump(4);
}

void FileManager::savePathJSON(const string& start, const string& end, const map<string, pair<double, string>>& shortestMap, const string& filename) {
	json j;
	vector<string> path;
	string current = end;

	// Reconstruct path backwards
	while (current != start && shortestMap.count(current)) {
		path.push_back(current);
		current = shortestMap.at(current).second;
	}
	path.push_back(start);
	reverse(path.begin(), path.end());

	j["path"] = path;
	j["total_distance"] = shortestMap.at(end).first;

	ofstream out(filename);
	out << j.dump(4);
}

void FileManager::savePathJSON(const vector<string>& path, const string& filename) {
	json j;
	j["path"] = path;
	ofstream out(filename);
	out << j.dump(4);
}
