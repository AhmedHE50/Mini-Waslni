#include "FileManager.h"
#include <fstream>
#include "json.hpp"
using json = nlohmann::json;

// Load graph data from a JSON file
void FileManager::loadGraph(Graph& graph, const string& filename) {
	ifstream in(filename);

	if (!in.is_open())
		throw runtime_error("Cannot open file: " + filename);

    // Read the JSON data from the file
	json j;
	in >> j;

    CityGraph& cities = graph.getAllCities(); // Get a reference to the graph's city data
    cities.clear(); // Clear any existing city data

    // Iterate through each city
	for (auto& city : j.items()) {
        cities[city.key()] = {};

		if (city.value().is_array()) {
            // Iterate through each neighbor of the city
			for (auto& neighbor : city.value()) {
				if (neighbor.contains("neighbor") && neighbor.contains("distance")) {
					cities[city.key()].emplace_back(neighbor["neighbor"], neighbor["distance"]);
				}
			}
		}
	}
}

// Save graph data to a JSON file
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

    out << j.dump(4); // Write with indentation
}

// Save a shortest path (start, end, distance) to a JSON file
void FileManager::savePathJSON(const string& start, const string& end, const map<string, pair<double, string>>& shortestMap, const string& filename) {
	json j;
	vector<string> path;
	string current = end;

    // Reconstruct path backwards (from end to start)
	while (current != start && shortestMap.count(current)) {
		path.push_back(current);
		current = shortestMap.at(current).second;
	}
	path.push_back(start);
    reverse(path.begin(), path.end()); // To get the correct order

	j["path"] = path;
	j["total_distance"] = shortestMap.at(end).first;

	ofstream out(filename);
	out << j.dump(4);
}

// Save a path (vector of city names) to a JSON file
void FileManager::savePathJSON(const vector<string>& path, const string& filename) {
	json j;
	j["path"] = path;
	ofstream out(filename);
	out << j.dump(4);
}
